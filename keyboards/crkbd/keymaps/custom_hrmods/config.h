/* Copyright 2024
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

// ============================================================================
// VIAL Configuration
// ============================================================================

#define VIAL_KEYBOARD_UID {0x8B, 0x08, 0x2D, 0x86, 0xB8, 0xC6, 0x08, 0x99}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 13 }

// ============================================================================
// Home Row Mods Configuration (Best Practice Settings)
// ============================================================================

// Tapping term: Time to hold before mod activates (in milliseconds)
// 175ms is a good balance - not too slow, not too fast for accidental activations
#define TAPPING_TERM 175

// PERMISSIVE_HOLD: Allows mod to activate immediately when another key is pressed
// This makes fast typing more reliable - you can quickly type "st" without Ctrl+t
#define PERMISSIVE_HOLD

// Per-key tapping term allows fine-tuning for specific keys
// We'll use this to give home row mods slightly longer time
#define TAPPING_TERM_PER_KEY

// Prevent accidental same-hand mod combinations (e.g., S+T = Ctrl+Shift)
// This requires defining the logic in keymap.c
#define BILATERAL_COMBINATIONS

// ============================================================================
// Tap Dance Configuration
// ============================================================================

#define TAPPING_TOGGLE 2  // How many taps to toggle a layer

// ============================================================================
// RGB Matrix Configuration
// ============================================================================

#ifdef RGB_MATRIX_ENABLE
    // Enable per-key RGB control
    #define RGB_MATRIX_KEYPRESSES

    // Disable unused RGB effects to save space
    #undef ENABLE_RGB_MATRIX_ALPHAS_MODS
    #undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #undef ENABLE_RGB_MATRIX_BREATHING
    #undef ENABLE_RGB_MATRIX_BAND_SAT
    #undef ENABLE_RGB_MATRIX_BAND_VAL
    #undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #undef ENABLE_RGB_MATRIX_CYCLE_ALL
    #undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #undef ENABLE_RGB_MATRIX_DUAL_BEACON
    #undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #undef ENABLE_RGB_MATRIX_RAINDROPS
    #undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #undef ENABLE_RGB_MATRIX_HUE_BREATHING
    #undef ENABLE_RGB_MATRIX_HUE_PENDULUM
    #undef ENABLE_RGB_MATRIX_HUE_WAVE
    #undef ENABLE_RGB_MATRIX_PIXEL_RAIN
    #undef ENABLE_RGB_MATRIX_PIXEL_FLOW
    #undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL

    // Default RGB settings
    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
    #define RGB_MATRIX_DEFAULT_HUE 0
    #define RGB_MATRIX_DEFAULT_SAT 255
    #define RGB_MATRIX_DEFAULT_VAL 128  // Medium brightness

    // RGB timeout (turn off after 10 minutes of inactivity)
    #define RGB_MATRIX_TIMEOUT 600000
#endif

// ============================================================================
// Combo Configuration
// ============================================================================

#define COMBO_COUNT 8  // Number of combos defined
#define COMBO_TERM 50  // Time window for combo activation (ms)

// ============================================================================
// Firmware Size Optimization
// ============================================================================

// Disable features we don't use to save space
#define NO_ACTION_ONESHOT
#define NO_MUSIC_MODE

// ============================================================================
// Split Keyboard Configuration (Corne V4.1 specific)
// ============================================================================

// Communication settings for split
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 2000
#define SPLIT_USB_TIMEOUT_POLL 10

// Serial communication for split halves
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SPLIT_POINTING_ENABLE
#define ROTARY_ENCODER_RESOLUTION 4

// ============================================================================
// USB Configuration
// ============================================================================

#define USB_POLLING_INTERVAL_MS 1

// ============================================================================
// Debounce Configuration
// ============================================================================

#define DEBOUNCE 5  // Default debounce time in ms
