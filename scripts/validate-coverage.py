#!/usr/bin/env python3
"""Validate layout coverage against ASCII, EurKey, and keymap-drawer mappings."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


XKB_LEVEL3_RE = re.compile(r"^\s*key\s+<([A-Z0-9]+)>\s*\{\[([^\]]+)\]\};")
RALT_KEY_RE = re.compile(r'^\s*"(RALT\([^"]+\))"\s*:')


@dataclass(frozen=True)
class EurKeyEntry:
    key_name: str
    qmk_keycode: str
    level3_symbol: str


ASCII_CHAR_TO_QMK = {
    " ": "KC_SPACE",
    "!": "LSFT(KC_1)",
    '"': "LSFT(KC_QUOTE)",
    "#": "LSFT(KC_3)",
    "$": "LSFT(KC_4)",
    "%": "LSFT(KC_5)",
    "&": "LSFT(KC_7)",
    "'": "KC_QUOTE",
    "(": "LSFT(KC_9)",
    ")": "LSFT(KC_0)",
    "*": "LSFT(KC_8)",
    "+": "LSFT(KC_EQUAL)",
    ",": "KC_COMMA",
    "-": "KC_MINUS",
    ".": "KC_DOT",
    "/": "KC_SLASH",
    "0": "KC_0",
    "1": "KC_1",
    "2": "KC_2",
    "3": "KC_3",
    "4": "KC_4",
    "5": "KC_5",
    "6": "KC_6",
    "7": "KC_7",
    "8": "KC_8",
    "9": "KC_9",
    ":": "LSFT(KC_SCOLON)",
    ";": "KC_SCOLON",
    "<": "LSFT(KC_COMMA)",
    "=": "KC_EQUAL",
    ">": "LSFT(KC_DOT)",
    "?": "LSFT(KC_SLASH)",
    "@": "LSFT(KC_2)",
    "A": "LSFT(KC_A)",
    "B": "LSFT(KC_B)",
    "C": "LSFT(KC_C)",
    "D": "LSFT(KC_D)",
    "E": "LSFT(KC_E)",
    "F": "LSFT(KC_F)",
    "G": "LSFT(KC_G)",
    "H": "LSFT(KC_H)",
    "I": "LSFT(KC_I)",
    "J": "LSFT(KC_J)",
    "K": "LSFT(KC_K)",
    "L": "LSFT(KC_L)",
    "M": "LSFT(KC_M)",
    "N": "LSFT(KC_N)",
    "O": "LSFT(KC_O)",
    "P": "LSFT(KC_P)",
    "Q": "LSFT(KC_Q)",
    "R": "LSFT(KC_R)",
    "S": "LSFT(KC_S)",
    "T": "LSFT(KC_T)",
    "U": "LSFT(KC_U)",
    "V": "LSFT(KC_V)",
    "W": "LSFT(KC_W)",
    "X": "LSFT(KC_X)",
    "Y": "LSFT(KC_Y)",
    "Z": "LSFT(KC_Z)",
    "[": "KC_LBRACKET",
    "\\": "KC_BSLASH",
    "]": "KC_RBRACKET",
    "^": "LSFT(KC_6)",
    "_": "LSFT(KC_MINUS)",
    "`": "KC_GRAVE",
    "a": "KC_A",
    "b": "KC_B",
    "c": "KC_C",
    "d": "KC_D",
    "e": "KC_E",
    "f": "KC_F",
    "g": "KC_G",
    "h": "KC_H",
    "i": "KC_I",
    "j": "KC_J",
    "k": "KC_K",
    "l": "KC_L",
    "m": "KC_M",
    "n": "KC_N",
    "o": "KC_O",
    "p": "KC_P",
    "q": "KC_Q",
    "r": "KC_R",
    "s": "KC_S",
    "t": "KC_T",
    "u": "KC_U",
    "v": "KC_V",
    "w": "KC_W",
    "x": "KC_X",
    "y": "KC_Y",
    "z": "KC_Z",
    "{": "LSFT(KC_LBRACKET)",
    "|": "LSFT(KC_BSLASH)",
    "}": "LSFT(KC_RBRACKET)",
    "~": "LSFT(KC_GRAVE)",
}


QMK_FROM_XKB_KEY = {
    "TLDE": "KC_GRAVE",
    "AE01": "KC_1",
    "AE02": "KC_2",
    "AE03": "KC_3",
    "AE04": "KC_4",
    "AE05": "KC_5",
    "AE06": "KC_6",
    "AE07": "KC_7",
    "AE08": "KC_8",
    "AE09": "KC_9",
    "AE10": "KC_0",
    "AE11": "KC_MINUS",
    "AE12": "KC_EQUAL",
    "AD01": "KC_Q",
    "AD02": "KC_W",
    "AD03": "KC_E",
    "AD04": "KC_R",
    "AD05": "KC_T",
    "AD06": "KC_Y",
    "AD07": "KC_U",
    "AD08": "KC_I",
    "AD09": "KC_O",
    "AD10": "KC_P",
    "AD11": "KC_LBRACKET",
    "AD12": "KC_RBRACKET",
    "BKSL": "KC_BSLASH",
    "AC01": "KC_A",
    "AC02": "KC_S",
    "AC03": "KC_D",
    "AC04": "KC_F",
    "AC05": "KC_G",
    "AC06": "KC_H",
    "AC07": "KC_J",
    "AC08": "KC_K",
    "AC09": "KC_L",
    "AC10": "KC_SCOLON",
    "AC11": "KC_QUOTE",
    "AB01": "KC_Z",
    "AB02": "KC_X",
    "AB03": "KC_C",
    "AB04": "KC_V",
    "AB05": "KC_B",
    "AB06": "KC_N",
    "AB07": "KC_M",
    "AB08": "KC_COMMA",
    "AB09": "KC_DOT",
    "AB10": "KC_SLASH",
}


# Exact match from xkb symbol names to the preferred visual string in keymap-drawer.
XKB_TO_DRAWER_SYMBOL = {
    "adiaeresis": "ä",
    "odiaeresis": "ö",
    "udiaeresis": "ü",
    "ssharp": "ß",
    "degree": "°",
}


REQUIRED_EURKEY_SYMBOLS = ("adiaeresis", "odiaeresis", "udiaeresis", "ssharp", "EuroSign")


def fail(msg: str) -> None:
    print(f"[FAIL] {msg}")


def warn(msg: str) -> None:
    print(f"[WARN] {msg}")


def info(msg: str) -> None:
    print(f"[INFO] {msg}")


def ok(msg: str) -> None:
    print(f"[PASS] {msg}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Validate coverage of keyboard layouts")
    parser.add_argument("--firmware-dir", default="firmware", help="Directory containing .vil files")
    parser.add_argument("--drawer-config", default="keymap-drawer.yaml", help="keymap-drawer YAML config")
    parser.add_argument(
        "--layout-glob",
        default="*.vil",
        help="Glob pattern for layouts to validate inside firmware-dir",
    )
    parser.add_argument(
        "--mode",
        choices=("strict", "non-strict"),
        default="non-strict",
        help="strict: coverage issues fail; non-strict: coverage issues warn",
    )
    return parser.parse_args()


def flatten_layout_keys(layout: list) -> set[str]:
    keys: set[str] = set()
    for layer in layout:
        for row in layer:
            for cell in row:
                if isinstance(cell, str):
                    keys.add(cell)
    return keys


def split_code(code: str) -> str:
    if code.startswith("KC_"):
        return code
    if code.startswith("RALT(") and code.endswith(")"):
        return split_code(code[5:-1])
    if code.startswith("LSFT(") and code.endswith(")"):
        inner = split_code(code[5:-1])
        return f"LSFT({inner})" if inner.startswith("KC_") else inner
    if code.startswith("MT(") and code.endswith(")"):
        parts = [part.strip() for part in code[3:-1].split(",")]
        if len(parts) == 2:
            return split_code(parts[1])
    return code


def expand_virtual_keys(keys: set[str]) -> set[str]:
    expanded = set(keys)
    has_shift = any(
        key in keys
        for key in (
            "KC_LSHIFT",
            "KC_RSHIFT",
            "KC_SFTENT",
        )
    ) or any(
        key.startswith("MT(") and ("MOD_LSFT" in key or "MOD_RSFT" in key)
        for key in keys
    )

    shiftable_symbols = {
        "KC_1",
        "KC_2",
        "KC_3",
        "KC_4",
        "KC_5",
        "KC_6",
        "KC_7",
        "KC_8",
        "KC_9",
        "KC_0",
        "KC_MINUS",
        "KC_EQUAL",
        "KC_LBRACKET",
        "KC_RBRACKET",
        "KC_BSLASH",
        "KC_SCOLON",
        "KC_QUOTE",
        "KC_GRAVE",
        "KC_COMMA",
        "KC_DOT",
        "KC_SLASH",
    }

    for key in keys:
        if key.startswith("MT(") and "MOD_RALT" in key:
            expanded.add("KC_RALT")

        base = split_code(key)
        if base.startswith("KC_"):
            expanded.add(base)
            if base.startswith("KC_") and len(base) == 4 and base[3].isalpha():
                expanded.add(f"LSFT({base})")
            if has_shift and base in shiftable_symbols:
                expanded.add(f"LSFT({base})")
    return expanded


def load_vil_keys(file_path: Path) -> set[str]:
    data = json.loads(file_path.read_text(encoding="utf-8"))
    layout = data.get("layout")
    if not isinstance(layout, list):
        raise ValueError(f"{file_path}: missing or invalid layout")
    keys = flatten_layout_keys(layout)
    return expand_virtual_keys(keys)


def nix_eval_out_path(attr: str) -> str:
    proc = subprocess.run(
        ["nix", "eval", "--raw", attr],
        capture_output=True,
        text=True,
        check=True,
    )
    return proc.stdout.strip()


def load_eurkey_entries() -> list[EurKeyEntry]:
    out_path = nix_eval_out_path("nixpkgs#xorg.xkeyboardconfig.outPath")
    symbol_file = Path(out_path) / "share" / "xkeyboard-config-2" / "symbols" / "eu"
    if not symbol_file.exists():
        raise FileNotFoundError(f"EurKey symbol file not found: {symbol_file}")

    entries: list[EurKeyEntry] = []
    for line in symbol_file.read_text(encoding="utf-8").splitlines():
        match = XKB_LEVEL3_RE.match(line)
        if not match:
            continue

        key_name = match.group(1)
        levels = [part.strip() for part in match.group(2).split(",")]
        if len(levels) < 3:
            continue

        if key_name not in QMK_FROM_XKB_KEY:
            continue

        level3 = levels[2]
        qmk = QMK_FROM_XKB_KEY[key_name]
        entries.append(EurKeyEntry(key_name=key_name, qmk_keycode=qmk, level3_symbol=level3))

    if not entries:
        raise RuntimeError("No EurKey entries parsed from xkb symbol file")

    return entries


def parse_ralt_drawer_map(drawer_config: Path) -> dict[str, str]:
    mapping: dict[str, str] = {}
    for line in drawer_config.read_text(encoding="utf-8").splitlines():
        key_match = RALT_KEY_RE.match(line)
        if not key_match:
            continue

        keycode = key_match.group(1)
        if ":" not in line:
            continue
        raw_value = line.split(":", 1)[1]
        value = raw_value.split("#", 1)[0].strip()
        if value.startswith('"') and value.endswith('"'):
            value = value[1:-1]
        mapping[keycode] = value
    return mapping


def validate_ascii_coverage(
    keys: set[str], layout_name: str, *, strict: bool
) -> tuple[bool, list[str]]:
    missing: list[str] = []
    for char, keycode in ASCII_CHAR_TO_QMK.items():
        if keycode not in keys:
            missing.append(f"{repr(char)} via {keycode}")

    if missing:
        reporter = fail if strict else warn
        reporter(f"{layout_name}: ASCII coverage missing {len(missing)} characters")
        for item in missing:
            print(f"  - {item}")
        return False, missing

    ok(f"{layout_name}: all 95 printable ASCII characters are reachable")
    return True, []


def validate_eurkey_required(
    keys: set[str],
    layout_name: str,
    eur_entries: list[EurKeyEntry],
    *,
    strict: bool,
) -> tuple[bool, list[str]]:
    by_symbol = {entry.level3_symbol: f"RALT({entry.qmk_keycode})" for entry in eur_entries}
    missing: list[str] = []

    for symbol in REQUIRED_EURKEY_SYMBOLS:
        expected = by_symbol.get(symbol)
        if expected is None:
            missing.append(f"{symbol} (not found in parsed EurKey data)")
            continue
        if expected not in keys:
            missing.append(f"{symbol} via {expected}")

    if "KC_RALT" not in keys:
        missing.append("KC_RALT (or MT with MOD_RALT)")

    if missing:
        reporter = fail if strict else warn
        reporter(f"{layout_name}: required EurKey coverage missing {len(missing)} entries")
        for item in missing:
            print(f"  - {item}")
        return False, missing

    ok(f"{layout_name}: required EurKey symbols (ä ö ü ß €) are covered")
    return True, []


def validate_drawer_drift(
    keys: set[str],
    layout_name: str,
    drawer_map: dict[str, str],
    eur_entries: list[EurKeyEntry],
    *,
    strict: bool,
) -> tuple[bool, list[str], list[str]]:
    missing_codes: list[str] = []
    mismatched_symbols: list[str] = []

    eur_by_keycode = {f"RALT({entry.qmk_keycode})": entry.level3_symbol for entry in eur_entries}

    for ralt_code, drawer_symbol in sorted(drawer_map.items()):
        if ralt_code not in keys:
            missing_codes.append(ralt_code)

        expected_xkb = eur_by_keycode.get(ralt_code)
        if expected_xkb is None:
            mismatched_symbols.append(
                f"{ralt_code}: present in keymap-drawer but absent from EurKey xkb"
            )
            continue

        expected_drawer = XKB_TO_DRAWER_SYMBOL.get(expected_xkb)
        if expected_drawer is not None and drawer_symbol != expected_drawer:
            mismatched_symbols.append(
                f"{ralt_code}: keymap-drawer='{drawer_symbol}', EurKey='{expected_drawer}'"
            )

    ok_codes = len(drawer_map) - len(missing_codes)
    if missing_codes:
        reporter = fail if strict else warn
        reporter(f"{layout_name}: keymap-drawer drift, {len(missing_codes)} RALT entries not in layout")
        for item in missing_codes:
            print(f"  - {item}")
    else:
        ok(f"{layout_name}: keymap-drawer RALT entries are present in layout ({ok_codes})")

    if mismatched_symbols:
        reporter = fail if strict else warn
        reporter(
            f"{layout_name}: keymap-drawer symbol mismatches against EurKey xkb ({len(mismatched_symbols)})"
        )
        for item in mismatched_symbols:
            print(f"  - {item}")
    else:
        ok(f"{layout_name}: keymap-drawer RALT symbols match EurKey xkb")

    return (not missing_codes and not mismatched_symbols, missing_codes, mismatched_symbols)


def main() -> int:
    args = parse_args()
    firmware_dir = Path(args.firmware_dir)
    drawer_config = Path(args.drawer_config)

    if not firmware_dir.exists() or not firmware_dir.is_dir():
        print(f"ERROR: Firmware directory not found: {firmware_dir}")
        return 1

    if not drawer_config.exists() or not drawer_config.is_file():
        print(f"ERROR: keymap-drawer config not found: {drawer_config}")
        return 1

    vil_files = sorted(firmware_dir.glob(args.layout_glob))
    if not vil_files:
        print(f"ERROR: No .vil files found in {firmware_dir} with glob {args.layout_glob}")
        return 1

    info("Loading EurKey xkb data from nixpkgs...")
    eur_entries = load_eurkey_entries()
    drawer_map = parse_ralt_drawer_map(drawer_config)

    failures = 0
    warned = 0

    for vil_file in vil_files:
        print()
        info(f"Validating coverage for {vil_file.name}")
        keys = load_vil_keys(vil_file)

        strict_mode = args.mode == "strict"
        ascii_ok, _ = validate_ascii_coverage(keys, vil_file.name, strict=strict_mode)
        eur_ok, _ = validate_eurkey_required(keys, vil_file.name, eur_entries, strict=strict_mode)
        drift_ok, _, _ = validate_drawer_drift(
            keys,
            vil_file.name,
            drawer_map,
            eur_entries,
            strict=strict_mode,
        )

        if not (ascii_ok and eur_ok and drift_ok):
            if args.mode == "strict":
                failures += 1
            else:
                warned += 1
                warn(f"{vil_file.name}: coverage issues reported in non-strict mode")

    print()
    if failures:
        fail(f"Coverage validation failed for {failures}/{len(vil_files)} layout(s)")
        return 1

    if warned:
        warn(f"Coverage validation completed with warnings for {warned}/{len(vil_files)} layout(s)")
        return 0

    ok(f"Coverage validation passed for all {len(vil_files)} layout(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
