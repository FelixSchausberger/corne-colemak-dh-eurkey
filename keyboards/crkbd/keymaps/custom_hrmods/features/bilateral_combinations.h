/* Copyright 2024
 * Bilateral Combinations for Home Row Mods - Header
 */

#pragma once

#include QMK_KEYBOARD_H

#ifdef BILATERAL_COMBINATIONS

// Per-key tapping term function
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record);

// Process bilateral combinations
bool process_record_bilateral(uint16_t keycode, keyrecord_t *record);

#endif // BILATERAL_COMBINATIONS
