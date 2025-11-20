/* Copyright 2024
 * Bilateral Combinations for Home Row Mods
 *
 * This prevents accidental same-hand modifier combinations.
 * For example, pressing S (Ctrl) + T (Shift) on the left hand won't
 * trigger Ctrl+Shift unless you intended it.
 *
 * Cross-hand combinations (left Ctrl + right key) work normally.
 */

#include QMK_KEYBOARD_H

#ifdef BILATERAL_COMBINATIONS

// Track which hand's mods are currently pressed
static uint8_t left_mods_down = 0;
static uint8_t right_mods_down = 0;

// Define which keys are on which hand (for Corne split keyboard)
// Row 0-2 are left hand, Row 3-5 are right hand (in split configuration)
static bool is_left_hand_key(keyrecord_t *record) {
    // For split keyboards, check if the key is on the left half
    return record->event.key.row < MATRIX_ROWS / 2;
}

// Per-key tapping term configuration
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Home row mods get slightly longer tapping term
        case MT(MOD_LGUI, KC_A):
        case MT(MOD_LALT, KC_R):
        case MT(MOD_LCTL, KC_S):
        case MT(MOD_LSFT, KC_T):
        case MT(MOD_RSFT, KC_N):
        case MT(MOD_RCTL, KC_E):
        case MT(MOD_RALT, KC_I):
        case MT(MOD_RGUI, KC_O):
            return TAPPING_TERM + 25;  // 200ms for home row mods
        default:
            return TAPPING_TERM;  // 175ms for everything else
    }
}

// Check if a keycode is a home row mod-tap
static bool is_home_row_mod(uint16_t keycode) {
    switch (keycode) {
        case MT(MOD_LGUI, KC_A):
        case MT(MOD_LALT, KC_R):
        case MT(MOD_LCTL, KC_S):
        case MT(MOD_LSFT, KC_T):
        case MT(MOD_RSFT, KC_N):
        case MT(MOD_RCTL, KC_E):
        case MT(MOD_RALT, KC_I):
        case MT(MOD_RGUI, KC_O):
            return true;
        default:
            return false;
    }
}

// Process bilateral combinations logic
bool process_bilateral_combinations(uint16_t keycode, keyrecord_t *record) {
    // Only process home row mods
    if (!is_home_row_mod(keycode)) {
        return true;  // Continue normal processing
    }

    bool is_left = is_left_hand_key(record);

    if (record->event.pressed) {
        // Key pressed - track which hand's mods are down
        if (is_left) {
            left_mods_down++;
        } else {
            right_mods_down++;
        }

        // If both hands have mods pressed simultaneously, this is likely intentional
        // (e.g., left Ctrl + right Shift for a cross-hand shortcut)
        // Allow it to proceed normally
        if (left_mods_down > 0 && right_mods_down > 0) {
            return true;
        }

        // If this is a same-hand combination (e.g., S+T both on left hand)
        // and they're pressed very close together, treat it as accidental
        if (is_left && left_mods_down > 1) {
            // Check if another mod from the same hand is already held
            // If so, this might be accidental - prevent mod activation
            // by treating it as a tap instead
            return false;  // Block the mod, treat as tap
        }

        if (!is_left && right_mods_down > 1) {
            return false;  // Block the mod, treat as tap
        }

    } else {
        // Key released - update tracking
        if (is_left) {
            left_mods_down = (left_mods_down > 0) ? left_mods_down - 1 : 0;
        } else {
            right_mods_down = (right_mods_down > 0) ? right_mods_down - 1 : 0;
        }
    }

    return true;  // Continue normal processing
}

// Hook into QMK's process_record_user
// This should be called from keymap.c's process_record_user function
bool process_record_bilateral(uint16_t keycode, keyrecord_t *record) {
    return process_bilateral_combinations(keycode, record);
}

#endif // BILATERAL_COMBINATIONS
