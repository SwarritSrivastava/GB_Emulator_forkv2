# Discord Bot + Cloudflare Worker + GitHub Actions Setup

---

## Overview

This document covers the complete setup for a serverless Discord bot that:
- Receives slash commands via Cloudflare Workers
- Triggers GitHub Actions workflows via the GitHub API
- Reads issues and pull requests from your repository

The system has three layers:

```
Discord Slash Command
        |
Cloudflare Worker  (receives, verifies, routes)
        |
GitHub REST API    (triggers workflows, reads data)
        |
GitHub Actions     (runs CI, deploys pages)
        |
Discord Webhook    (sends results back)
```

---

## Part 1 — Discord Application Setup

### 1.1 Create the Application

1. Go to https://discord.com/developers/applications
2. Click **New Application**, give it a name, and confirm.
3. On the left sidebar, go to **General Information**.
4. Copy and save:
   - **Application ID** (you will need this for registering commands)
   - **Public Key** (you will store this in Cloudflare as `DISCORD_PUBLIC_KEY`)

### 1.2 Create a Bot User

1. On the left sidebar, go to **Bot**.
2. Click **Add Bot** and confirm.
3. Under **Token**, click **Reset Token**, then copy and save it. This is your bot token used for registering commands, not for the Worker itself.
4. Under **Privileged Gateway Intents**, you do not need to enable anything for slash commands.

### 1.3 Invite the Bot to Your Server

1. On the left sidebar, go to **OAuth2 > URL Generator**.
2. Under **Scopes**, select:
   - `bot`
   - `applications.commands`
3. Under **Bot Permissions**, select:
   - `Send Messages`
4. Copy the generated URL and open it in a browser, then select your server.

### 1.4 Register Slash Commands

You register slash commands by sending a POST request to the Discord API. You can do this with `curl` or any HTTP client.

Replace `YOUR_APP_ID` and `YOUR_BOT_TOKEN` in the commands below.

**Register all commands at once using a bulk overwrite:**

```bash
curl -X PUT \
  "https://discord.com/api/v10/applications/YOUR_APP_ID/commands" \
  -H "Authorization: Bot YOUR_BOT_TOKEN" \
  -H "Content-Type: application/json" \
  -d '[
    {
      "name": "test",
      "description": "Trigger the CI test workflow"
    },
    {
      "name": "deploy",
      "description": "Deploy to GitHub Pages"
    },
    {
      "name": "list-issues",
      "description": "List open issues in the repository"
    },
    {
      "name": "issue",
      "description": "Get details for a specific issue",
      "options": [
        {
          "name": "id",
          "type": 4,
          "description": "Issue number",
          "required": true
        }
      ]
    },
    {
      "name": "list-pr",
      "description": "List open pull requests"
    },
    {
      "name": "pr",
      "description": "Get details for a specific pull request",
      "options": [
        {
          "name": "id",
          "type": 4,
          "description": "Pull request number",
          "required": true
        }
      ]
    }
  ]'
```

Note: Global commands take up to one hour to propagate. To register commands only in a specific server for immediate testing, replace the URL with:
`https://discord.com/api/v10/applications/YOUR_APP_ID/guilds/YOUR_GUILD_ID/commands`

Your guild ID is the server ID. Enable Developer Mode in Discord settings, then right-click your server name and select **Copy Server ID**.

---

## Part 2 — GitHub Token Setup

### 2.1 Create a Fine-Grained Personal Access Token

1. Go to https://github.com/settings/tokens?type=beta
2. Click **Generate new token**.
3. Set:
   - **Token name**: something descriptive (e.g. `discord-worker-bot`)
   - **Expiration**: 90 days or as needed
   - **Resource owner**: your account
   - **Repository access**: select only `Jayesh-Dev21/GB_Emulator`

4. Under **Repository permissions**, set:

| Permission     | Level          |
|----------------|----------------|
| Actions        | Read and write |
| Issues         | Read-only      |
| Pull requests  | Read-only      |
| Metadata       | Read-only (mandatory, auto-selected) |

5. Click **Generate token** and copy it immediately. You will not be able to see it again.

### 2.2 Encode the Token with ROT13

ROT13 is a simple character substitution that shifts each letter by 13 positions. It is not encryption — it is obfuscation. Its purpose here is to prevent accidental plain-text exposure in logs or screenshots.

Use this command to ROT13-encode your token:

```bash
echo "YOUR_GITHUB_TOKEN" | tr 'A-Za-z' 'N-ZA-Mn-za-m'
```

Do the same for your Discord public key:

```bash
echo "YOUR_DISCORD_PUBLIC_KEY" | tr 'A-Za-z' 'N-ZA-Mn-za-m'
```

Save both encoded values. These are what you will store in Cloudflare.

---

## Part 3 — Cloudflare Worker Setup

### 3.1 Create the Worker

1. Go to https://dash.cloudflare.com
2. On the left sidebar, go to **Workers & Pages**.
3. Click **Create application**, then **Create Worker**.
4. Give it a name (e.g. `discord-github-bot`) and click **Deploy**.
5. Click **Edit code** and replace the default script with the code in **Part 4** below.
6. Click **Deploy**.

### 3.2 Set Environment Variables

1. In your Worker, go to **Settings > Variables**.
2. Under **Environment Variables**, add the following. Use **Add variable** for each:

| Variable name          | Value                                      |
|------------------------|--------------------------------------------|
| `DISCORD_PUBLIC_KEY`   | Your ROT13-encoded Discord public key      |
| `GITHUB_TOKEN`         | Your ROT13-encoded GitHub token            |
| `GITHUB_REPO`          | `Jayesh-Dev21/GB_Emulator`                 |

3. Click **Save and deploy**.

### 3.3 Set the Interactions Endpoint in Discord

1. Go back to your Discord application at https://discord.com/developers/applications
2. Go to **General Information**.
3. Under **Interactions Endpoint URL**, paste your Worker URL:
   `https://your-worker-name.your-subdomain.workers.dev`
4. Click **Save Changes**. Discord will send a ping request to verify the endpoint. The Worker must respond correctly to the ping for this to succeed. If your Worker is deployed with the code below, this will pass automatically.

---

## Part 4 — Cloudflare Worker Code

Save this as your Worker script:

```javascript
export default {
  async fetch(request, env, ctx) {
    if (request.method !== "POST") {
      return new Response("Method not allowed", { status: 405 });
    }

    const signature = request.headers.get("x-signature-ed25519");
    const timestamp = request.headers.get("x-signature-timestamp");
    const body = await request.text();

    if (!signature || !timestamp) {
      return new Response("Missing signature headers", { status: 401 });
    }

    const publicKey = rot13(env.DISCORD_PUBLIC_KEY);

    const isValid = await verifyDiscordRequest(body, signature, timestamp, publicKey);
    if (!isValid) {
      return new Response("Invalid request signature", { status: 401 });
    }

    const json = JSON.parse(body);

    // Discord verification ping
    if (json.type === 1) {
      return jsonResponse({ type: 1 });
    }

    if (json.type !== 2) {
      return new Response("Unsupported interaction type", { status: 400 });
    }

    const command = json.data.name;

    switch (command) {
      case "test":
        ctx.waitUntil(triggerGitHub(env, json, "run-test"));
        return reply("CI test workflow triggered.");

      case "deploy":
        ctx.waitUntil(triggerGitHub(env, json, "deploy-pages"));
        return reply("Deploy to GitHub Pages triggered.");

      case "list-issues":
        return reply(await listIssues(env));

      case "issue":
        return reply(await getIssue(env, json));

      case "list-pr":
        return reply(await listPRs(env));

      case "pr":
        return reply(await getPR(env, json));

      default:
        return reply("Unknown command: " + command);
    }
  }
};

// Trigger a GitHub repository_dispatch event
async function triggerGitHub(env, interaction, eventType) {
  const token = rot13(env.GITHUB_TOKEN);
  const user = interaction.member?.user?.username
    ?? interaction.user?.username
    ?? "unknown";

  const res = await fetch(
    `https://api.github.com/repos/${env.GITHUB_REPO}/dispatches`,
    {
      method: "POST",
      headers: {
        Authorization: `Bearer ${token}`,
        "Content-Type": "application/json",
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      },
      body: JSON.stringify({
        event_type: eventType,
        client_payload: {
          triggered_by: "discord",
          user: user,
          command: interaction.data.name
        }
      })
    }
  );

  if (!res.ok) {
    const text = await res.text();
    console.error(`GitHub dispatch failed: ${res.status} ${text}`);
  }
}

// List open issues
async function listIssues(env) {
  const token = rot13(env.GITHUB_TOKEN);

  const res = await fetch(
    `https://api.github.com/repos/${env.GITHUB_REPO}/issues?state=open&per_page=10`,
    {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    }
  );

  if (!res.ok) {
    return `Failed to fetch issues: ${res.status}`;
  }

  const issues = await res.json();

  // GitHub returns PRs in the issues endpoint; filter them out
  const filtered = issues.filter(i => !i.pull_request);

  if (!filtered.length) {
    return "No open issues found.";
  }

  return filtered
    .map(i => `#${i.number} [${i.state}] ${i.title}`)
    .join("\n");
}

// Get a single issue with its comments
async function getIssue(env, json) {
  const id = json.data.options?.[0]?.value;
  if (!id) return "No issue ID provided.";

  const token = rot13(env.GITHUB_TOKEN);

  const [issueRes, commentsRes] = await Promise.all([
    fetch(`https://api.github.com/repos/${env.GITHUB_REPO}/issues/${id}`, {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    }),
    fetch(`https://api.github.com/repos/${env.GITHUB_REPO}/issues/${id}/comments?per_page=5`, {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    })
  ]);

  if (!issueRes.ok) {
    return `Issue #${id} not found (${issueRes.status}).`;
  }

  const issue = await issueRes.json();
  const comments = commentsRes.ok ? await commentsRes.json() : [];

  const labels = issue.labels?.map(l => l.name).join(", ") || "none";
  const body = (issue.body || "No description.").slice(0, 400);

  let msg = `Issue #${issue.number} — ${issue.title}\n`;
  msg += `State: ${issue.state} | Author: ${issue.user?.login} | Labels: ${labels}\n\n`;
  msg += body;

  if (body.length === 400) msg += "...";

  if (comments.length) {
    msg += `\n\nComments (${comments.length} shown):\n`;
    comments.slice(0, 3).forEach(c => {
      const excerpt = (c.body || "").slice(0, 120);
      msg += `\n${c.user.login}: ${excerpt}`;
      if (c.body?.length > 120) msg += "...";
    });
  }

  return msg.slice(0, 1900);
}

// List open pull requests
async function listPRs(env) {
  const token = rot13(env.GITHUB_TOKEN);

  const res = await fetch(
    `https://api.github.com/repos/${env.GITHUB_REPO}/pulls?state=open&per_page=10`,
    {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    }
  );

  if (!res.ok) {
    return `Failed to fetch pull requests: ${res.status}`;
  }

  const prs = await res.json();

  if (!prs.length) {
    return "No open pull requests found.";
  }

  return prs
    .map(pr => `#${pr.number} [${pr.state}] ${pr.title} (${pr.head.ref} -> ${pr.base.ref})`)
    .join("\n");
}

// Get a single pull request with comments
async function getPR(env, json) {
  const id = json.data.options?.[0]?.value;
  if (!id) return "No PR ID provided.";

  const token = rot13(env.GITHUB_TOKEN);

  const [prRes, commentsRes] = await Promise.all([
    fetch(`https://api.github.com/repos/${env.GITHUB_REPO}/pulls/${id}`, {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    }),
    fetch(`https://api.github.com/repos/${env.GITHUB_REPO}/issues/${id}/comments?per_page=5`, {
      headers: {
        Authorization: `Bearer ${token}`,
        Accept: "application/vnd.github+json",
        "User-Agent": "discord-worker-bot"
      }
    })
  ]);

  if (!prRes.ok) {
    return `PR #${id} not found (${prRes.status}).`;
  }

  const pr = await prRes.json();
  const comments = commentsRes.ok ? await commentsRes.json() : [];

  const labels = pr.labels?.map(l => l.name).join(", ") || "none";
  const body = (pr.body || "No description.").slice(0, 400);
  const mergeable = pr.mergeable === null ? "unknown" : pr.mergeable ? "yes" : "no";

  let msg = `PR #${pr.number} — ${pr.title}\n`;
  msg += `State: ${pr.state} | ${pr.head.ref} -> ${pr.base.ref}\n`;
  msg += `Author: ${pr.user?.login} | Labels: ${labels} | Mergeable: ${mergeable}\n`;
  msg += `Commits: ${pr.commits} | Additions: +${pr.additions} | Deletions: -${pr.deletions}\n\n`;
  msg += body;

  if (body.length === 400) msg += "...";

  if (comments.length) {
    msg += `\n\nComments (${comments.length} shown):\n`;
    comments.slice(0, 3).forEach(c => {
      const excerpt = (c.body || "").slice(0, 120);
      msg += `\n${c.user.login}: ${excerpt}`;
      if (c.body?.length > 120) msg += "...";
    });
  }

  return msg.slice(0, 1900);
}

// ROT13 decode: reverses ROT13-encoded strings
function rot13(str) {
  if (!str) return "";
  return str.replace(/[a-zA-Z]/g, c => {
    const base = c <= "Z" ? 65 : 97;
    return String.fromCharCode((c.charCodeAt(0) - base + 13) % 26 + base);
  });
}

// Verify Discord Ed25519 signature
async function verifyDiscordRequest(body, signature, timestamp, publicKey) {
  try {
    const encoder = new TextEncoder();
    const data = encoder.encode(timestamp + body);

    const key = await crypto.subtle.importKey(
      "raw",
      hexToUint8Array(publicKey),
      { name: "Ed25519" },
      false,
      ["verify"]
    );

    return await crypto.subtle.verify("Ed25519", key, hexToUint8Array(signature), data);
  } catch {
    return false;
  }
}

function hexToUint8Array(hex) {
  return new Uint8Array(hex.match(/.{1,2}/g).map(b => parseInt(b, 16)));
}

function reply(content) {
  return jsonResponse({ type: 4, data: { content: String(content) } });
}

function jsonResponse(obj) {
  return new Response(JSON.stringify(obj), {
    headers: { "Content-Type": "application/json" }
  });
}
```

---

## Part 5 — Updated GitHub Workflows

### 5.1 Test Workflow

Replace your existing `.github/workflows/test-init-response-protocol.yml` with the following. The only changes from your original are the added `workflow_dispatch` and `repository_dispatch` triggers, plus a step that prints the trigger context.

```yaml
name: TEST INIT RESPONSE PROTOCOL

on:
  push:
  pull_request:
    branches: ["main", "master"]

  workflow_dispatch:
    inputs:
      trigger_source:
        description: "Trigger source (discord, manual, etc.)"
        required: false
        default: "manual"

  repository_dispatch:
    types: [run-test]

permissions:
  contents: read

concurrency:
  group: test-init-response-${{ github.ref }}
  cancel-in-progress: true

jobs:
  test-init-response:
    name: Build, Test, Valgrind, Discord Notify
    runs-on: ubuntu-latest
    timeout-minutes: 60

    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          persist-credentials: false
          fetch-depth: 0

      - name: Print trigger context
        run: |
          echo "Event: ${{ github.event_name }}"
          echo "Trigger source: ${{ github.event.inputs.trigger_source }}"
          echo "Client payload: ${{ toJson(github.event.client_payload) }}"

      - name: Build and run tests
        id: run_build_test
        run: |
          set +e
          chmod +x ./utility_scripts/build_and_test.sh
          ./utility_scripts/build_and_test.sh > build_and_test.log 2>&1
          code=$?
          echo "exit_code=$code" >> "$GITHUB_OUTPUT"
          exit 0

      - name: Run valgrind test protocol
        id: run_valgrind
        if: always()
        run: |
          set +e
          chmod +x ./utility_scripts/valgrind_tests.sh
          sudo ./utility_scripts/valgrind_tests.sh > valgrind_tests.log 2>&1
          code=$?
          echo "exit_code=$code" >> "$GITHUB_OUTPUT"
          exit 0

      - name: Generate AI push summary
        if: always() && github.event_name == 'push'
        env:
          GROQ_API_KEY: ${{ secrets.API }}
          GROQ_MODEL: openai/gpt-oss-20b
          EVENT_PATH: ${{ github.event_path }}
          REPO: ${{ github.repository }}
          REF_NAME: ${{ github.ref_name }}
        run: |
          python3 -m pip install --upgrade pip
          python3 -m pip install openai
          chmod +x ./utility_scripts/groq_push_summary.py
          python3 ./utility_scripts/groq_push_summary.py

      - name: Ensure AI summary file exists
        if: always()
        run: |
          [ -f ai_push_summary.txt ] || echo "AI summary was not generated" > ai_push_summary.txt

      - name: Prepare Discord payloads
        if: always()
        env:
          JOB_STATUS: ${{ job.status }}
          RUN_ID: ${{ github.run_id }}
          RUN_NUMBER: ${{ github.run_number }}
          REPO: ${{ github.repository }}
          REF_NAME: ${{ github.ref_name }}
          SHA: ${{ github.sha }}
          ACTOR: ${{ github.actor }}
          BUILD_TEST_EXIT: ${{ steps.run_build_test.outputs.exit_code }}
          VALGRIND_EXIT: ${{ steps.run_valgrind.outputs.exit_code }}
          EVENT_PATH: ${{ github.event_path }}
          SERVER_URL: ${{ github.server_url }}
        run: |
          python3 - <<'PY'
          import json
          import os
          import re
          from pathlib import Path

          def read_text(path, default=""):
              try:
                  with open(path, "r", encoding="utf-8", errors="replace") as fh:
                      return fh.read()
              except FileNotFoundError:
                  return default

          def first_match(pattern, text, flags=0):
              m = re.search(pattern, text, flags)
              return m.group(1) if m else ""

          def parse_ctest_stats(log_text):
              total = first_match(r"100% tests passed, (\d+) tests passed out of (\d+)", log_text)
              if total:
                  passed, total_count = re.search(r"100% tests passed, (\d+) tests passed out of (\d+)", log_text).groups()
                  return int(total_count), int(passed), 0

              m = re.search(r"(\d+)% tests passed, (\d+) tests failed out of (\d+)", log_text)
              if m:
                  failed = int(m.group(2))
                  total_count = int(m.group(3))
                  passed = total_count - failed
                  return total_count, passed, failed

              m = re.search(r"Total Tests:\s*(\d+)", log_text)
              if m:
                  total_count = int(m.group(1))
                  failed_list = len(re.findall(r"^\s*\d+\s*-\s+.*\(Failed\)", log_text, re.MULTILINE))
                  passed = max(total_count - failed_list, 0)
                  return total_count, passed, failed_list

              return 0, 0, 0

          def extract_failure_block(log_text, max_lines=25):
              lines = [line.rstrip() for line in log_text.splitlines()]
              if not lines:
                  return ["No log output captured."]

              failure_markers = [
                  "The following tests FAILED:",
                  "***Failed",
                  "FAILED",
                  "Error",
                  "error:",
              ]

              start = -1
              for i, line in enumerate(lines):
                  if any(marker in line for marker in failure_markers):
                      start = i
                      break

              if start == -1:
                  tail = [x for x in lines if x.strip()][-max_lines:]
                  return tail if tail else ["Failure detected but no details were found."]

              block = []
              for line in lines[start:]:
                  block.append(line)
                  if len(block) >= max_lines:
                      break
              return block

          def parse_valgrind_status(log_text):
              if re.search(r"ERROR SUMMARY:\s*0\s+errors", log_text):
                  return "PASS"
              if re.search(r"ERROR SUMMARY:\s*[1-9]\d*\s+errors", log_text):
                  return "FAIL"
              return "UNKNOWN"

          def trim_text(text, max_len=1800):
              if len(text) <= max_len:
                  return text
              return text[: max_len - 18] + "\n...[truncated]"

          def split_text(text, max_len=1800):
              if not text:
                  return []
              lines = text.splitlines()
              chunks = []
              current = []
              current_len = 0

              for line in lines:
                  add = len(line) + 1
                  if current and current_len + add > max_len:
                      chunks.append("\n".join(current))
                      current = [line]
                      current_len = add
                  else:
                      current.append(line)
                      current_len += add

              if current:
                  chunks.append("\n".join(current))

              return chunks

          def clean_markdown_line(line):
              s = line.strip()
              if not s:
                  return ""
              s = re.sub(r"^\*\*(.+)\*\*$", r"\1", s)
              s = re.sub(r"^_(.+)_$", r"\1", s)
              s = s.replace("\u2011", "-")
              s = re.sub(r"\s+", " ", s).strip()
              if s.startswith("* "):
                  s = "- " + s[2:]
              return s

          def classify_heading(line):
              raw = line.strip()
              if not raw:
                  return ""
              heading = re.sub(r"^[#\s]+", "", raw)
              heading = re.sub(r"^\*\*(.+?)\*\*$", r"\1", heading)
              heading = heading.replace("*", "").replace("`", "")
              lower = heading.lower().strip(" :")

              if "push summary" in lower:
                  return "Push Summary"
              if "key code changes" in lower or "key changes" in lower:
                  return "Key Changes"
              if "potential risks" in lower or "follow-up" in lower or "follow ups" in lower:
                  return "Risks / Follow-ups"
              if "probable intent" in lower or lower == "intent":
                  return "Intent"
              return ""

          def normalize_ai_summary(raw_text):
              text = (raw_text or "").replace("\r\n", "\n").replace("\r", "\n")
              lines = []
              in_fence = False
              for line in text.splitlines():
                  stripped = line.strip()
                  if stripped.startswith("```"):
                      in_fence = not in_fence
                      continue
                  if in_fence:
                      lines.append(line)
                      continue
                  lines.append(line)

              sections = {
                  "Push Summary": [],
                  "Key Changes": [],
                  "Risks / Follow-ups": [],
                  "Intent": [],
              }

              current = "Push Summary"
              for raw in lines:
                  line = clean_markdown_line(raw)
                  if not line:
                      if sections[current] and sections[current][-1] != "":
                          sections[current].append("")
                      continue

                  if line.lower().startswith("model used:"):
                      continue

                  mapped = classify_heading(line)
                  if mapped:
                      current = mapped
                      inline = re.split(r":", line, maxsplit=1)
                      if len(inline) == 2 and inline[1].strip():
                          sections[current].append(clean_markdown_line(inline[1]))
                      continue

                  probable_intent = re.match(r"^\*?probable intent\*?\s*:\s*(.+)$", line, re.IGNORECASE)
                  if probable_intent:
                      current = "Intent"
                      sections[current].append(clean_markdown_line(probable_intent.group(1)))
                      continue

                  sections[current].append(line)

              def compact(lines_in):
                  out = []
                  for item in lines_in:
                      if item == "":
                          if out and out[-1] != "":
                              out.append("")
                          continue
                      out.append(item)
                  while out and out[-1] == "":
                      out.pop()
                  return out

              sections = {k: compact(v) for k, v in sections.items()}

              for key in ("Key Changes", "Risks / Follow-ups"):
                  normalized = []
                  for item in sections[key]:
                      if not item:
                          continue
                      if item.startswith("- "):
                          normalized.append(item)
                      else:
                          normalized.append(f"- {item}")
                  sections[key] = normalized

              output = []
              order = ["Push Summary", "Key Changes", "Risks / Follow-ups", "Intent"]
              for title in order:
                  content = sections[title]
                  if not content:
                      continue
                  if output:
                      output.append("")
                  output.append(f"**{title}**")
                  output.extend(content)

              return "\n".join(output).strip()

          event_path = os.environ["EVENT_PATH"]
          with open(event_path, "r", encoding="utf-8") as fh:
              event = json.load(fh)

          commits = event.get("commits", [])
          commit_lines = []
          for c in commits[:10]:
              cid = (c.get("id") or "")[:7]
              msg = ((c.get("message") or "").splitlines() or [""])[0].strip()
              author = (c.get("author") or {}).get("name", "unknown")
              commit_lines.append(f"- `{cid}` {msg} ({author})")

          if len(commits) > 10:
              commit_lines.append(f"- ... and {len(commits) - 10} more commits")

          status = os.environ["JOB_STATUS"].upper()
          emoji = ":white_check_mark:" if status == "SUCCESS" else ":x:"
          sha_full = os.environ["SHA"]
          sha_short = sha_full[:7]

          repo = os.environ["REPO"]
          ref_name = os.environ["REF_NAME"]
          actor = os.environ["ACTOR"]
          run_id = os.environ["RUN_ID"]
          run_number = os.environ["RUN_NUMBER"]
          server_url = os.environ["SERVER_URL"]
          build_test_exit = os.environ.get("BUILD_TEST_EXIT", "1")
          valgrind_exit = os.environ.get("VALGRIND_EXIT", "1")

          run_url = f"{server_url}/{repo}/actions/runs/{run_id}"
          commit_url = f"{server_url}/{repo}/commit/{sha_full}"
          compare_url = event.get("compare", "")

          build_log = read_text("build_and_test.log", "")
          valgrind_log = read_text("valgrind_tests.log", "")
          ai_summary = read_text("ai_push_summary.txt", "AI summary unavailable").strip()

          total_tests, passed_tests, failed_tests = parse_ctest_stats(build_log)
          tests_status = "PASS" if build_test_exit == "0" else "FAIL"
          vg_status = "PASS" if valgrind_exit == "0" else "FAIL"
          parsed_vg = parse_valgrind_status(valgrind_log)
          if vg_status == "PASS" and parsed_vg == "FAIL":
              vg_status = "FAIL"

          summary_lines = [
              f"{emoji} **TEST INIT RESPONSE PROTOCOL** `{status}`",
              "",
              f"Repository: `{repo}`",
              f"Branch: `{ref_name}`",
              f"Actor: `{actor}`",
              f"Head Commit: [`{sha_short}`]({commit_url})",
              f"Run: [#{run_number}]({run_url})",
          ]

          if compare_url:
              summary_lines.append(f"Compare: {compare_url}")

          summary_lines.extend(
              [
                  "",
                  "**Checks**",
                  f"- Tests: `{tests_status}` | total=`{total_tests}` passed=`{passed_tests}` failed=`{failed_tests}`",
                  f"- Valgrind: `{vg_status}`",
                  f"- Commit Count In Push: `{len(commits)}`",
              ]
          )

          if commit_lines:
              summary_lines.append("")
              summary_lines.append("Commits:")
              summary_lines.extend(commit_lines)

          if tests_status == "FAIL":
              summary_lines.append("")
              summary_lines.append("**Test Failure Output**")
              summary_lines.append("```")
              summary_lines.extend(extract_failure_block(build_log, max_lines=30))
              summary_lines.append("```")

          if vg_status == "FAIL":
              summary_lines.append("")
              summary_lines.append("**Valgrind Failure Output**")
              summary_lines.append("```")
              summary_lines.extend(extract_failure_block(valgrind_log, max_lines=30))
              summary_lines.append("```")

          primary_content = trim_text("\n".join(summary_lines), 1800)

          Path("discord_ai_payloads").mkdir(exist_ok=True)

          with open("discord_payload_main.json", "w", encoding="utf-8") as out:
              json.dump({"content": primary_content}, out)

          ai_text = normalize_ai_summary(ai_summary)
          if not ai_text:
              ai_text = "**Push Summary**\nAI summary unavailable"
          ai_chunks = split_text(ai_text, 1600)
          if not ai_chunks:
              ai_chunks = ["**Push Summary**\nAI summary unavailable"]

          payload_files = []
          total = len(ai_chunks)
          for i, chunk in enumerate(ai_chunks, start=1):
              header = f"**AI Commit Summary ({i}/{total})**"
              content = "\n\n".join([header, chunk])
              payload = {"content": trim_text(content, 1800)}
              path = Path("discord_ai_payloads") / f"ai_{i:02d}.json"
              path.write_text(json.dumps(payload), encoding="utf-8")
              payload_files.append(path.as_posix())

          Path("discord_ai_payloads.txt").write_text("\n".join(payload_files) + "\n", encoding="utf-8")
          PY

      - name: Validate Discord webhook
        id: webhook_check
        if: always() && github.event_name == 'push'
        env:
          WEBHOOK: ${{ secrets.WEBHOOK }}
        run: |
          rm -f .discord_webhook_blocked

          if [ -z "$WEBHOOK" ]; then
            echo "WEBHOOK secret is not set; skipping Discord notification."
            echo "should_send=false" >> "$GITHUB_OUTPUT"
            exit 0
          fi

          case "$WEBHOOK" in
            https://discord.com/api/webhooks/*|https://ptb.discord.com/api/webhooks/*)
              echo "should_send=true" >> "$GITHUB_OUTPUT"
              ;;
            *)
              echo "WEBHOOK does not look like a Discord webhook URL; skipping Discord notification."
              echo "should_send=false" >> "$GITHUB_OUTPUT"
              exit 0
              ;;
          esac

      - name: Send Discord notification messages
        if: always() && github.event_name == 'push' && steps.webhook_check.outputs.should_send == 'true'
        env:
          WEBHOOK: ${{ secrets.WEBHOOK }}
        run: |
          curl --fail --silent --show-error \
            -H "Content-Type: application/json" \
            -X POST \
            --data @discord_payload_main.json \
            "$WEBHOOK"

          if [ -f discord_ai_payloads.txt ]; then
            while IFS= read -r payload_file; do
              [ -n "$payload_file" ] || continue
              sleep 5
              curl --fail --silent --show-error \
                -H "Content-Type: application/json" \
                -X POST \
                --data @"$payload_file" \
                "$WEBHOOK"
            done < discord_ai_payloads.txt
          fi

      - name: Mark workflow failed if checks failed
        if: always()
        run: |
          if [ "${{ steps.run_build_test.outputs.exit_code }}" != "0" ] || \
             [ "${{ steps.run_valgrind.outputs.exit_code }}" != "0" ]; then
            echo "One or more test protocols failed"
            exit 1
          fi
```

---

### 5.2 Deploy Workflow

Replace your existing `.github/workflows/deploy.yml` (or whatever it is named) with the following. The only change from your original is the added `repository_dispatch` trigger.

```yaml
name: Deploy static content to Pages

on:
  push:
    tags:
      - "deploy-*"

  workflow_dispatch:

  repository_dispatch:
    types: [deploy-pages]

permissions:
  contents: read
  pages: write
  id-token: write

concurrency:
  group: "pages"
  cancel-in-progress: false

jobs:
  deploy:
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}

    runs-on: ubuntu-latest

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Print trigger context
        run: |
          echo "Event: ${{ github.event_name }}"
          echo "Client payload: ${{ toJson(github.event.client_payload) }}"

      - name: Setup Pages
        uses: actions/configure-pages@v5

      - name: Upload artifact
        uses: actions/upload-pages-artifact@v3
        with:
          path: './documentation/html'

      - name: Deploy to GitHub Pages
        id: deployment
        uses: actions/deploy-pages@v4
```

---

## Part 6 — Secrets Required in Each System

### GitHub Repository Secrets

Go to your repository → Settings → Secrets and variables → Actions → New repository secret.

| Secret name | Value                                                         |
|-------------|---------------------------------------------------------------|
| `WEBHOOK`   | Your Discord webhook URL (plain text, not ROT13)              |
| `API`       | Your GROQ API key (already present in your current workflow)  |

The Discord webhook URL is used by the workflow to send results back to Discord. To get one:
1. Go to your Discord server → select a channel → Edit Channel → Integrations → Webhooks → New Webhook.
2. Copy the webhook URL.

### Cloudflare Worker Environment Variables

As described in Part 3.2.

---

## Part 7 — Verification Checklist

Work through this list in order before testing with Discord.

1. Worker is deployed and returns a response at its URL.
2. `DISCORD_PUBLIC_KEY`, `GITHUB_TOKEN`, and `GITHUB_REPO` are set in Cloudflare Worker environment variables as ROT13-encoded strings.
3. The Interactions Endpoint URL in the Discord Developer Portal points to your Worker URL and has been saved successfully (Discord verifies it on save).
4. Slash commands have been registered via the bulk overwrite curl command.
5. The bot has been invited to your server with the `applications.commands` scope.
6. Both workflow files have been committed to the `main` branch of the repository.
7. `WEBHOOK` secret is set in your GitHub repository.

---

## Part 8 — Testing

### Test the ping manually

```bash
curl -X POST https://your-worker.workers.dev \
  -H "Content-Type: application/json" \
  -d '{"type": 1}'
```

Expected response: `{"type":1}`

If you get `Invalid request signature`, the ping path is fine since you skipped signature headers. The Worker returns 401 for invalid signatures but the Discord portal sends a proper signed ping when you save the Interactions Endpoint URL — that is the actual verification step.

### Test a slash command

Type `/test` in your Discord server. You should see "CI test workflow triggered." and within a few seconds the workflow should appear in the Actions tab of your repository.

### Test issue and PR commands

Type `/list-issues` or `/list-pr` to verify the GitHub token has the correct permissions and the repo variable is set correctly.

---

## Part 9 — Common Errors

**401 from Discord on endpoint save**
The Worker signature verification is failing. Check that `DISCORD_PUBLIC_KEY` in Cloudflare is the ROT13-encoded version of the public key from the Discord Developer Portal General Information page.

**403 from GitHub on workflow trigger**
The token does not have Actions read/write permission, or the `repository_dispatch` event type does not match what is listed in the workflow `on.repository_dispatch.types` array.

**Workflow does not appear after /test**
Check the Worker logs in Cloudflare (Worker → Logs) for the GitHub API response. Also verify the workflow file is on the default branch (`main`) and that the file is syntactically valid YAML.

**Commands not appearing in Discord**
Global commands take up to one hour. Use guild-scoped command registration during development for instant propagation.
