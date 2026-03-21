#!/usr/bin/env python3

import argparse
import json
import os
import sys
import time
import urllib.error
import urllib.request


def load_messages(path):
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = json.load(fh)
    except FileNotFoundError:
        print(f"[discord] messages file not found: {path}; skipping")
        return []
    except json.JSONDecodeError as exc:
        print(f"[discord] invalid JSON in {path}: {exc}; skipping")
        return []

    if not isinstance(data, list):
        print(f"[discord] expected list in {path}; skipping")
        return []

    filtered = []
    for entry in data:
        if isinstance(entry, dict) and isinstance(entry.get("content"), str) and entry["content"].strip():
            filtered.append({"content": entry["content"]})
    return filtered


def post_message(webhook, payload):
    body = json.dumps(payload).encode("utf-8")
    request = urllib.request.Request(
        webhook,
        data=body,
        headers={"Content-Type": "application/json"},
        method="POST",
    )
    with urllib.request.urlopen(request, timeout=20) as response:
        return response.status


def parse_retry_after_seconds(headers, details):
    retry_after = headers.get("Retry-After") if headers else None
    if retry_after:
        try:
            return max(float(retry_after), 0.0)
        except ValueError:
            pass

    try:
        data = json.loads(details)
        if isinstance(data, dict) and "retry_after" in data:
            value = float(data["retry_after"])
            return max(value, 0.0)
    except Exception:
        pass

    return 2.0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--webhook", required=True)
    parser.add_argument("--messages-file", required=True)
    parser.add_argument("--label", default="messages")
    parser.add_argument("--max-messages", type=int, default=5)
    parser.add_argument("--delay-seconds", type=float, default=1.5)
    parser.add_argument("--max-retries", type=int, default=3)
    args = parser.parse_args()

    block_file = os.environ.get("DISCORD_BLOCK_FILE", ".discord_webhook_blocked")
    if os.path.exists(block_file):
        print(f"[discord] webhook is marked blocked ({block_file}); skipping {args.label}")
        return 0

    messages = load_messages(args.messages_file)
    if not messages:
        print(f"[discord] no {args.label} messages to send")
        return 0

    if args.max_messages > 0 and len(messages) > args.max_messages:
        print(
            f"[discord] limiting {args.label} messages from {len(messages)} to {args.max_messages}",
        )
        messages = messages[: args.max_messages]

    for idx, payload in enumerate(messages, start=1):
        attempt = 0
        while True:
            attempt += 1
            try:
                status = post_message(args.webhook, payload)
                if status >= 300:
                    print(f"[discord] {args.label} message {idx} failed with status {status}; skipping remainder")
                    return 0
                break
            except urllib.error.HTTPError as exc:
                details = exc.read().decode("utf-8", errors="replace")

                if exc.code in (401, 403, 404):
                    with open(block_file, "w", encoding="utf-8") as fh:
                        fh.write(f"http={exc.code}\n")
                        fh.write(details)
                    print(
                        f"[discord] {args.label} message {idx} HTTP error {exc.code}: {details}; skipping remainder",
                    )
                    return 0

                if exc.code == 429 and attempt <= args.max_retries:
                    wait_seconds = parse_retry_after_seconds(exc.headers, details)
                    print(
                        f"[discord] {args.label} message {idx} rate limited; retrying in {wait_seconds:.2f}s (attempt {attempt}/{args.max_retries})",
                    )
                    time.sleep(wait_seconds)
                    continue

                if 500 <= exc.code < 600 and attempt <= args.max_retries:
                    wait_seconds = min(2.0 * attempt, 8.0)
                    print(
                        f"[discord] {args.label} message {idx} server error {exc.code}; retrying in {wait_seconds:.2f}s (attempt {attempt}/{args.max_retries})",
                    )
                    time.sleep(wait_seconds)
                    continue

                print(
                    f"[discord] {args.label} message {idx} HTTP error {exc.code}: {details}; skipping remainder",
                )
                return 0
            except Exception as exc:
                if attempt <= args.max_retries:
                    wait_seconds = min(1.5 * attempt, 6.0)
                    print(
                        f"[discord] {args.label} message {idx} error: {exc}; retrying in {wait_seconds:.2f}s (attempt {attempt}/{args.max_retries})",
                    )
                    time.sleep(wait_seconds)
                    continue

                print(f"[discord] {args.label} message {idx} error: {exc}; skipping remainder")
                return 0

        time.sleep(max(args.delay_seconds, 0.0))

    print(f"[discord] sent {len(messages)} {args.label} message(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
