# Custom Corne V4.1 Keymap with RGB Indicators & Home Row Mods

This is a custom QMK/VIAL keymap for the Corne V4.1 keyboard featuring:

## Features

### 1. Per-Layer RGB Indicators (Active Keys Only)
- **Layer 0 (Base)**: White - Shows only letter keys and active modifiers
- **Layer 1 (Numbers)**: Orange - Highlights numbers and navigation arrows
- **Layer 2 (Symbols)**: Yellow - Shows programming symbols and EurKey characters
- **Layer 3 (Function)**: Red - Displays function keys and system controls

**How it works**: Only keys that are assigned on the current layer are lit up. Transparent (`KC_TRNS`) and disabled (`KC_NO`) keys remain dark, making it easy to see which keys are available.

### 2. Home Row Mods with Anti-Accidental Activation

**Left Hand:**
- A = GUI (hold)
- R = Alt (hold)
- S = Ctrl (hold)
- T = Shift (hold)

**Right Hand:**
- N = Shift (hold)
- E = Ctrl (hold)
- I = Alt (hold)
- O = GUI (hold)

**Protection Features:**
- **Bilateral Combinations**: Prevents accidental same-hand mod combos (e.g., S+T won't trigger Ctrl+Shift unintentionally)
- **Tuned Timing**: 200ms tapping term for home row mods (175ms for other keys)
- **PERMISSIVE_HOLD**: Mod activates immediately when another key is pressed, allowing fast typing

This solves the issue where typing "st" quickly would accidentally trigger Ctrl+T (new tab in browsers).

### 3. System Integration Macros

- **M0 (IDLE)**: Toggle vigiland idle inhibitor - `pkill -x vigiland || vigiland &`
- **M1 (AREA)**: Region screenshot - `screenshot-region`
- **M2 (FULL)**: Full screenshot - `screenshot-full`

### 4. Tap Dance Keys

- **TD(0)**: Esc / Ctrl
- **TD(1)**: Delete / Shift
- **TD(2)**: Escape / GUI
- **TD(3)**: M2 (Full Screenshot) / Print Screen

## Layout

The layout is based on Colemak-DH with EurKey support for German characters (äöüß).

### Layer Structure

- **Layer 0**: Base Colemak-DH layout with home row mods
- **Layer 1**: Numbers (1-6 on left, 7-0 on right) and navigation arrows
- **Layer 2**: Programming symbols + EurKey characters (äöüß)
- **Layer 3**: Function keys (F1-F12), RGB controls, system macros

## Building

The firmware is built automatically via Nix:

```bash
nix develop
build  # Builds firmware with custom keymap
flash  # Builds and flashes to keyboard
```

The custom keymap is automatically copied to the vial-qmk source before building.

## Configuration Files

- `keymap.c` - Main keymap definition with RGB indicators
- `config.h` - Home row mod timing and RGB configuration
- `rules.mk` - Feature flags (VIAL, RGB Matrix, Tap Dance, etc.)
- `vial.json` - VIAL GUI layout configuration
- `features/bilateral_combinations.c` - Anti-accidental mod activation logic

## Technical Details

### RGB Matrix Implementation

The RGB indicator system uses QMK's `rgb_matrix_indicators_advanced_user()` function to:
1. Detect the current layer
2. Check each LED's corresponding key
3. Light up only active keys (non-transparent, non-disabled)
4. Use warm gradient colors (white → orange → yellow → red)

### Home Row Mod Protection

The bilateral combinations system tracks which hand's modifiers are pressed:
- Same-hand combinations pressed quickly are blocked (treated as taps)
- Cross-hand combinations work normally
- Both hands pressing mods simultaneously is allowed (intentional shortcuts)

## Credits

- Base layout: Exported from VIAL configuration (`corne_v4-1_custom_hrmods.vil`)
- RGB indicator inspiration: [sunaku's implementation](https://github.com/sunaku/vial-qmk/blob/70af7e/keyboards/ergohaven/remnant/keymaps/sunaku/keymap_footer.c#L16)
- Home row mod best practices: QMK community documentation

## License

GPL v2 or later (same as QMK)
