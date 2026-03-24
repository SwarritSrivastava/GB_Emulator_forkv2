#!/usr/bin/env python3

import json
import os
import re
import subprocess
import urllib.error
import urllib.request
from pathlib import Path


MAX_OVERVIEW_CHARS = 1800
MAX_DIFF_FILES_CHARS = 1200
MAX_DIFF_PATCH_CHARS = 2500
MAX_LOG_DIGEST_CHARS = 2200
MAX_COMMITS = 12
DEFAULT_MODEL = "openai/gpt-oss-20b"
MODEL_PRIORITY = [
    "openai/gpt-oss-20b",
    "openai/gpt-oss-120b",
    "groq/compound",
    "llama-3.3-70b-versatile",
    "qwen/qwen3-32b",
    "meta-llama/llama-4-scout-17b-16e-instruct",
    "llama-3.1-8b-instant",
]


def read_text(path: Path, default: str = "") -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return default


def run_git(args):
    proc = subprocess.run(
        ["git", *args],
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
    )
    if proc.returncode != 0:
        return ""
    return proc.stdout.strip()


def trim(text: str, limit: int) -> str:
    if len(text) <= limit:
        return text
    return text[: limit - 20] + "\n...[truncated]"


def build_fallback_summary(commits_text: str, changed_files: str) -> str:
    lines = [
        "### Push Summary (Fallback)",
        "- AI summary was unavailable, so this is an automatic fallback.",
        "- Review commit list and changed files below.",
        "",
        "### Commit Messages",
        commits_text or "- No commit messages were detected in the push payload.",
        "",
        "### Changed Files",
        changed_files or "- No changed files were detected.",
    ]
    return "\n".join(lines)


def build_log_digest(build_log: str, valgrind_log: str) -> str:
    pattern = re.compile(
        r"(error|failed|fail|assert|warning|segmentation fault|invalid read|definitely lost|leak)",
        re.IGNORECASE,
    )

    def collect(log_name: str, text: str):
        if not text.strip():
            return [f"[{log_name}] log unavailable"]

        hits = []
        seen = set()
        for line in text.splitlines():
            clean = " ".join(line.strip().split())
            if not clean:
                continue
            if not pattern.search(clean):
                continue
            key = clean.lower()
            if key in seen:
                continue
            seen.add(key)
            hits.append(clean)
            if len(hits) >= 18:
                break

        if not hits:
            raw = [" ".join(line.strip().split()) for line in text.splitlines() if line.strip()]
            hits = raw[:6]

        lines = [f"[{log_name}]"]
        lines.extend(f"- {line}" for line in hits)
        return lines

    digest = "\n".join(collect("build_and_test", build_log) + [""] + collect("valgrind", valgrind_log))
    return trim(digest, MAX_LOG_DIGEST_CHARS)


def fetch_available_models(api_key: str):
    req = urllib.request.Request(
        "https://api.groq.com/openai/v1/models",
        headers={
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json",
        },
        method="GET",
    )
    with urllib.request.urlopen(req, timeout=15) as resp:
        payload = json.loads(resp.read().decode("utf-8", errors="replace"))
    data = payload.get("data", [])
    return [item.get("id", "") for item in data if item.get("id")]


def choose_model(api_key: str, requested_model: str) -> str:
    try:
        available = fetch_available_models(api_key)
    except Exception:
        return requested_model or DEFAULT_MODEL

    if requested_model and requested_model in available:
        return requested_model

    for model_id in MODEL_PRIORITY:
        if model_id in available:
            return model_id

    return requested_model or DEFAULT_MODEL


def main():
    event_path = Path(os.environ.get("EVENT_PATH", ""))
    repo = os.environ.get("REPO", "unknown")
    ref_name = os.environ.get("REF_NAME", "unknown")
    groq_api_key = os.environ.get("GROQ_API_KEY", "")
    requested_model = os.environ.get("GROQ_MODEL", "")
    step_summary_path = os.environ.get("GITHUB_STEP_SUMMARY", "")

    event = {}
    if event_path.exists():
        event = json.loads(read_text(event_path, "{}") or "{}")

    commits = event.get("commits", [])
    before = event.get("before", "")
    after = event.get("after", "")

    project_overview = read_text(Path("ai_context_overview.txt"), "")
    commits_text = read_text(Path("ai_context_commits.txt"), "")
    changed_files = read_text(Path("ai_context_changed_files.txt"), "")
    diff_patch = read_text(Path("ai_context_diff.txt"), "")
    logs_digest = read_text(Path("ai_context_logs.txt"), "")

    if not commits_text:
        commit_lines = []
        for commit in commits[:MAX_COMMITS]:
            cid = (commit.get("id") or "")[:7]
            message = (commit.get("message") or "").splitlines()[0].strip()
            author = (commit.get("author") or {}).get("name", "unknown")
            commit_lines.append(f"- {cid} {message} ({author})")

        if len(commits) > MAX_COMMITS:
            commit_lines.append(f"- ... and {len(commits) - MAX_COMMITS} more commit(s)")
        commits_text = "\n".join(commit_lines)

    if not changed_files or not diff_patch:
        if before and after and before != "0" * 40:
            if not changed_files:
                changed_files = run_git(["diff", "--name-status", before, after])
            if not diff_patch:
                diff_patch = run_git(["diff", "--unified=1", "--no-color", before, after])

        if not changed_files:
            changed_files = run_git(["show", "--name-status", "--pretty=format:", "HEAD"])
        if not diff_patch:
            diff_patch = run_git(["show", "--unified=1", "--no-color", "--pretty=format:", "HEAD"])

    if not project_overview:
        project_overview = read_text(Path("README.md"), "")

    project_overview = trim(project_overview, MAX_OVERVIEW_CHARS)
    commits_text = trim(commits_text, 1600)
    changed_files = trim(changed_files, MAX_DIFF_FILES_CHARS)
    diff_patch = trim(diff_patch, MAX_DIFF_PATCH_CHARS)

    if not logs_digest:
        logs_digest = build_log_digest(
            read_text(Path("build_and_test.log"), ""),
            read_text(Path("valgrind_tests.log"), ""),
        )

    prompt = f"""
You are an engineering release assistant for repository `{repo}` on branch `{ref_name}`.

Your task is to produce a professional summary for a GitHub push notification.
Use only the provided repository overview, commit messages, and git diff context.

Output rules:
1. Use Markdown.
2. Keep it concise (80-140 words).
3. Sections required:
    - Push Summary
    - Key Code Changes
    - Potential Risks/Follow-ups
4. Be specific and factual; do not invent behavior not shown in the input.
5. Mention probable intent behind this push in one sentence.

Repository overview:
{project_overview}

Commit messages:
{commits_text}

Changed files summary:
{changed_files}

Diff excerpt:
{diff_patch}

Truncated and compressed CI log digest:
{logs_digest}
""".strip()

    summary_text = ""
    selected_model = requested_model or DEFAULT_MODEL
    if groq_api_key:
        try:
            selected_model = choose_model(groq_api_key, requested_model)
            openai_module = __import__("openai")
            client = openai_module.OpenAI(
                api_key=groq_api_key,
                base_url="https://api.groq.com/openai/v1",
            )
            response = client.responses.create(input=prompt, model=selected_model)
            summary_text = (response.output_text or "").strip()
        except Exception as exc:
            summary_text = (
                "### Push Summary (Fallback)\n"
                f"- AI summarization failed: {type(exc).__name__}.\n"
                f"- Requested/selected model: `{selected_model}`.\n"
            )

    if not summary_text:
        summary_text = build_fallback_summary(commits_text, changed_files)

    summary_with_meta = f"_Model used: `{selected_model}`_\n\n{summary_text}".strip()

    Path("ai_push_summary.txt").write_text(summary_with_meta + "\n", encoding="utf-8")
    Path("ai_push_summary.json").write_text(
        json.dumps({"summary": summary_text, "model": selected_model}, ensure_ascii=False),
        encoding="utf-8",
    )

    if step_summary_path:
        with open(step_summary_path, "a", encoding="utf-8") as fh:
            fh.write("\n## AI Push Summary\n\n")
            fh.write(summary_with_meta)
            fh.write("\n")


if __name__ == "__main__":
    main()
