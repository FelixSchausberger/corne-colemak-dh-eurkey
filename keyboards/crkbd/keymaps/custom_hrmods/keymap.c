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

#include QMK_KEYBOARD_H
#include "features/bilateral_combinations.h"

// Layer definitions
enum layers {
    _BASE = 0,
    _NUM,
    _SYM,
    _FN
};

// Custom keycodes
enum custom_keycodes {
    M_IDLE = SAFE_RANGE,
    M_AREA,
    M_FULL
};

// Tap Dance declarations
enum {
    TD_ESC_CTRL = 0,
    TD_DEL_SHFT,
    TD_ESC_GUI,
    TD_MACRO_PSCR
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_CTRL] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_LCTL),
    [TD_DEL_SHFT] = ACTION_TAP_DANCE_DOUBLE(KC_DEL, KC_LSFT),
    [TD_ESC_GUI] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_LGUI),
    [TD_MACRO_PSCR] = ACTION_TAP_DANCE_DOUBLE(M_FULL, KC_PSCR)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Base (Colemak-DH with home row mods)
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐               ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │ Tab │  Q  │  W  │  F  │  P  │  B  │               │ Esc │ TD2 │  J  │  L  │  U  │  Y  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │ ;:  │BKSPC│LSPO │GUI/A│ALT/R│CTL/S│               │SFT/T│  G  │RCPC │ Alt │  M  │SFT/N│
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │CTL/E│ALT/I│GUI/O│ TD1 │LGui │  Z  │               │  X  │  C  │  D  │  V  │  K  │  H  │
     * └─────┴─────┴─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┴─────┴─────┘
     *                   │ ,   │  .  │  /  │               │  '  │LCtrl│MO(1)│
     *                   └─────┴─────┴─────┘               └─────┴─────┴─────┘
     *                                 ┌─────┬─────┐ ┌─────┬─────┐
     *                                 │Space│SftEn││ │MO(2)│RCtrl│
     *                                 └─────┴─────┘ └─────┴─────┘
     */
    [_BASE] = LAYOUT_split_3x6_3_ex2(
        KC_TAB,  KC_Q,            KC_W,            KC_F,            KC_P,            KC_B,     KC_GESC, TD(TD_ESC_GUI), KC_J,    KC_L,            KC_U,            KC_Y,            KC_SCLN,         KC_BSPC,
        KC_LSPO, MT(MOD_LGUI,KC_A), MT(MOD_LALT,KC_R), MT(MOD_LCTL,KC_S), MT(MOD_LSFT,KC_T), KC_G,     KC_RCPC, KC_RALT,        KC_M,    MT(MOD_RSFT,KC_N), MT(MOD_RCTL,KC_E), MT(MOD_RALT,KC_I), MT(MOD_RGUI,KC_O), TD(TD_DEL_SHFT),
        KC_LGUI, KC_Z,            KC_X,            KC_C,            KC_D,            KC_V,     KC_K,    KC_H,           KC_COMM, KC_DOT,          KC_SLSH,         KC_QUOT,
                                                   KC_LCTL,         MO(_NUM),        KC_SPC,   KC_SFTENT, MO(_SYM),     KC_RCTL
    ),

    /* Layer 1: Numbers & Navigation
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐               ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │     │  1  │  2  │  3  │  4  │  5  │               │     │     │  6  │  7  │  8  │  9  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │  0  │BKSPC│     │     │     │     │               │     │     │     │     │ Left│ Down│
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │ Up  │Right│     │     │     │     │               │ TG3 │     │     │     │     │     │
     * └─────┴─────┴─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┴─────┴─────┘
     *                   │     │     │     │               │     │     │     │
     *                   └─────┴─────┴─────┘               └─────┴─────┴─────┘
     *                                 ┌─────┬─────┐ ┌─────┬─────┐
     *                                 │LGui │     │ │Space│SftEn│
     *                                 └─────┴─────┘ └─────┴─────┘
     *                                            ┌─────┬─────┐
     *                                            │     │RGui │
     *                                            └─────┴─────┘
     */
    [_NUM] = LAYOUT_split_3x6_3_ex2(
        KC_TRNS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_TRNS, KC_TRNS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_BSPC,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_TRNS, KC_TRNS, KC_NO,   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_NO,
        KC_TRNS, KC_NO,   KC_NO,   TG(_FN), KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                    KC_LGUI, KC_TRNS, KC_SPC,   KC_SFTENT, KC_TRNS, KC_RGUI
    ),

    /* Layer 2: Symbols & Programming
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐               ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │ Tab │  !  │  @  │  #  │  $  │  %  │               │LCtrl│RCtrl│  *  │  "  │  ü  │  &  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │  ^  │BKSPC│LCtrl│  ä  │  ß  │  ö  │               │     │     │LAlt │ Alt │  -  │  =  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │  ]  │  [  │  ö  │  `  │LShft│  |  │               │  ?  │     │     │     │  _  │  +  │
     * └─────┴─────┴─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┴─────┴─────┘
     *                   │  }  │  {  │  \  │               │  ~  │LGui │     │
     *                   └─────┴─────┴─────┘               └─────┴─────┴─────┘
     *                                 ┌─────┬─────┐ ┌─────┬─────┐
     *                                 │Space│SftEn│ │     │RGui │
     *                                 └─────┴─────┘ └─────┴─────┘
     */
    [_SYM] = LAYOUT_split_3x6_3_ex2(
        KC_TAB,  LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5),  KC_LCTL,   KC_RCTL,      LSFT(KC_8), LSFT(KC_QUOT), RALT(KC_U), LSFT(KC_7), LSFT(KC_6), KC_BSPC,
        KC_LCTL, RALT(KC_A), RALT(KC_0), RALT(KC_S), KC_NO,      KC_NO,       KC_LALT,   KC_RALT,      KC_MINS,    KC_EQL,        KC_RBRC,    KC_LBRC,    RALT(KC_O), KC_GRV,
        KC_LSFT, LSFT(KC_BSLS), LSFT(KC_SLSH), KC_NO, KC_NO,   KC_NO,       LSFT(KC_MINS), LSFT(KC_EQL), LSFT(KC_RBRC), LSFT(KC_LBRC), KC_BSLS, LSFT(KC_GRV),
                                              KC_LGUI, KC_TRNS, KC_SPC,      KC_SFTENT, KC_TRNS,      KC_RGUI
    ),

    /* Layer 3: Function & System
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐               ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │Reset│ F1  │ F2  │ F3  │ F4  │ F5  │               │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │ F12 │ TD3 │RGB T│RGB H│RGB S│RGB V│               │IDLE │     │     │BriDn│BriUp│Mute │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │Vol- │Vol+ │PgUp │Home │RGB M│RGB h│               │RGB s│RGB v│FULL │PScr │     │     │
     * └─────┴─────┴─────┼─────┼─────┼─────┤               ├─────┼─────┼─────┼─────┴─────┴─────┘
     *                   │     │     │PgDn │               │ End │LGui │     │
     *                   └─────┴─────┴─────┘               └─────┴─────┴─────┘
     *                                 ┌─────┬─────┐ ┌─────┬─────┐
     *                                 │Space│SftEn│ │     │RGui │
     *                                 └─────┴─────┘ └─────┴─────┘
     */
    [_FN] = LAYOUT_split_3x6_3_ex2(
        QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TD(TD_MACRO_PSCR),
        RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, M_IDLE,  KC_NO,    KC_NO,   KC_BRID, KC_BRIU, KC_MUTE, KC_VOLD, KC_VOLU, KC_PGUP, KC_HOME,
        RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, M_FULL,  KC_PSCR,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PGDN, KC_END,
                                    KC_LGUI, KC_TRNS, KC_SPC,   KC_SFTENT, KC_TRNS, KC_RGUI
    )
};

// Process custom keycodes (macros)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef BILATERAL_COMBINATIONS
    // Process bilateral combinations for home row mods
    if (!process_record_bilateral(keycode, record)) {
        return false;
    }
#endif

    switch (keycode) {
        case M_IDLE:  // Toggle vigiland idle inhibitor
            if (record->event.pressed) {
                SEND_STRING("pkill -x vigiland || vigiland &" SS_TAP(X_ENTER));
            }
            return false;
        case M_AREA:  // Region screenshot
            if (record->event.pressed) {
                SEND_STRING("screenshot-region" SS_TAP(X_ENTER));
            }
            return false;
        case M_FULL:  // Full screenshot
            if (record->event.pressed) {
                SEND_STRING("screenshot-full" SS_TAP(X_ENTER));
            }
            return false;
    }
    return true;
}

#ifdef RGB_MATRIX_ENABLE

// RGB color definitions for warm gradient scheme
// Layer 0 (Base): White (255, 255, 255)
// Layer 1 (Numbers): Orange (255, 165, 0)
// Layer 2 (Symbols): Yellow (255, 255, 0)
// Layer 3 (Function): Red (255, 0, 0)

// Helper function to determine if a key is active on the current layer
bool is_key_active(uint8_t layer, uint8_t row, uint8_t col) {
    uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col, row});

    // Transparent keys are not active on this layer
    if (keycode == KC_TRNS) {
        return false;
    }

    // KC_NO keys are explicitly disabled
    if (keycode == KC_NO) {
        return false;
    }

    // All other keys are active
    return true;
}

// Main RGB indicator function - called on every matrix scan
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);

    // Define colors for each layer (warm gradient scheme)
    RGB layer_colors[4] = {
        {255, 255, 255},  // Layer 0: White (base letters)
        {255, 165, 0},    // Layer 1: Orange (numbers/nav)
        {255, 255, 0},    // Layer 2: Yellow (symbols)
        {255, 0, 0}       // Layer 3: Red (function keys)
    };

    // Only highlight if we're on layers 0-3
    if (layer > 3) {
        return false;
    }

    RGB current_color = layer_colors[layer];

    // Iterate through all LEDs
    for (uint8_t led = led_min; led < led_max; led++) {
        // Get the position of the LED
        uint8_t row = g_led_config.matrix_co[led][0];
        uint8_t col = g_led_config.matrix_co[led][1];

        // Skip if this LED doesn't correspond to a key
        if (row == NO_LED || col == NO_LED) {
            continue;
        }

        // Check if this key is active on the current layer
        if (is_key_active(layer, row, col)) {
            // Set the LED to the layer color
            rgb_matrix_set_color(led, current_color.r, current_color.g, current_color.b);
        } else {
            // Turn off inactive keys
            rgb_matrix_set_color(led, 0, 0, 0);
        }
    }

    return false;
}

#endif // RGB_MATRIX_ENABLE
