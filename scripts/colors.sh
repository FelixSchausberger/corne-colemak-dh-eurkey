#!/bin/bash
# Color utility functions for terminal output
# Provides professional status indicators with ANSI colors

# Check if terminal supports colors
has_colors() {
    if [ -t 1 ] && command -v tput > /dev/null 2>&1; then
        if [ "$(tput colors 2>/dev/null)" -ge 8 ]; then
            return 0
        fi
    fi
    return 1
}

# Initialize color variables
if has_colors && [ "${NO_COLOR:-}" != "1" ]; then
    # ANSI color codes
    readonly COLOR_RED='\033[0;31m'
    readonly COLOR_GREEN='\033[0;32m'
    readonly COLOR_YELLOW='\033[0;33m'
    readonly COLOR_BLUE='\033[0;34m'
    readonly COLOR_CYAN='\033[0;36m'
    readonly COLOR_BOLD='\033[1m'
    readonly COLOR_RESET='\033[0m'

    # Using tput for better compatibility (fallback)
    readonly TPUT_RED=$(tput setaf 1 2>/dev/null || echo '')
    readonly TPUT_GREEN=$(tput setaf 2 2>/dev/null || echo '')
    readonly TPUT_YELLOW=$(tput setaf 3 2>/dev/null || echo '')
    readonly TPUT_BLUE=$(tput setaf 4 2>/dev/null || echo '')
    readonly TPUT_CYAN=$(tput setaf 6 2>/dev/null || echo '')
    readonly TPUT_BOLD=$(tput bold 2>/dev/null || echo '')
    readonly TPUT_RESET=$(tput sgr0 2>/dev/null || echo '')

    COLORS_ENABLED=true
else
    # No colors - empty variables
    readonly COLOR_RED=''
    readonly COLOR_GREEN=''
    readonly COLOR_YELLOW=''
    readonly COLOR_BLUE=''
    readonly COLOR_CYAN=''
    readonly COLOR_BOLD=''
    readonly COLOR_RESET=''

    readonly TPUT_RED=''
    readonly TPUT_GREEN=''
    readonly TPUT_YELLOW=''
    readonly TPUT_BLUE=''
    readonly TPUT_CYAN=''
    readonly TPUT_BOLD=''
    readonly TPUT_RESET=''

    COLORS_ENABLED=false
fi

# Color utility functions
color_red() {
    printf "${COLOR_RED}%s${COLOR_RESET}" "$1"
}

color_green() {
    printf "${COLOR_GREEN}%s${COLOR_RESET}" "$1"
}

color_yellow() {
    printf "${COLOR_YELLOW}%s${COLOR_RESET}" "$1"
}

color_blue() {
    printf "${COLOR_BLUE}%s${COLOR_RESET}" "$1"
}

color_cyan() {
    printf "${COLOR_CYAN}%s${COLOR_RESET}" "$1"
}

color_bold() {
    printf "${COLOR_BOLD}%s${COLOR_RESET}" "$1"
}

# Status indicator functions
status_pass() {
    printf "%s %s\n" "$(color_green "[PASS]")" "$1"
}

status_fail() {
    printf "%s %s\n" "$(color_red "[FAIL]")" "$1"
}

status_warn() {
    printf "%s %s\n" "$(color_yellow "[WARN]")" "$1"
}

status_info() {
    printf "%s %s\n" "$(color_blue "[INFO]")" "$1"
}

status_running() {
    printf "%s %s\n" "$(color_cyan "[RUNNING]")" "$1"
}

status_error() {
    printf "%s %s\n" "$(color_red "ERROR:")" "$1"
}

# Header functions
print_header() {
    printf "%s\n" "$(color_bold "$1")"
}

print_section() {
    printf "\n%s\n" "$(color_bold "$1")"
    printf "%s\n" "$(echo "$1" | sed 's/./=/g')"
}

# Test if colors are working (for debugging)
test_colors() {
    echo "Color test:"
    status_pass "This is a PASS message"
    status_fail "This is a FAIL message"
    status_warn "This is a WARN message"
    status_info "This is an INFO message"
    status_running "This is a RUNNING message"
    status_error "This is an ERROR message"
    print_header "This is a header"
    print_section "This is a section"
    echo ""
    echo "Colors enabled: $COLORS_ENABLED"
}
