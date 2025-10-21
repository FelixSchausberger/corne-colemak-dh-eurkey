#!/bin/bash
# Complete test suite for keyboard configuration
# Equivalent to cargo nextest run --workspace

set -euo pipefail

# Load color utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/colors.sh"

print_header "Running complete test suite..."
status_info "Started at: $(date)"
echo ""

# Track test results
FAILED_TESTS=()
TOTAL_TESTS=0

# Function to run test and track results
run_test() {
    local test_name="$1"
    local test_script="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    status_running "$test_name"

    if $test_script; then
        status_pass "$test_name"
    else
        status_fail "$test_name"
        FAILED_TESTS+=("$test_name")
    fi
    echo ""
}

# Script directory already defined above for colors

# Run essential tests only
run_test "Layout Validation" "$SCRIPT_DIR/test-layouts.sh"
run_test "Configuration Validation" "$SCRIPT_DIR/test-configs.sh"

# Optional: Run GitHub Actions locally (can be slow on first run)
if [ "${RUN_ACT_CHECK:-}" = "true" ]; then
    run_test "GitHub Actions (local)" "$SCRIPT_DIR/act-check.sh"
else
    status_info "Skipping act-check (set RUN_ACT_CHECK=true to enable)"
fi

# Summary
print_section "Test Results Summary"
status_info "Completed at: $(date)"
echo "Total tests: $TOTAL_TESTS"
echo "Passed: $((TOTAL_TESTS - ${#FAILED_TESTS[@]}))"
echo "Failed: ${#FAILED_TESTS[@]}"

if [ ${#FAILED_TESTS[@]} -eq 0 ]; then
    echo ""
    status_pass "All tests passed"
    exit 0
else
    echo ""
    status_fail "Failed tests:"
    for test in "${FAILED_TESTS[@]}"; do
        printf "  %s %s\n" "$(color_red "•")" "$test"
    done
    echo ""
    status_fail "Test suite failed"
    exit 1
fi
