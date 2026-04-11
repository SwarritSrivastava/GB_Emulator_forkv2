#!/usr/bin/env python3

import argparse
import os
import re
import sys
from pathlib import Path


RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
CYAN = "\033[36m"

FULL_BLOCK = "█"
EMPTY_BLOCK = "░"


def parse_opcode_declarations(header_path: Path) -> list[tuple[str, str]]:
    entries: list[tuple[str, str]] = []
    pattern = re.compile(
        r"^\s*int\s+(op_[A-Za-z0-9_]+)\s*\([^;]*\)\s*;\s*//\s*(0x[0-9A-Fa-f]+)\s*$"
    )

    for line in header_path.read_text(encoding="utf-8").splitlines():
        match = pattern.match(line)
        if match:
            entries.append((match.group(1), match.group(2).upper()))

    return entries


def collect_dummy_symbols(source_dir: Path) -> set[str]:
    pattern = re.compile(r"DUMMY\(\s*(op_[A-Za-z0-9_]+)\s*\)")
    dummies: set[str] = set()

    for cpp_file in sorted(source_dir.glob("*.cpp")):
        content = cpp_file.read_text(encoding="utf-8")
        for symbol in pattern.findall(content):
            dummies.add(symbol)

    return dummies


def use_color(args: argparse.Namespace) -> bool:
    if args.no_color:
        return False
    if args.force_color:
        return True
    if os.environ.get("NO_COLOR") is not None:
        return False
    return sys.stdout.isatty()


def color_text(text: str, color: str, enabled: bool) -> str:
    if not enabled or not text:
        return text
    return f"{color}{text}{RESET}"


def progress_bar(
    done: int, total: int, width: int, unicode_bar: bool, colored: bool, fill_color: str
) -> str:
    if total <= 0:
        empty_char = EMPTY_BLOCK if unicode_bar else "-"
        return "[" + (empty_char * width) + "]"

    filled = int((done * width) / total)
    filled = max(0, min(width, filled))
    empty = width - filled

    full_char = FULL_BLOCK if unicode_bar else "#"
    empty_char = EMPTY_BLOCK if unicode_bar else "-"

    fill_text = full_char * filled
    empty_text = empty_char * empty

    if colored:
        fill_text = color_text(fill_text, fill_color, True)
        empty_text = color_text(empty_text, DIM, True)

    return f"[{fill_text}{empty_text}]"


def print_section(
    label: str,
    done: int,
    total: int,
    width: int,
    unicode_bar: bool,
    colored: bool,
    fill_color: str,
    label_width: int,
) -> None:
    pct = 0.0 if total == 0 else (done / total) * 100.0
    bar = progress_bar(done, total, width, unicode_bar, colored, fill_color)
    spacer = " " * (label_width - len(label) + 1)
    pct_text = f"{pct:.1f}%"
    if colored:
        pct_text = color_text(pct_text, BOLD, True)
    print(f"- {label}:{spacer}{bar} {pct_text} ({done}/{total})")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Show opcode implementation progress based on DUMMY placeholders"
    )
    parser.add_argument(
        "--width",
        type=int,
        default=20,
        help="Progress bar width (default: 20)",
    )
    parser.add_argument(
        "--show-missing",
        action="store_true",
        help="Print missing opcode symbols",
    )
    parser.add_argument(
        "--ascii",
        action="store_true",
        help="Use ASCII bars (# and -) instead of block bars",
    )
    parser.add_argument(
        "--no-color",
        action="store_true",
        help="Disable ANSI colors",
    )
    parser.add_argument(
        "--force-color",
        action="store_true",
        help="Force ANSI colors even when not in a TTY",
    )
    args = parser.parse_args()

    project_root = Path(__file__).resolve().parent.parent

    base_header = project_root / "include" / "opcodes.hpp"
    cb_header = project_root / "include" / "cb_opcodes.hpp"
    base_src = project_root / "src" / "core" / "cpu" / "instructions" / "opcodes"
    cb_src = project_root / "src" / "core" / "cpu" / "instructions" / "cb_opcodes"

    base_entries = parse_opcode_declarations(base_header)
    cb_entries = parse_opcode_declarations(cb_header)

    base_dummies = collect_dummy_symbols(base_src)
    cb_dummies = collect_dummy_symbols(cb_src)

    base_missing = [
        (name, opcode) for name, opcode in base_entries if name in base_dummies
    ]
    cb_missing = [(name, opcode) for name, opcode in cb_entries if name in cb_dummies]

    base_total = len(base_entries)
    cb_total = len(cb_entries)
    base_done = base_total - len(base_missing)
    cb_done = cb_total - len(cb_missing)

    total_all = base_total + cb_total
    done_all = base_done + cb_done

    colored = use_color(args)
    unicode_bar = not args.ascii
    label_width = len("Base Opcodes")

    print_section(
        "Base Opcodes",
        base_done,
        base_total,
        args.width,
        unicode_bar,
        colored,
        YELLOW,
        label_width,
    )
    print_section(
        "CB Opcodes",
        cb_done,
        cb_total,
        args.width,
        unicode_bar,
        colored,
        GREEN,
        label_width,
    )
    print_section(
        "Combined",
        done_all,
        total_all,
        args.width,
        unicode_bar,
        colored,
        CYAN,
        label_width,
    )

    if args.show_missing:
        if base_missing:
            print("\nMissing Base Opcodes")
            for name, opcode in base_missing:
                print(f"- {opcode}: {name}")
        if cb_missing:
            print("\nMissing CB Opcodes")
            for name, opcode in cb_missing:
                print(f"- {opcode}: {name}")
        if not base_missing and not cb_missing:
            print("\nNo missing opcode implementations detected.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
