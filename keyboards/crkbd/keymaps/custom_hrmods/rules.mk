# Copyright 2024
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.

# ============================================================================
# VIAL Support
# ============================================================================

VIAL_ENABLE = yes
VIAL_INSECURE = yes  # Disable encryption for easier development

# ============================================================================
# RGB Matrix Support
# ============================================================================

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = ws2812

# ============================================================================
# Features
# ============================================================================

COMBO_ENABLE = yes
TAP_DANCE_ENABLE = yes
EXTRAKEY_ENABLE = yes        # Audio control and System control
MOUSEKEY_ENABLE = yes        # Mouse keys
NKRO_ENABLE = yes            # N-Key Rollover
RGBLIGHT_ENABLE = no         # Disable old RGB underglow (we use RGB Matrix)

# ============================================================================
# Size Optimization
# ============================================================================

LTO_ENABLE = yes             # Link Time Optimization (reduces firmware size)
CONSOLE_ENABLE = no          # Console for debug
COMMAND_ENABLE = no          # Commands for debug and configuration
SPACE_CADET_ENABLE = no      # We use custom shift keys
GRAVE_ESC_ENABLE = no        # We use custom escape behavior

# ============================================================================
# Split Keyboard Support (Corne V4.1)
# ============================================================================

SPLIT_KEYBOARD = yes
SERIAL_DRIVER = vendor

# ============================================================================
# Bootloader
# ============================================================================

# Corne V4.1 uses RP2040 with UF2 bootloader
BOOTLOADER = rp2040
# Note: crkbd/rev4_1 natively supports RP2040, no converter needed

# ============================================================================
# Custom Code
# ============================================================================

SRC += features/bilateral_combinations.c
