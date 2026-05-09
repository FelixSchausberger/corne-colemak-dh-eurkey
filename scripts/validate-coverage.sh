#!/usr/bin/env bash
# Coverage validation for printable ASCII, EurKey subset, and keymap drift

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec python3 "$SCRIPT_DIR/validate-coverage.py" "$@"
