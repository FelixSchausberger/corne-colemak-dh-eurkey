#!/bin/bash
# Simple VIAL layout validation
# Basic JSON validation for .vil files

set -euo pipefail

# Load color utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/colors.sh"

FIRMWARE_DIR="${1:-firmware}"
print_section "VIAL Layout Validation"

if [ ! -d "$FIRMWARE_DIR" ]; then
    status_error "Firmware directory not found: $FIRMWARE_DIR"
    exit 1
fi

# Find and validate VIAL files
TOTAL_FILES=0
ERRORS=0

for file in "$FIRMWARE_DIR"/*.vil; do
    if [ -f "$file" ]; then
        TOTAL_FILES=$((TOTAL_FILES + 1))
        echo "Validating: $(basename "$file")"

        # Basic JSON validation
        if jq empty "$file" 2>/dev/null; then
            status_pass "$(basename "$file"): Valid JSON"

            # Check for basic VIAL structure
            if jq -e '.layout' "$file" >/dev/null 2>&1; then
                layers=$(jq '.layout | length' "$file" 2>/dev/null || echo "0")
                status_info "  Has $layers layer(s)"
            else
                status_warn "  No layout data found"
            fi

            if jq -e '.vial_protocol' "$file" >/dev/null 2>&1; then
                version=$(jq -r '.vial_protocol' "$file")
                status_info "  VIAL protocol: $version"
            else
                status_warn "  No VIAL protocol version"
            fi
        else
            status_fail "$(basename "$file"): Invalid JSON"
            ERRORS=$((ERRORS + 1))
        fi
        echo ""
    fi
done

# Summary
print_section "Validation Summary"
echo "Files processed: $TOTAL_FILES"
echo "Errors: $ERRORS"

if [ $ERRORS -eq 0 ]; then
    status_pass "All VIAL files are valid"
    exit 0
else
    status_fail "Found $ERRORS error(s)"
    exit 1
fi
