#!/bin/bash
# VIAL layout validation script
# Tests all .vil files in firmware directory

set -euo pipefail

# Load color utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/colors.sh"

print_header "Validating VIAL layouts..."

# Use the validation script
if [ -f "scripts/validate-vial.sh" ]; then
    exec ./scripts/validate-vial.sh firmware
else
    status_error "validate-vial.sh script not found"
    exit 1
fi
