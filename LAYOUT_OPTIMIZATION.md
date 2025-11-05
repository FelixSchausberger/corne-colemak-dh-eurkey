# Layout Optimization Suggestions

## Current Issues Identified

### Layer 4 (L4) - Window Management

#### Problem 1: Plain Letters Instead of GUI Combos (Left Side Middle Row)
**Current**: `N, E, I, O` (plain letters)
**Expected**: `Super+N, Super+E, Super+I, Super+O` (window focus)

The VIAL file shows `KC_N, KC_E, KC_I, KC_O` - these are **plain letter keys**, not GUI combinations. This is likely a configuration mistake.

#### Problem 2: Redundant Workspace Switching
You have workspace switching in two places:
- **L4 left top**: `Super+1-6` → Switch to workspace 1-6
- **Number keys** (any layer with numbers): Can do `Super+1` directly

**Optimization**: Remove dedicated workspace switch keys from L4 since you can use `Super+number` from any layer.

#### Problem 3: Confusing Navigation Layout
- **Left side middle**: Plain `N,E,I,O` (should be `Super+NEIO` for focus)
- **Left side bottom**: `Shift+Super+NEIO` (move window + focus combined)
- **Right side middle**: `Super+NEIO` (focus only, reversed order)
- **Right side top**: `Shift+Super+1-6` (move to workspace, reversed order)

The reversed order (right side) is confusing and the duplication of navigation makes it harder to remember.

### Layer 5 (L5) - System Controls
Currently very sparse - only has:
- M0, M1, M2 (vigiland, screenshots)
- Print screen keys
- Everything else is transparent

**Optimization**: Could be combined with L4 to create one comprehensive "system + window management" layer.

## Recommended Optimizations

### Option 1: Simplified Single Layer Approach

**Combine L4 + L5 into one "System & Window Management" layer:**

```
Layer 4 (New Combined):
┌─────────────────────────────────────────────┐
│ Left Side                                    │
│ Top:    M0     M1     M2     PrtSc  ___     │  System controls
│ Middle: ___    ___    ◀ F    ▼ F    ▲ F     │  Focus window (Super+NEIO)
│ Bottom: ___    ___    ⬅ M    ⬇ M    ⬆ M     │  Move window (Shift+Super+NEIO)
│                                              │
│ Right Side                                   │
│ Top:    ___    ___    →Move  ←Focus ___     │  Right-handed shortcuts
│ Middle: ➊ M    ➋ M    ➌ M    ➍ M    ➎ M     │  Move to workspace 1-5
│ Bottom: ___    ___    ___    ___    ___     │
└─────────────────────────────────────────────┘

Legend:
F = Focus window in direction (Super+NEIO)
M = Move window in direction (Shift+Super+NEIO)
➊ M = Move window to workspace (Shift+Super+1-5)
```

**Rationale**:
- Keep most-used actions on dominant hand (right for most users)
- Group related functions (window management together, system controls together)
- Remove redundant workspace switching (use `Super+number` from number layer)
- Fix plain letter bug by using proper Super+NEIO combos
- Consistent directional layout (no reversed order confusion)

### Option 2: Keep L4/L5 Separate but Optimize

**Layer 4: Window Management Only**
```
Left Side:
Top:    ___    ___    ___    ___    ___
Middle: ___    ___    ◀      ▼      ▲         Focus window (Super+NEIO)
Bottom: ___    ___    ⬅      ⬇      ⬆         Move window (Shift+Super+NEIO)

Right Side:
Top:    ___    ___    ___    ___    ▶         Focus right
Middle: ➊      ➋      ➌      ➍      ➎         Move to workspace 1-5
Bottom: ___    ___    ___    ___    ➡         Move right
```

**Layer 5: System Controls**
```
Left Side:
Top:    M0     M1     M2     PrtSc  S+PrtSc
Middle: ___    ___    ___    ___    ___
Bottom: ___    ___    ___    ___    ___

(Keep L5 sparse for future system controls)
```

### Option 3: Ergonomic Split by Hand

**Layer 4: Left-Hand Window Management**
```
Left Side:
Top:    ➊      ➋      ➌      ➍      ➎         Move to workspace
Middle: ___    ___    ◀      ▼      ▲         Focus window
Bottom: ___    ___    ⬅      ⬇      ⬆         Move window

Right Side:
(transparent or mirror for symmetry)
```

**Layer 5: Right-Hand System Controls**
```
Right Side:
Top:    M0     M1     M2     PrtSc  ___
Middle: ___    ___    ___    ___    ▶         Focus right
Bottom: ___    ___    ___    ___    ➡         Move right
```

## Implementation Steps

1. **Open VIAL**: Load `firmware/corne_v4-1_custom_hrmods.vil`

2. **Fix the bug** (Layer 4, left middle row):
   - Change `KC_N` → `LGUI(KC_N)` (or remove if optimizing)
   - Change `KC_E` → `LGUI(KC_E)`
   - Change `KC_I` → `LGUI(KC_I)`
   - Change `KC_O` → `LGUI(KC_O)`

3. **Choose optimization approach**:
   - Option 1: Most space-efficient, best for minimal layer count
   - Option 2: Keeps logical separation, easier mental model
   - Option 3: Most ergonomic, hand-specific optimizations

4. **Test the new layout**:
   - Load in VIAL
   - Flash to keyboard
   - Use for a few days to build muscle memory
   - Iterate based on actual usage patterns

## My Recommendation: Option 1

**Why**: You have a 46-key Corne with 6 layers. Consolidating L4+L5 into one layer:
- **Frees up a layer** for future functionality
- **Reduces cognitive load** (one less layer to remember)
- **Groups related functions** (all window/system management in one place)
- **Fixes the plain letter bug** in a natural way
- **Removes redundancy** (no duplicate workspace switching)

The consolidated layer puts:
- **System controls** (macros, screenshots) on left thumb row (easy to hit)
- **Window focus** on left main area (Colemak-DH NEIO positions)
- **Window movement** on left bottom (Shift+Super+NEIO)
- **Workspace movement** on right side (numbered for clarity)

This matches the **single-hand window management** pattern used in tiling WMs like i3, sway, niri.

## Questions to Consider

1. **How often do you switch workspaces vs move windows between workspaces?**
   - If switching is more common: Keep workspace numbers prominent
   - If moving is more common: Prioritize Shift+Super+numbers

2. **Do you use window focus navigation more than window movement?**
   - If yes: Put Super+NEIO in easier-to-reach positions
   - If no: Prioritize Shift+Super+NEIO placement

3. **Do you need both screenshot methods (M1, M2)?**
   - Region + Full screen are both useful
   - Consider adding window screenshot if needed

4. **What other system controls would you want?**
   - Volume/brightness (currently in L1)
   - Media controls (currently in combos)
   - Lock screen, sleep, etc.

Feel free to modify these suggestions based on your actual usage patterns!
