#!/bin/bash
# Nix configuration validation
# Tests flake configuration and syntax

set -euo pipefail

# Load color utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/colors.sh"

print_header "Validating configurations..."

# Test flake configuration
echo "Checking Nix flake configuration..."
if nix flake check --no-build; then
    status_pass "Flake configuration valid"
else
    status_fail "Flake configuration invalid"
    exit 1
fi

# Test flake syntax
echo ""
echo "Checking flake syntax..."
if nix flake show > /dev/null 2>&1; then
    status_pass "Flake syntax valid"
else
    status_fail "Flake syntax invalid"
    exit 1
fi

echo ""
status_pass "All configuration tests passed"
