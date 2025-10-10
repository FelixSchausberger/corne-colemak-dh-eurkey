#!/bin/bash
# GitHub Actions local testing with act
# Simplified wrapper with WSL2/NixOS certificate handling

set -euo pipefail

# Load color utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/colors.sh"

print_header "Running GitHub Actions locally with act..."

# Check dependencies
if ! command -v act > /dev/null 2>&1; then
    status_error "act command not found - run from nix develop shell"
    exit 1
fi

if ! command -v docker > /dev/null 2>&1; then
    status_error "Docker not found - act requires Docker"
    exit 1
fi

if ! docker info > /dev/null 2>&1; then
    status_error "Docker daemon not running"
    status_info "Start with: sudo systemctl start docker"
    exit 1
fi

# Parse workflow argument
WORKFLOW=".github/workflows/firmware-ci.yml"
ACT_ARGS="--pull=false"

for arg in "$@"; do
    if [[ "$arg" == *.yml ]] || [[ "$arg" == *.yaml ]]; then
        WORKFLOW="$arg"
    else
        ACT_ARGS="$ACT_ARGS $arg"
    fi
done

# Check if workflow exists
if [ ! -f "$WORKFLOW" ]; then
    status_error "Workflow file not found: $WORKFLOW"
    exit 1
fi

# Set up certificates for NixOS/WSL2
export SSL_CERT_FILE="/etc/ssl/certs/ca-bundle.crt"
export CURL_CA_BUNDLE="/etc/ssl/certs/ca-bundle.crt"

status_info "Testing workflow: $WORKFLOW"
status_info "Command: act -W \"$WORKFLOW\" $ACT_ARGS"
echo ""

# Run act with timeout
if command -v timeout >/dev/null 2>&1; then
    timeout 300s act -W "$WORKFLOW" $ACT_ARGS
    exit_code=$?
    if [ $exit_code -eq 124 ]; then
        status_warn "Act timed out (normal on first run)"
        status_info "Docker images need to download. Try again."
        exit 1
    fi
else
    act -W "$WORKFLOW" $ACT_ARGS
    exit_code=$?
fi

if [ $exit_code -eq 0 ]; then
    status_pass "GitHub Actions workflow completed successfully"
else
    status_fail "GitHub Actions workflow failed"
    echo ""
    echo "Common WSL2/NixOS certificate fixes:"
    echo "1. sudo systemctl restart docker"
    echo "2. sudo nixos-rebuild switch"
    echo "3. wsl --update && wsl --shutdown && wsl"
fi

exit $exit_code
