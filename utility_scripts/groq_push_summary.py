#!/usr/bin/env python3

import json
import os
import re
import subprocess
import urllib.request
from pathlib import Path


MAX_DIFF_FILES_CHARS = 1600
MAX_DIFF_PATCH_CHARS = 3200
MAX_COMMITS = 20
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


def parse_changed_paths(changed_files: str) -> list[str]:
    paths = []
    for line in changed_files.splitlines():
        raw = line.strip()
        if not raw:
            continue
        parts = raw.split("\t")
        if len(parts) >= 2:
            candidate = parts[-1].strip()
        else:
            candidate = raw.split()[-1]
        if candidate:
            paths.append(candidate)
    return paths


def classify_path(path: str) -> str:
    p = path.strip().lower()
    if p.startswith("src/") or p.startswith("include/"):
        return "core"
    if p.startswith("tests/"):
        return "tests"
    if p.startswith(".github/workflows/"):
        return "workflow"
    if p.startswith("utility_scripts/"):
        return "scripts"
    if p.startswith("documentation/") or p.startswith("docs/") or p.endswith("doxygenconfig"):
        return "docs"
    if p.startswith("dist/"):
        return "dist"
    return "other"


def infer_primary_focus(changed_paths: list[str]) -> str:
    if not changed_paths:
        return "unknown"

    weights = {
        "core": 5,
        "tests": 4,
        "scripts": 3,
        "workflow": 2,
        "docs": 1,
        "dist": 1,
        "other": 1,
    }
    scores = {k: 0 for k in weights}
    for path in changed_paths:
        scores[classify_path(path)] += weights[classify_path(path)]

    return max(scores, key=lambda k: scores[k])


def summarize_focus_label(focus: str) -> str:
    mapping = {
        "core": "core emulator logic",
        "tests": "test coverage and verification",
        "scripts": "developer/automation scripts",
        "workflow": "CI workflow behavior",
        "docs": "documentation",
        "dist": "prebuilt/reference assets",
        "other": "project configuration",
        "unknown": "repository updates",
    }
    return mapping.get(focus, "repository updates")


def build_fallback_summary(commits_text: str, changed_files: str, changed_paths: list[str]) -> str:
    focus = infer_primary_focus(changed_paths)
    focus_label = summarize_focus_label(focus)
    top_paths = "\n".join(f"- `{p}`" for p in changed_paths[:8]) or "- No changed files detected"

    lines = [
        "Push Summary",
        f"This push primarily updates {focus_label}.",
        "",
        "Key Changes",
        commits_text or "- No commit messages were detected in the push payload.",
        "",
        "Risks / Follow-ups",
        "- Review changed files and test impact for this push.",
        "- Verify CI output if behavior changes are expected.",
        "",
        "Intent",
        f"Likely intent: improve {focus_label} while keeping the branch synchronized.",
        "",
        "Changed Files (Top)",
        top_paths,
    ]
    return "\n".join(lines)


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


def call_model(api_key: str, model: str, prompt: str) -> str:
    openai_module = __import__("openai")
    client = openai_module.OpenAI(
        api_key=api_key,
        base_url="https://api.groq.com/openai/v1",
    )
    response = client.responses.create(input=prompt, model=model)
    return (response.output_text or "").strip()


def build_prompt(
    repo: str,
    ref_name: str,
    commits_text: str,
    changed_files: str,
    diff_patch: str,
    focus: str,
    strict_retry: bool,
) -> str:
    strict_clause = ""
    if strict_retry:
        strict_clause = (
            "IMPORTANT RETRY MODE: Your previous draft referenced areas not grounded in changed files. "
            "In this retry, keep claims tightly scoped to changed files and diff."
        )

    return f"""
You are an engineering release assistant for repository `{repo}` on branch `{ref_name}`.

Task: produce a concise push summary grounded in the provided commit messages and git diff context.

Grounding rules (strict-with-light-inference):
1. You may infer high-level intent, but every concrete claim must be supported by changed files or diff.
2. Do NOT mention components/files that are not present in Changed files summary or Diff excerpt.
3. If `.github/workflows/` is absent from changed files, do not discuss CI/workflow changes.
4. Prioritize the likely primary focus of this push: `{summarize_focus_label(focus)}`.
5. If context is insufficient for specifics, say so briefly instead of guessing.

Output format:
- Use Markdown without code fences.
- 4 sections exactly, with these headings in order:
  - Push Summary
  - Key Changes
  - Risks / Follow-ups
  - Intent
- Keep total length around 90-170 words.

{strict_clause}

Commit messages:
{commits_text}

Changed files summary:
{changed_files}

Diff excerpt:
{diff_patch}
""".strip()


def has_ungrounded_workflow_reference(summary_text: str, changed_paths: list[str]) -> bool:
    has_workflow_changes = any(p.lower().startswith(".github/workflows/") for p in changed_paths)
    if has_workflow_changes:
        return False

    lower = summary_text.lower()
    suspicious_tokens = [
        "workflow",
        "github actions",
        "discord notification",
        "static.yml",
        "test-init-response-protocol.yml",
        "deploy-*",
        "pages deployment",
    ]
    return any(token in lower for token in suspicious_tokens)


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

    commit_lines = []
    for commit in commits[:MAX_COMMITS]:
        cid = (commit.get("id") or "")[:7]
        message = (commit.get("message") or "").splitlines()[0].strip()
        author = (commit.get("author") or {}).get("name", "unknown")
        commit_lines.append(f"- {cid} {message} ({author})")

    if len(commits) > MAX_COMMITS:
        commit_lines.append(f"- ... and {len(commits) - MAX_COMMITS} more commit(s)")

    commits_text = "\n".join(commit_lines)

    changed_files = ""
    diff_patch = ""

    if before and after and before != "0" * 40:
        changed_files = run_git(["diff", "--name-status", before, after])
        diff_patch = run_git(["diff", "--unified=1", "--no-color", before, after])

    if not changed_files:
        changed_files = run_git(["show", "--name-status", "--pretty=format:", "HEAD"])
    if not diff_patch:
        diff_patch = run_git(["show", "--unified=1", "--no-color", "--pretty=format:", "HEAD"])

    changed_files = trim(changed_files, MAX_DIFF_FILES_CHARS)
    diff_patch = trim(diff_patch, MAX_DIFF_PATCH_CHARS)

    changed_paths = parse_changed_paths(changed_files)
    focus = infer_primary_focus(changed_paths)

    selected_model = requested_model or DEFAULT_MODEL
    summary_text = ""

    if groq_api_key:
        try:
            selected_model = choose_model(groq_api_key, requested_model)
            prompt = build_prompt(
                repo=repo,
                ref_name=ref_name,
                commits_text=commits_text,
                changed_files=changed_files,
                diff_patch=diff_patch,
                focus=focus,
                strict_retry=False,
            )
            summary_text = call_model(groq_api_key, selected_model, prompt)

            if has_ungrounded_workflow_reference(summary_text, changed_paths):
                retry_prompt = build_prompt(
                    repo=repo,
                    ref_name=ref_name,
                    commits_text=commits_text,
                    changed_files=changed_files,
                    diff_patch=diff_patch,
                    focus=focus,
                    strict_retry=True,
                )
                summary_text = call_model(groq_api_key, selected_model, retry_prompt)
        except Exception as exc:
            summary_text = (
                "Push Summary\n"
                f"AI summarization failed ({type(exc).__name__}); using fallback summary."
            )

    if (not summary_text) or has_ungrounded_workflow_reference(summary_text, changed_paths):
        summary_text = build_fallback_summary(commits_text, changed_files, changed_paths)

    summary_with_meta = f"_Model used: `{selected_model}`_\n\n{summary_text}".strip()

    Path("ai_push_summary.txt").write_text(summary_with_meta + "\n", encoding="utf-8")
    Path("ai_push_summary.json").write_text(
        json.dumps(
            {
                "summary": summary_text,
                "model": selected_model,
                "focus": focus,
                "changed_paths": changed_paths,
            },
            ensure_ascii=False,
            indent=2,
        ),
        encoding="utf-8",
    )

    if step_summary_path:
        with open(step_summary_path, "a", encoding="utf-8") as fh:
            fh.write("\n## AI Push Summary\n\n")
            fh.write(summary_with_meta)
            fh.write("\n")


if __name__ == "__main__":
    main()
