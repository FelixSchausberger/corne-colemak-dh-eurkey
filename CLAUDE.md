# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a **custom keyboard configuration repository** for a Corne V4.1 split keyboard (46 keys), not a traditional
software development project. The repository contains firmware files, keyboard layout configurations, documentation,
and a complete development environment for a personal ergonomic typing setup using:

- **Keyboard Layout**: EurKey (European-based key layout)
- **Key Arrangement**: Colemak-DH (Matrix) for improved ergonomics
- **Firmware**: VIAL (open-source keyboard configuration tool)
- **Build System**: Nix flakes with Vial-QMK integration
- **Version Control**: Jujutsu (jj) with single main branch workflow

## Development Workflow

### Jujutsu Workflow

The project uses Jujutsu (jj) for version control with a simple single-branch workflow:

**Branch Strategy:**

- **main**: Single branch for all development and stable releases
- All work is committed directly to main
- CI ensures quality through automated validation

**Daily Workflow:**

```bash
# Work directly on main branch
jj describe -m "feat: add new feature"

# Or use AI-powered commit message (if lumen is available)
jjdescribe

# Push changes to remote main branch
jj git push
```

**Creating Feature Branches (Optional):**

For experimental features that need isolation:

```bash
jjbranch  # or jjb
# Interactive prompts:
# 1. Select type: feat, fix, chore, docs, test, refactor, perf
# 2. Enter description (lowercase, hyphens only)
# Result: Creates branch from current revision, commits with conventional format

# When ready, use jjpush to create PR back to main
jjpush
```

**Key points:**

- All work happens on `main` by default (single developer workflow)
- Feature branches (via `jjbranch`) are optional for experimental work
- Branch names follow: `type/description` (e.g., `feat/add-auto-merge`)
- Commit messages follow conventional commits: `type: description`
- Validation enforced via prek hook

### Conventional Commits Validation

Commit messages are validated automatically via prek hook.

**Valid format:**

```
type(scope?): description

Types: feat, fix, docs, style, refactor, perf, test, chore
```

**Examples:**

```
feat: add auto-merge workflow
fix: resolve jj bookmark creation issue
docs: update jujutsu workflow guide
feat(ci): optimize cachix push filter
```

## Repository Structure

```text
.
├── firmware/                           # Keyboard firmware and configuration files
│   ├── corne_v4-1_46_keebart_vial.uf2  # VIAL firmware binary for flashing
│   ├── corne_v4-1_default.vil          # Standard VIAL layout
│   ├── corne_v4-1_custom.vil           # Custom Colemak-DH layout
│   ├── corne_v4-1_custom_hrmods.vil    # Enhanced layout with home row modifiers
│   └── corne_nwwy.vil                  # Original base layout (from nwwy)
├── images/                             # Keyboard layout visualizations
│   └── nwwy/                           # Original layout images (from nwwy)
├── scripts/                            # Testing and validation tools
│   ├── validate-vial.sh                # VIAL layout validation
│   ├── test-all.sh                     # Main test runner
│   ├── test-configs.sh                 # Nix configuration validation
│   └── act-check.sh                    # Local GitHub Actions runner
├── .github/workflows/                  # CI/CD automation
│   ├── firmware-ci.yml                 # Firmware validation and building
│   └── layout-images-ci.yml            # Automatic layout image generation
├── flake.nix                           # Nix development environment
├── README.md                           # Main project documentation (includes testing guide)
├── CLAUDE.md                           # This file - Claude Code guidance
└── LICENSE                             # MIT License
```

## Key File Types & Purposes

### VIAL Firmware Files (.uf2)

- **Purpose**: Firmware binaries that get flashed directly to the keyboard
- **Usage**: Flash using bootloader mode (drag-and-drop to mounted drive)
- **Main file**: `corne_v4-1_46_keebart_vial.uf2`
- **Source**: [Keebart Corne V4.1](https://www.keebart.com/products/corne)

### VIAL Configuration Files (.vil)

- **Purpose**: Keyboard layout configurations that can be loaded in VIAL software
- **Key files**:
  - `corne_nwwy.vil` - Original base layout from community contributor
  - `corne_v4-1_default.vil` - Standard VIAL layout
  - `corne_v4-1_custom.vil` - Custom Colemak-DH layout with NixOS/programming enhancements
  - `corne_v4-1_custom_hrmods.vil` - Enhanced layout with home row modifiers (mod-tap functionality)
- **Usage**: Load these files in VIAL software to configure key mappings, macros, and layers

### Home Row Modifiers Layout

The `corne_v4-1_custom_hrmods.vil` layout includes mod-tap functionality:

- **Left hand**: A(GUI), R(Alt), S(Ctrl), T(Shift) - tap for letters, hold for modifiers
- **Right hand**: N(Shift), E(Ctrl), I(Alt), O(GUI) - tap for letters, hold for modifiers
- **Benefits**: Significantly reduced finger travel and improved ergonomics
- **Adaptation**: Requires 1-2 weeks of practice to build muscle memory

### Layout Images (.png)

- **Purpose**: Visual representations of different keyboard layers
- **Organization**: Stored in `images/nwwy/` with descriptive naming
- **Usage**: Reference images for understanding the layout design and documentation

## Development Environment

### Nix Flake Integration

The repository includes a complete Nix flake (`flake.nix`) that provides:

**Development Shell**: `nix develop`

- Complete Vial-QMK build environment
- All required dependencies (ARM toolchain, Python, QMK tools)
- VIAL GUI application
- Layout visualization tools

**Available Tools**:

- `keymap` - Layout visualization (from nixpkgs)
- `vial-to-keymap-drawer` - VIAL to YAML conversion (web version available)
- `qmk` - QMK firmware build tools
- `vial` - VIAL configuration GUI
- `wayshot` - Modern Wayland screenshot tool (Rust)
- `slurp` - Wayland region selector
- `satty` - Screenshot annotation tool

**Useful Aliases** (available in nix shell):

- `build` - Build firmware (.uf2 file)
- `flash` - Build and flash firmware
- `validate-layout` - Validate VIAL layout JSON
- `draw-layout` - Generate keymap visualizations

### Build Configuration

- **Keyboard Target**: `crkbd/rev4_1` (Corne V4.1 specific)
- **Keymap**: `vial`
- **QMK Source**: Vial-QMK fork with GUI support

## Layout Optimization

### Current Layout Structure

The custom layout (`corne_v4-1_custom_hrmods.vil`) features:

**Layer 0**: Base Colemak-DH layout with home row mods and ergonomic thumb keys (Alt left, Ctrl right)
**Layer 1**: Numbers (1-6) and navigation arrows with volume/brightness controls
**Layer 2**: Programming symbols + EurKey characters (äöüß)
**Layer 3**: Function keys (F1-F12), RGB controls, and system macros (IDLE, AREA, FULL, PSCR)
**Layers 4-14**: Empty (all KC_TRNS) reserved for future expansion

### Key Optimizations

- **Ergonomic Thumb Keys**: Ctrl on right thumb for comfortable two-hand Ctrl+C/V/Z operations
- **EurKey Integration**: AltGr access via home row mod-tap (hold I) for German characters (äöüß)
- **Programming Focus**: Optimized for Nix/Rust/Python development
- **Clear Visual Symbols**: IDLE, AREA, FULL, PSCR text labels instead of blurry emoji
- **Consistent Navigation**: Matches Helix editor keybindings (neio pattern)
- **Modern Rust Tooling**: satty for screenshots with annotation, vigiland for idle management
- **System Control Integration**: Single source of truth for volume, brightness, RGB, and system functions
- **Minimal Layer Usage**: Only L0-L3 active, layers 4-14 empty for clean visualization

## System Integration

### Modern Architecture

The keyboard layout integrates seamlessly with modern Rust-based system tools:

- **Screenshot System**: Uses satty (Rust) for unified, WM-agnostic screenshot functionality
- **Idle Management**: Direct vigiland integration without wrapper scripts for clean architecture
- **Window Management**: Consistent keybindings across niri, hyprland, and cosmic compositors

### Macro Implementation

- **M0 (IDLE)**: `sh -c 'pkill -x vigiland || vigiland &'` - Direct vigiland toggle for idle inhibition
- **M1 (AREA)**: `screenshot-region` - Region screenshot workflow (wayshot + slurp + satty)
- **M2 (FULL)**: `screenshot-full` - Fullscreen screenshot workflow (wayshot + satty)

**Note**: The macros use simple command names that can be aliased or wrapped in shell scripts for flexibility.

### Design Philosophy

- **No Redundancy**: Each system function has exactly one control location
- **Direct Commands**: No wrapper scripts - clean, maintainable architecture
- **WM Agnostic**: Same functionality works across all supported window managers

## Working with This Repository

### VIAL Workflow

1. **Development Environment**: Run `nix develop` to enter build shell
2. **Configuration**: Use VIAL GUI to modify `.vil` files
3. **Testing**: Load configurations in VIAL to test key mappings
4. **Flashing**: Flash `.uf2` firmware files to keyboard when needed
5. **Documentation**: Generate layout images when making changes

### Layout Visualization Workflow

The repository uses an **automated three-step workflow** with a custom Rust tool for generating keyboard layout images:

#### Overview

1. **VIAL → JSON** (`.vil` → `.json`): Custom Rust tool (`vil2json`) converts VIAL layouts to QMK keymap.json
2. **JSON → YAML** (`.json` → `.yaml`): Parse with keymap-drawer
3. **YAML → Images** (`.yaml` → `.svg/.pdf/.png`): Generate visualizations with layer filtering and symbol translation (automated in CI)

#### Automated Workflow (Default Behavior)

The CI automatically handles conversion when `.vil` files are pushed:

```yaml
# From .github/workflows/layout-images-ci.yml
- name: Auto-convert .vil files to JSON and YAML
  run: |
    for vil_file in firmware/*.vil; do
      vil2json "$vil_file" -o "$json_file" -m 6  # Max 6 layers
      # Apply config during parse to transform keycodes to symbols
      keymap -c keymap-drawer.yaml parse -q "$json_file" -o "$yaml_file"
    done

- name: Generate layout images with layer filtering and symbol translation
  run: |
    # Only draw first 6 layers (L0-L5) to skip empty layers
    # Use custom config for styling and glyph mapping
    keymap -c keymap-drawer.yaml draw "$yaml_file" -s L0 L1 L2 L3 L4 L5 -o "$output_svg"

    # Convert to PDF for printing
    rsvg-convert -f pdf -o "$output_pdf" "$output_svg"
```

**Benefits:**

- Automatic conversion on every `.vil` file change
- Layer filtering reduces SVG size by ~60% (137KB → 55KB)
- Symbol translation via `keymap-drawer.yaml` config (shows `!` instead of `Sft+1`, `[` instead of `LBRACKET`)
- PDF generation for printable reference sheets
- Error handling for failed conversions (continues with warnings)
- No manual intervention needed
- Reproducible builds via Nix

#### Manual Workflow (For Local Testing)

When modifying `.vil` files in VIAL:

```bash
nix develop

# Convert .vil to keymap.json using custom Rust tool
vil2json firmware/your-layout.vil -o firmware/your-layout.json

# With layer filtering and empty layer removal:
vil2json firmware/your-layout.vil -o firmware/your-layout.json -m 6 -f

# Or with explicit keyboard/layout:
vil2json firmware/your-layout.vil \
  -k crkbd/rev4_1/standard \
  -l LAYOUT_split_3x6_3_ex2 \
  -m 6 -f \
  -o firmware/your-layout.json
```

**vil2json CLI Options:**

- `-o, --output <FILE>`: Output JSON file path
- `-m, --max-layers <N>`: Limit to first N layers (default: all layers)
- `-f, --filter-empty`: Skip completely empty layers (only KC_TRNS/KC_NO)
- `-k, --keyboard <KB>`: QMK keyboard identifier (default: crkbd/rev4_1/standard)
- `-l, --layout <LAYOUT>`: Layout macro name (default: LAYOUT_split_3x6_3_ex2)

**Why a Custom Rust Tool?**

- VIAL `.vil` files use `"layout"` field with 2D matrix structure
- QMK's `qmk via2json` expects VIA backups with `"layers"` field (incompatible)
- The vial-to-keymap-drawer web tool includes `-1` matrix placeholders causing validation errors
- Rust implementation: Fast, type-safe, integrated into Nix flake
- Proper filtering of matrix placeholders and empty layers

#### Step 2: Generate YAML for Keymap-Drawer

```bash
# Parse QMK JSON to keymap-drawer YAML format with config to transform keycodes to symbols
keymap -c keymap-drawer.yaml parse -q firmware/your-layout.json -o firmware/your-layout.yaml
```

Only commit `.vil` files - CI generates `.json`, `.yaml`, and images automatically.

#### Step 3: Layer-Filtered Image Generation

```bash
# Generate SVG with only active layers (L0-L5) and symbol translation
keymap -c keymap-drawer.yaml draw firmware/your-layout.yaml \
  -s L0 L1 L2 L3 L4 L5 \
  -o images/generated/your-layout.svg

# Convert to PDF for printing (recommended for learning layouts)
rsvg-convert -f pdf -o images/generated/your-layout.pdf images/generated/your-layout.svg

# Optional: Convert to PNG for preview
convert images/generated/your-layout.svg images/generated/your-layout.png
```

**Layer Filtering Benefits:**

- Corne keyboards have 16 layers (L0-L15), but typically only 6 are used
- Filtering reduces file size by ~60%
- Improves README load times
- Keeps visualizations focused on active layers

**Symbol Translation:**

- The `keymap-drawer.yaml` config maps QMK keycodes to readable symbols using `raw_binding_map`
- **Critical**: Apply config during BOTH `parse` and `draw` steps:
  - `keymap -c keymap-drawer.yaml parse ...` - Transforms keycodes to symbols in YAML
  - `keymap -c keymap-drawer.yaml draw ...` - Applies styling and glyph mappings in SVG
- Format after `keymap parse`: `LBRACKET` (not `KC_LBRACKET`), `Sft+1` (not `LSFT(KC_1)`), `Gui+1` (not `GUI(KC_1)`), `AltGr+A` (not `RALT(KC_A)`)
- Shows actual characters: `[`, `{`, `!`, `ä` instead of internal keycodes
- Makes layouts much easier to read and understand

#### Local Preview (Optional)

For local development and preview:

```bash
nix develop

# Complete workflow with layer filtering and symbol translation
vil2json firmware/corne_v4-1_custom_hrmods.vil -m 6 -f
keymap -c keymap-drawer.yaml parse -q firmware/corne_v4-1_custom_hrmods.json \
  -o firmware/corne_v4-1_custom_hrmods.yaml
keymap -c keymap-drawer.yaml draw firmware/corne_v4-1_custom_hrmods.yaml \
  -s L0 L1 L2 L3 L4 L5 \
  -o images/generated/corne_v4-1_custom_hrmods.svg
```

#### Complete Workflow Example

```bash
nix develop

# 1. Edit layout in VIAL, save as .vil file

# 2. (Optional) Local preview with layer filtering and symbol translation
vil2json firmware/corne_v4-1_custom_hrmods.vil -m 6 -f
keymap -c keymap-drawer.yaml parse -q firmware/corne_v4-1_custom_hrmods.json \
  -o firmware/corne_v4-1_custom_hrmods.yaml
keymap -c keymap-drawer.yaml draw firmware/corne_v4-1_custom_hrmods.yaml \
  -s L0 L1 L2 L3 L4 L5 \
  -o images/generated/corne_v4-1_custom_hrmods.svg

# 3. Commit only .vil file (CI handles the rest)
jj describe -m "feat: update keymap layout"
jj git push

# Note: CI will auto-generate .json, .yaml, and filtered images
```

#### Tool Integration

The `vil2json` Rust tool is packaged in `flake.nix`:

```nix
# Rust tool: VIAL to QMK JSON converter
(pkgs.rustPlatform.buildRustPackage {
  pname = "vil2json";
  version = "0.1.0";
  src = ./tools/vil2json;
  cargoLock.lockFile = ./tools/vil2json/Cargo.lock;
})
```

Available automatically in `nix develop` shell.

#### Keymap-Drawer Configuration

The `keymap-drawer.yaml` config file provides symbol translation for better visualization:

```yaml
# Maps QMK keycodes to actual symbols
parse_config:
  raw_binding_map:
    "LSFT(KC_1)": "!" # Shows ! instead of "Sft+1"
    "KC_LBRACKET": "[" # Shows [ instead of "LBRACKET"
    "RALT(KC_A)": "ä" # Shows ä for EurKey
    # ... and more
```

**Purpose:**

- Translates QMK keycodes like `LSFT(KC_1)` to readable symbols `!`
- Makes generated SVG images show actual glyphs instead of technical names
- Improves layout visualization for documentation and understanding

**What it translates:**

- Shifted numbers: `! @ # $ % ^ & * ( )`
- Brackets: `[ ] { }`
- Math symbols: `+ - = _`
- Pipes and slashes: `| \ / ?`
- Quotes: `' " \` ~`
- EurKey characters: `ä ö ü ß`

The config is automatically used by CI and can be manually used with `-c keymap-drawer.yaml` flag.

### Common Tasks

- **Modify key layout**: Edit `.vil` files using VIAL software
- **Add new layer**: Use VIAL to create additional keyboard layers
- **Flash firmware**: Use `.uf2` files with keyboard in bootloader mode
- **Update images**: Automatic via GitHub Actions (or manual workflow for local preview)
- **Build firmware**: Use `build` command in development shell
- **Run tests**: Use `./scripts/test-all.sh` for comprehensive validation
- **Code quality**: Use `prek run --all-files` (or `--fail-fast` for CI) in nix shell
- **Prek maintenance**: Use `prek cache dir` to view cache location for cleanup/debugging

#### Modern Tool Usage

- **Screenshots with Annotation**: Use modern Rust-based tools for efficient capture

  - Region screenshot: `wayshot -s "$(slurp)" --stdout | satty --filename - --fullscreen`
  - Full screenshot: `wayshot --stdout | satty --filename - --fullscreen`
  - Direct save: `wayshot -s "$(slurp)" -f ~/Pictures/screenshot-$(date '+%Y%m%d-%H:%M:%S').png`

- **RGB Configuration**: Use VIAL GUI for all RGB and lighting control

  - Open VIAL: `vial` (included in development environment)
  - Configure RGB Matrix effects, colors, and brightness through GUI
  - Per-layer RGB requires custom QMK firmware modifications

- **Idle Management**: Simple vigiland usage for Wayland idle inhibition
  - Start inhibiting: `vigiland` (ctrl+c to stop)
  - Background toggle: `pkill -x vigiland || vigiland &` (used in keyboard macro M0)

### Tools Required

- **Nix**: For reproducible development environment
- **VIAL**: Main configuration software (included in flake)
- **Bootloader access**: For flashing firmware files
- **Web browser**: For vial-to-keymap-drawer conversion
- **Docker**: For local GitHub Actions testing (optional)
- **prek**: Code quality and pre-commit hooks (included in flake)

## RGB Configuration

### Current Capabilities

- **VIAL GUI Support**: Basic RGB Matrix control (effects, colors, brightness)
- **Per-Layer RGB**: Limited support (requires custom QMK code for advanced features)
- **RGB Matrix**: Suitable for per-key RGB and underglow combination

### Implementation Notes

- Advanced per-layer RGB colors require custom QMK firmware modifications
- VIAL provides good basic RGB control but VIA has more advanced lighting layers
- RGB lighting layers can be implemented via QMK's `rgb_matrix_indicators_user` function

## Community Contributions

This project builds upon excellent work from the keyboard community:

- **Base Layout**: Original `corne_nwwy.vil` and layer images from [nwwy](https://www.reddit.com/user/nwwy/)
- **Firmware**: Vial-QMK firmware from the VIAL project
- **Hardware**: Corne V4.1 keyboard design by foostan
- **Visualization**: keymap-drawer and vial-to-keymap-drawer tools

## Development Notes

This repository represents a hardware configuration project rather than traditional software development.
The "development" process involves:

- Keyboard layout design and optimization through VIAL GUI
- Firmware building using the Nix development environment
- Layout visualization and documentation using keymap-drawer
- Integration with NixOS system configuration for seamless workflow
- Comprehensive testing using the scripts in the `scripts/` directory
- Code quality assurance through prek hooks

All development is reproducible through the Nix flake, ensuring consistent build environments and dependency management.

### Testing Philosophy

The project emphasizes thorough testing and validation:

- **Layout validation**: Ensures VIAL files are syntactically correct
- **Configuration validation**: Verifies Nix flake integrity
- **CI/CD testing**: Local GitHub Actions testing with act
- **Code quality**: Automated formatting and linting with prek

All testing tools are documented in the main README.md under the "Testing & Validation" section.

## CI/CD Automation

### GitHub Actions Workflows

#### Firmware CI/CD (`firmware-ci.yml`)

- **Purpose**: Validates and builds keyboard firmware
- **Triggers**: Changes to `firmware/`, `flake.nix`, or workflow files
- **Process**: Layout validation, flake check, firmware compilation, artifact generation
- **Outputs**: `.uf2` and `.hex` firmware files, build reports, PR comments

#### Layout Images CI/CD (`layout-images-ci.yml`)

- **Purpose**: Automatically generates layout visualization images from VIAL files
- **Triggers**: Changes to `firmware/*.vil` files or manual dispatch
- **Process**:
  1. **Auto-conversion**: Converts `.vil` → `.json` → `.yaml` using `vil2json` and `keymap parse`
  2. **Layer Filtering**: Generates images with only active layers (L0-L5) for 60% size reduction
  3. **Image Generation**: Creates `.svg` and `.png` files using keymap-drawer in Nix environment
  4. **Auto-commit**: Commits generated images to `images/generated/` directory (main branch only)
  5. **PR Integration**: Comments with generation results and artifact links
- **Benefits**:
  - Fully automated workflow (no manual conversion needed)
  - Layer filtering for smaller, faster-loading images
  - Reproducible builds via Nix
  - Type-safe Rust tool for reliable conversion

**Implementation Details:**

- Custom Rust tool (`vil2json`) handles VIAL to QMK JSON conversion
- Filters `-1` matrix placeholders and empty layers
- Supports layer limiting (`-m 6`) and empty layer filtering (`-f`)
- Integrated into Nix flake for consistent builds across local/CI

### Automation Architecture

- **Reproducible Builds**: Nix flakes ensure consistent environments across local/CI
- **Dependency Management**: All tools (QMK, VIAL, keymap-drawer) managed via Nix
- **Security**: Minimal permissions for core functionality
- **Efficiency**: Smart change detection, parallel workflows, artifact caching
