#!/usr/bin/env python3

import argparse
import curses
import html
import json
import re
import shutil
import subprocess
import sys
import tempfile
import textwrap
from datetime import datetime, timezone
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parent.parent
DIST_DIR = PROJECT_ROOT / "dist"
INDEX_HTML = DIST_DIR / "index.html"
INDEX_JSON = DIST_DIR / "opcode_lookup_index.json"


def find_chromium_binary() -> str:
    for name in ("chromium", "chromium-browser", "google-chrome", "google-chrome-stable"):
        binary = shutil.which(name)
        if binary:
            return binary
    raise RuntimeError("No Chromium binary found. Install chromium/chromium-browser.")


def run_chromium_dump(helper_file: Path) -> str:
    binary = find_chromium_binary()
    cmd = [
        binary,
        "--headless",
        "--disable-gpu",
        "--allow-file-access-from-files",
        "--virtual-time-budget=20000",
        "--dump-dom",
        f"file://{helper_file.as_posix()}",
    ]
    proc = subprocess.run(cmd, capture_output=True, text=True, check=False)
    if proc.returncode != 0:
        raise RuntimeError(f"Chromium failed: {proc.stderr.strip() or proc.stdout.strip()}")
    return proc.stdout


def scrape_rows_from_dist() -> list[dict]:
    if not INDEX_HTML.exists():
        raise FileNotFoundError(f"Missing {INDEX_HTML}")

    with tempfile.TemporaryDirectory(prefix="gb_opcode_lookup_") as td:
        temp_dir = Path(td)
        helper = temp_dir / "dump.html"

        helper.write_text(
            f"""<!doctype html><html><body><pre id=\"out\">[]</pre><script>
(async function() {{
  const out = document.getElementById('out');
  const iframe = document.createElement('iframe');
  iframe.src = 'file://{INDEX_HTML.resolve().as_posix()}';
  iframe.style.width = '1400px';
  iframe.style.height = '1000px';
  document.body.appendChild(iframe);

  await new Promise((resolve) => iframe.addEventListener('load', resolve, {{ once: true }}));
  const doc = iframe.contentWindow.document;

  function sleep(ms) {{ return new Promise((resolve) => setTimeout(resolve, ms)); }}

  for (let i = 0; i < 120; i += 1) {{
    if (doc.querySelectorAll('button[aria-label^="Opcode:"]').length > 0) break;
    await sleep(50);
  }}

  const buttons = Array.from(doc.querySelectorAll('button[aria-label^="Opcode:"]'));
  const rows = [];

  for (const btn of buttons) {{
    btn.click();
    await sleep(12);
    const sidebar = doc.querySelector('#details-sidebar');
    const title = sidebar?.querySelector('h2')?.textContent?.trim() || '';
    const fields = Array.from(sidebar?.querySelectorAll('li') || []).map((x) => x.textContent.trim());
    const desc = sidebar?.querySelector('section[aria-label="description"]')?.innerText?.replace(/\\s+/g, ' ').trim() || '';
    const aria = btn.getAttribute('aria-label') || '';
    rows.push({{ aria, title, fields, description: desc }});
  }}

  out.textContent = JSON.stringify(rows);
  document.title = 'done';
}})();
</script></body></html>""",
            encoding="utf-8",
        )

        dumped = run_chromium_dump(helper)

    match = re.search(r"<pre id=\"out\">([\s\S]*?)</pre>", dumped)
    if not match:
        raise RuntimeError("Could not extract opcode data from rendered DOM")

    raw = html.unescape(match.group(1))
    rows = json.loads(raw)
    if not isinstance(rows, list):
        raise RuntimeError("Unexpected scrape payload format")
    return rows


def normalize_spaces(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def parse_fields(fields: list[str]) -> tuple[str, int, str, dict]:
    opcode = ""
    bytes_count = -1
    cycles = ""
    flags = {"Z": "-", "N": "-", "H": "-", "CY": "-"}

    for item in fields:
        if item.startswith("Opcode:"):
            opcode = normalize_spaces(item.split(":", 1)[1])
        elif item.startswith("Number of Bytes:"):
            raw = normalize_spaces(item.split(":", 1)[1])
            try:
                bytes_count = int(raw)
            except ValueError:
                bytes_count = -1
        elif item.startswith("Number of Cycles:"):
            cycles = normalize_spaces(item.split(":", 1)[1])
        elif item.startswith("Flags:"):
            raw = normalize_spaces(item.split(":", 1)[1])
            parts = raw.split(" ")
            while len(parts) < 4:
                parts.append("-")
            flags = {
                "Z": parts[0],
                "N": parts[1],
                "H": parts[2],
                "CY": parts[3],
            }

    return opcode, bytes_count, cycles, flags


def canonical_opcode(opcode_text: str) -> str:
    cleaned = opcode_text.upper().replace(" ", "")
    if cleaned.startswith("0X"):
        cleaned = cleaned[2:]
    return f"0x{cleaned}"


def rows_to_entries(rows: list[dict]) -> list[dict]:
    entries = []

    for row in rows:
        if not isinstance(row, dict):
            continue

        fields = row.get("fields", [])
        if not isinstance(fields, list):
            fields = []

        opcode_text, bytes_count, cycles, flags = parse_fields(fields)
        if not opcode_text:
            continue

        opcode = canonical_opcode(opcode_text)
        mnemonic = normalize_spaces(str(row.get("title", "")))
        description = normalize_spaces(str(row.get("description", "")))
        if description.lower().startswith("description "):
            description = description[len("Description ") :].strip()

        type_label = "cb" if opcode.upper().startswith("0XCB") else "base"

        entries.append(
            {
                "opcode": opcode,
                "prefix": type_label,
                "mnemonic": mnemonic,
                "bytes": bytes_count,
                "cycles": cycles,
                "flags": flags,
                "description": description,
            }
        )

    unique = {}
    for entry in entries:
        unique[entry["opcode"].upper()] = entry

    deduped = list(unique.values())
    deduped.sort(key=lambda x: int(x["opcode"][2:], 16))
    return deduped


def build_index() -> dict:
    rows = scrape_rows_from_dist()
    entries = rows_to_entries(rows)

    payload = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "source": str(INDEX_HTML.relative_to(PROJECT_ROOT)),
        "count": len(entries),
        "entries": entries,
    }

    INDEX_JSON.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    return payload


def parse_query_as_opcode(query: str) -> str | None:
    q = query.strip().upper()
    q = re.sub(r"\s+", " ", q)

    for pattern in (
        r"^0XCB([0-9A-F]{2})$",
        r"^CB\s*([0-9A-F]{2})$",
        r"^CB[:\- ]([0-9A-F]{2})$",
    ):
        m = re.match(pattern, q)
        if m:
            return f"0xCB{m.group(1)}"

    m4 = re.match(r"^0X([0-9A-F]{4})$", q)
    if m4:
        return f"0x{m4.group(1)}"

    m2 = re.match(r"^(?:0X)?([0-9A-F]{2})$", q)
    if m2:
        return f"0x{m2.group(1)}"

    return None


def load_index() -> dict:
    if not INDEX_JSON.exists():
        raise FileNotFoundError(
            f"Missing {INDEX_JSON}. Run: python3 utility_scripts/opcode_lookup.py --build"
        )

    data = json.loads(INDEX_JSON.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or "entries" not in data:
        raise RuntimeError(f"Invalid index format: {INDEX_JSON}")
    return data


def find_entries(index: dict, query: str) -> list[dict]:
    entries = index.get("entries", [])
    if not isinstance(entries, list):
        return []

    opcode = parse_query_as_opcode(query)
    if opcode:
        key = opcode.upper()
        return [e for e in entries if str(e.get("opcode", "")).upper() == key]

    needle = normalize_spaces(query).lower()
    if not needle:
        return []

    exact = [e for e in entries if str(e.get("mnemonic", "")).lower() == needle]
    if exact:
        return exact

    return [e for e in entries if needle in str(e.get("mnemonic", "")).lower()]


def print_entry(entry: dict) -> None:
    flags = entry.get("flags", {})
    z = flags.get("Z", "-")
    n = flags.get("N", "-")
    h = flags.get("H", "-")
    cy = flags.get("CY", "-")

    print(f"Opcode      : {entry.get('opcode', '')}")
    print(f"Mnemonic    : {entry.get('mnemonic', '')}")
    print(f"Prefix      : {entry.get('prefix', '')}")
    print(f"Bytes       : {entry.get('bytes', '')}")
    print(f"Cycles      : {entry.get('cycles', '')}")
    print(f"Flags (ZNHC): {z} {n} {h} {cy}")
    print("Description :")
    print(f"  {entry.get('description', '')}")


def build_search_text(entry: dict) -> str:
    parts = [
        str(entry.get("opcode", "")),
        str(entry.get("mnemonic", "")),
        str(entry.get("prefix", "")),
        str(entry.get("cycles", "")),
        str(entry.get("bytes", "")),
        str(entry.get("description", "")),
    ]
    return " ".join(parts).lower()


def filter_entries(entries: list[dict], query: str) -> list[dict]:
    needle = normalize_spaces(query).lower()
    if not needle:
        return entries

    opcode = parse_query_as_opcode(needle)
    if opcode:
        key = opcode.upper()
        exact = [e for e in entries if str(e.get("opcode", "")).upper() == key]
        if exact:
            return exact

    tokens = [t for t in needle.split(" ") if t]
    filtered = []
    for entry in entries:
        haystack = build_search_text(entry)
        if all(token in haystack for token in tokens):
            filtered.append(entry)
    return filtered


def wrap_lines(text: str, width: int) -> list[str]:
    if width <= 1:
        return [text]
    out = []
    for raw in (text or "").splitlines() or [""]:
        chunks = textwrap.wrap(raw, width=width, replace_whitespace=False, drop_whitespace=False)
        if chunks:
            out.extend(chunks)
        else:
            out.append("")
    return out


def init_tui_colors() -> dict[str, int]:
    palette = {
        "title": 0,
        "label": 0,
        "muted": 0,
        "selected": curses.A_REVERSE,
        "divider": 0,
        "value": 0,
        "good": 0,
        "warn": 0,
    }

    if not curses.has_colors():
        return palette

    curses.start_color()
    try:
        curses.use_default_colors()
        bg = -1
    except curses.error:
        bg = curses.COLOR_BLACK

    curses.init_pair(1, curses.COLOR_CYAN, bg)
    curses.init_pair(2, curses.COLOR_WHITE, bg)
    curses.init_pair(3, curses.COLOR_BLUE, bg)
    curses.init_pair(4, curses.COLOR_BLACK, curses.COLOR_CYAN)
    curses.init_pair(5, curses.COLOR_GREEN, bg)
    curses.init_pair(6, curses.COLOR_YELLOW, bg)

    palette.update(
        {
            "title": curses.color_pair(1) | curses.A_BOLD,
            "label": curses.color_pair(2) | curses.A_BOLD,
            "muted": curses.color_pair(3),
            "selected": curses.color_pair(4) | curses.A_BOLD,
            "divider": curses.color_pair(3),
            "value": curses.color_pair(2),
            "good": curses.color_pair(5) | curses.A_BOLD,
            "warn": curses.color_pair(6) | curses.A_BOLD,
        }
    )
    return palette


def draw_tui(stdscr, entries: list[dict]) -> int:
    curses.curs_set(0)
    stdscr.keypad(True)
    palette = init_tui_colors()

    query = ""
    selected = 0
    scroll = 0

    while True:
        height, width = stdscr.getmaxyx()
        if width < 70 or height < 16:
            stdscr.erase()
            stdscr.addstr(0, 0, "Terminal too small. Resize to at least 70x16.", palette["warn"])
            stdscr.addstr(1, 0, "Press q to quit.", palette["muted"])
            stdscr.refresh()
            ch = stdscr.getch()
            if ch in (ord("q"), ord("Q")):
                return 0
            continue

        filtered = filter_entries(entries, query)
        if filtered:
            selected = max(0, min(selected, len(filtered) - 1))
        else:
            selected = 0

        list_width = max(30, min(44, width // 2 - 2))
        detail_x = list_width + 2
        detail_width = width - detail_x - 1
        list_rows = height - 5

        if selected < scroll:
            scroll = selected
        if selected >= scroll + list_rows:
            scroll = selected - list_rows + 1
        if scroll < 0:
            scroll = 0

        stdscr.erase()
        stdscr.addstr(0, 0, "GB Opcode Lookup Interface", palette["title"])
        stdscr.addstr(1, 0, "Search:", palette["label"])
        stdscr.addstr(1, 8, f"{query}", palette["value"])
        stdscr.addstr(2, 0, "Matches:", palette["label"])
        stdscr.addstr(2, 9, f"{len(filtered)}/{len(entries)}", palette["good"])
        stdscr.addstr(2, detail_x, "Keys: Up/Down PgUp/PgDn Backspace q", palette["muted"])

        for row in range(3, height - 2):
            stdscr.addch(row, list_width + 1, "|", palette["divider"])

        visible = filtered[scroll : scroll + list_rows]
        for idx, entry in enumerate(visible):
            y = 3 + idx
            absolute_idx = scroll + idx
            label = f"{entry.get('opcode', ''):<8} {entry.get('mnemonic', '')}"
            label = label[: list_width - 1]
            if absolute_idx == selected:
                stdscr.addstr(y, 0, label.ljust(list_width - 1), palette["selected"])
            else:
                stdscr.addstr(y, 0, label.ljust(list_width - 1), palette["value"])

        if filtered:
            current = filtered[selected]
            flags = current.get("flags", {})
            flag_line = (
                f"Z={flags.get('Z', '-')} "
                f"N={flags.get('N', '-')} "
                f"H={flags.get('H', '-')} "
                f"CY={flags.get('CY', '-')}"
            )
            detail_rows: list[tuple[str, str]] = [
                ("Opcode", str(current.get("opcode", ""))),
                ("Mnemonic", str(current.get("mnemonic", ""))),
                ("Prefix", str(current.get("prefix", ""))),
                ("Bytes", str(current.get("bytes", ""))),
                ("Cycles", str(current.get("cycles", ""))),
                ("Flags", flag_line),
            ]

            y = 3
            for label, value in detail_rows:
                if y >= height - 1:
                    break
                stdscr.addstr(y, detail_x, f"{label:<9}", palette["label"])
                value_attr = palette["good"] if label in ("Opcode", "Cycles", "Bytes") else palette["value"]
                stdscr.addstr(y, detail_x + 10, value[: max(1, detail_width - 11)], value_attr)
                y += 1

            if y < height - 1:
                y += 1
            if y < height - 1:
                stdscr.addstr(y, detail_x, "Description", palette["label"])
                y += 1

            desc_lines = wrap_lines(str(current.get("description", "")), detail_width - 1)
            for line in desc_lines:
                if y >= height - 1:
                    break
                stdscr.addstr(y, detail_x, line[: detail_width - 1], palette["value"])
                y += 1
        else:
            stdscr.addstr(3, detail_x, "No matches.", palette["warn"])

        stdscr.refresh()
        ch = stdscr.getch()

        if ch in (ord("q"), ord("Q"), 27):
            return 0
        if ch in (curses.KEY_UP, ord("k")):
            if filtered:
                selected = max(0, selected - 1)
            continue
        if ch in (curses.KEY_DOWN, ord("j")):
            if filtered:
                selected = min(len(filtered) - 1, selected + 1)
            continue
        if ch == curses.KEY_NPAGE:
            if filtered:
                selected = min(len(filtered) - 1, selected + max(1, list_rows - 2))
            continue
        if ch == curses.KEY_PPAGE:
            if filtered:
                selected = max(0, selected - max(1, list_rows - 2))
            continue
        if ch in (curses.KEY_BACKSPACE, 127, 8):
            if query:
                query = query[:-1]
                selected = 0
                scroll = 0
            continue
        if 32 <= ch <= 126:
            query += chr(ch)
            selected = 0
            scroll = 0
            continue


def run_tui(index: dict) -> int:
    entries = index.get("entries", [])
    if not isinstance(entries, list):
        print("Invalid index data", file=sys.stderr)
        return 1
    return curses.wrapper(lambda stdscr: draw_tui(stdscr, entries))


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Lookup Game Boy opcodes from dist/index.html metadata"
    )
    parser.add_argument("query", nargs="?", help="opcode (e.g. 0x04, CB 11) or mnemonic")
    parser.add_argument("--build", action="store_true", help="build dist/opcode_lookup_index.json")
    parser.add_argument("--json", action="store_true", help="output JSON for query result")
    parser.add_argument("--all", action="store_true", help="show all mnemonic matches")
    parser.add_argument("--tui", action="store_true", help="launch interactive terminal UI")
    args = parser.parse_args()

    if args.build:
        payload = build_index()
        print(
            f"Built {INDEX_JSON.relative_to(PROJECT_ROOT)} with {payload.get('count', 0)} entries"
        )
        if not args.query:
            return 0

    try:
        index = load_index()
    except Exception as exc:
        print(str(exc), file=sys.stderr)
        return 1

    if args.tui or not args.query:
        return run_tui(index)

    results = find_entries(index, args.query)
    if not results:
        print(f"No results for: {args.query}")
        return 1

    if args.json:
        payload = results if (args.all or len(results) == 1) else [results[0]]
        print(json.dumps(payload, indent=2))
        return 0

    if len(results) > 1 and not args.all:
        print(f"Found {len(results)} matches, showing best match. Use --all to list all.")
        print()
        print_entry(results[0])
        return 0

    if len(results) > 1:
        for idx, entry in enumerate(results, start=1):
            print(f"[{idx}] {entry.get('opcode', '')}  {entry.get('mnemonic', '')}")
        return 0

    print_entry(results[0])
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        print("Interrupted", file=sys.stderr)
        raise SystemExit(130)
