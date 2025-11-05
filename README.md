# Custom Keyboard Configuration: Corne V4.1 Colemak-DH (EurKey)

## Overview

This repository documents my journey into ergonomic typing with a [Corne V4.1 (46 keys)](https://github.com/foostan/crkbd/)
split keyboard. It's a personal project focused on optimizing typing efficiency and comfort using modern tooling and
reproducible development environments.

### Configuration Details

- **Keyboard Layout**: [EurKey](https://eurkey.steffen.bruentjen.eu/) (European character support)
- **Key Arrangement**: [Colemak-DH (Matrix)](https://colemakmods.github.io/mod-dh/) (ergonomic optimization)
- **Firmware**: [VIAL](https://get.vial.today/) (open-source keyboard configuration)
- **Development Environment**: [Nix Flakes](https://nixos.wiki/wiki/Flakes) (reproducible builds)
- **Version Control**: [Jujutsu](https://github.com/martinvonz/jj) (modern VCS with dev-main workflow)

## Current Keyboard Layout

The layout visualization below is **automatically updated** from the main branch. It always shows the latest version of the `corne_v4-1_custom_hrmods.vil` layout (Colemak-DH with home row modifiers):

![Keyboard Layout](https://raw.githubusercontent.com/FelixSchausberger/corne-colemak-dh-eurkey/main/images/generated/corne_v4-1_custom_hrmods.svg)

> **Note**: Images are automatically generated via GitHub Actions when YAML layout files are updated. See the [Layout Visualization](#layout-visualization--image-generation) section for details.

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
│   ├── test-*.sh                       # Testing infrastructure
│   └── act-check.sh                    # Local GitHub Actions runner
├── .github/workflows/                  # CI/CD automation
│   ├── firmware-ci.yml                 # Firmware validation and building
│   └── layout-images-ci.yml            # Automatic layout image generation
├── flake.nix                           # Nix development environment
├── README.md                           # This file
├── CLAUDE.md                           # Claude Code guidance
└── LICENSE                             # MIT License
```

## Quick Start

### Prerequisites

- NixOS or Nix package manager
- Flakes enabled in your Nix configuration

### Development Environment

```bash
# Enter the development environment
nix develop

# Available commands in the shell:
build            # Build firmware (.uf2 file)
flash            # Build and flash firmware
validate-layout  # Validate VIAL layout JSON
keymap           # Generate layout images from YAML files
wayshot          # Modern Wayland screenshot tool (Rust)
slurp            # Wayland region selector
vial             # Open VIAL GUI for layout configuration
```

### Layout Modification Workflow

1. **Open VIAL**: Launch VIAL GUI (`vial` command in nix shell)
2. **Load Layout**: Import one of the `.vil` files from `firmware/`
3. **Modify Layout**: Use VIAL's visual editor to customize key mappings
4. **Export Layout**: Save your modifications as a new `.vil` file
5. **Generate Images**: Follow the layout visualization workflow below
6. **Update Documentation**: Commit your changes with updated images

## Development Workflow

### Jujutsu (jj) Version Control

This repository uses Jujutsu (jj) for version control with a **single main branch workflow**:

**Branch Strategy:**

- **main** - Single branch for all development and stable releases
- All work is committed directly to main
- CI ensures quality through automated validation

**Daily Workflow:**

```bash
# Work directly on main branch
jj describe -m "feat: add new macro for screenshot workflow"

# Or use AI-powered commit message (if lumen is available)
jjdescribe

# Push changes to remote main branch
jj git push
```

**Creating Feature Branches (Optional):**

For experimental features that need isolation:

```bash
# Create feature branch with interactive prompts
jjbranch  # or jjb

# Work on your feature...
jj describe -m "feat(experimental): test new layer design"

# Push and create PR back to dev
jjpush
```

**Merging to Main:**

When dev branch is stable and tested:

1. Create PR from dev → main via GitHub UI
2. Wait for CI checks to pass
3. Merge when ready for production release

**Key Points:**

- Daily work happens on `dev` branch
- `main` only receives tested, stable changes
- Commit messages follow conventional commits: `type: description`
- Types: feat, fix, docs, style, refactor, perf, test, chore
- Validation enforced via prek hooks

**Useful Commands:**

```bash
jj log              # View commit history
jj diff             # Show changes
jj bookmark list    # List all branches
jj status           # Show working copy status
```

For comprehensive workflow documentation and helper functions (jjbranch, jjdescribe, jjpush), see [CLAUDE.md](CLAUDE.md#development-workflow).

## Layout Visualization & Image Generation

This repository uses an **automated three-step workflow** for generating keyboard layout images:

### Workflow Overview

1. **VIAL → JSON** (`.vil` → `.json`): Custom Rust tool converts VIAL layouts to QMK keymap.json
2. **JSON → YAML** (`.json` → `.yaml`): Parse with keymap-drawer
3. **YAML → Images** (`.yaml` → `.svg/.png`): Generate visualizations with layer filtering (automated in CI)

### Automated Workflow (Recommended)

The simplest approach is to **let GitHub Actions handle everything automatically**:

1. Edit your layout in VIAL and save as `.vil` file
2. Commit the `.vil` file to `firmware/` directory
3. Push to GitHub
4. CI automatically generates `.json`, `.yaml`, and images with only active layers (L0-L5)

**Benefits:**

- No manual conversion needed
- Automatic layer filtering (60% smaller SVG files)
- Reproducible builds via Nix
- Images always stay in sync with layouts

### Manual Workflow (Advanced)

For local preview or custom conversion:

```bash
nix develop

# Convert .vil to keymap.json using custom Rust tool
vil2json firmware/your-layout.vil -o firmware/your-layout.json

# Optional: Limit to first 6 layers and filter empty ones
vil2json firmware/your-layout.vil -o firmware/your-layout.json -m 6 -f

# Generate YAML from JSON
keymap parse -q firmware/your-layout.json -o firmware/your-layout.yaml

# Generate SVG with layer filtering (only L0-L5)
keymap draw firmware/your-layout.yaml -s L0 L1 L2 L3 L4 L5 \
  -o images/generated/your-layout.svg

# Optional: Convert to PNG
convert images/generated/your-layout.svg images/generated/your-layout.png
```

**vil2json Options:**

- `-m, --max-layers <N>`: Limit to first N layers (e.g., `-m 6` for L0-L5)
- `-f, --filter-empty`: Skip completely empty layers (only KC_TRNS/KC_NO)
- `-k, --keyboard <KB>`: Set QMK keyboard (default: crkbd/rev4_1/standard)
- `-l, --layout <LAYOUT>`: Set layout macro (default: LAYOUT_split_3x6_3_ex2)

### Why This Approach?

**Custom Rust Tool (vil2json):**

- VIAL `.vil` files are incompatible with `qmk via2json` (different JSON structure)
- The vial-to-keymap-drawer web tool includes matrix placeholders causing validation errors
- Fast, type-safe Rust implementation handles proper key filtering
- Integrated into Nix flake for reproducible builds

**Layer Filtering:**

- Corne layouts have 16 layers (L0-L15), but typically only 6 are used
- Filtering reduces SVG size by ~60% (137KB → 55KB)
- Improves README load times and readability

### Complete Example

```bash
nix develop

# 1. Edit layout in VIAL, save as .vil file

# 2. Convert to JSON (6 layers max, filter empty)
vil2json firmware/corne_v4-1_custom_hrmods.vil -m 6 -f

# 3. Generate YAML
keymap parse -q firmware/corne_v4-1_custom_hrmods.json \
  -o firmware/corne_v4-1_custom_hrmods.yaml

# 4. (Optional) Preview locally with layer filtering
keymap draw firmware/corne_v4-1_custom_hrmods.yaml \
  -s L0 L1 L2 L3 L4 L5 \
  -o images/generated/corne_v4-1_custom_hrmods.svg

# 5. Commit and push (.vil files trigger auto-conversion in CI)
jj describe -m "feat: update keymap layout"
jj git push
```

## Current Layout Features

The custom layout (`corne_v4-1_custom.vil`) includes:

### Layer Organization

- **Layer 0**: Base Colemak-DH layout with ergonomic key placement
- **Layer 1**: Numbers (1-6) and navigation arrows for efficient data entry
- **Layer 2**: Programming symbols + EurKey characters (äöüß) for multilingual coding
- **Layer 3**: Function keys (F1-F12) and RGB controls for system interaction
- **Layer 4**: NixOS/Niri window management with Colemak-DH native navigation
- **Layer 5**: System controls (vigiland idle inhibitor, satty-based screenshots)

### Key Optimizations

- **EurKey Integration**: Natural access to German characters via AltGr combinations
- **Programming Focus**: Optimized symbol placement for Nix/Rust/Python development
- **Consistent Navigation**: Uses `neio` pattern matching Helix editor bindings
- **Window Management**: Native Super+1-6 workspace switching with directional window movement
- **Modern Rust Tooling**: satty for screenshots, vigiland for idle management
- **WM-Agnostic Design**: Unified keybindings work consistently across niri/hyprland/cosmic
- **System Control Integration**: Single source of truth for volume, brightness, and system functions

### Detailed Layer Analysis

| Layer | Purpose                  | Key Features                                                    |
| ----- | ------------------------ | --------------------------------------------------------------- |
| 0     | **Base Colemak-DH**      | Original ergonomic layout (preserved)                           |
| 1     | **Numbers + Nav**        | 1-6 numbers, arrow keys, volume/brightness controls             |
| 2     | **Programming + EurKey** | äöüß, brackets, operators, Nix symbols                          |
| 3     | **Function Keys**        | F1-F12, RGB controls, system functions                          |
| 4     | **Window Management**    | Super+1-6 workspaces, neio navigation                           |
| 5     | **System Controls**      | M0: vigiland toggle, M1: screenshot region, M2: screenshot full |
| 6-15  | **Available**            | Empty layers for optional future expansion                      |

#### Programming Layer Enhancement (Layer 2)

Customized for **Nix/Rust/Python** development:

```text
Programming Symbols Layout:
!  @  #  $  %  ^     &  *  (  )  _  +
   ä     ß           ö  ü  [  ]  =  -
~  \  }  {  +  -     |  ...
```

**Key Features**:

- **Brackets**: Easy access to `[]`, `{}`, `()`
- **Operators**: Pipe `|`, arrows `->`, `=>`
- **Nix-specific**: Attribute sets `{}`, lists `[]`
- **String handling**: All quote types `"`, `'`, ```

#### EurKey Integration

- **German Characters**: Added proper AltGr mappings
  - `AltGr + A` → ä
  - `AltGr + O` → ö
  - `AltGr + U` → ü
  - `AltGr + S` → ß
- **Layout Compliance**: Maintains EurKey standard for European character access
- **Ergonomic Access**: Right thumb AltGr key for comfortable German typing

#### NixOS/Niri Window Management (Layer 4)

**Colemak-DH Native Navigation** with **neio** pattern:

```text
Window Management Layer:
Super+1  Super+2  Super+3  Super+4  Super+5  Super+6     [Workspace Switch]
         n        e        i        o                     [Window Focus]
         Shift+n  Shift+e  Shift+i  Shift+o               [Move Windows]

Shift+Super+6  Shift+Super+5  ...  Shift+Super+1        [Move to Workspace]
```

**Navigation Philosophy**:

- **n/e/i/o** = left/down/up/right
- Matches existing Hyprland `Super+neio` keybindings
- Consistent muscle memory between editor and window manager
- No context switching between hjkl and neio patterns

## Key Workflows

### Programming Workflow (Layer 2)

- Hold layer key + programming symbols
- Natural access to Nix attribute syntax `{}`
- Rust arrow operators `->`
- Python string operations with all quote types

### German Typing Workflow (Layer 2)

- `AltGr + aous`: Access äöüß characters
- Maintains full EurKey compatibility

### Window Management Workflow (Layer 4)

- `Super + Layer + 1-6`: Switch workspace
- `Super + Layer + neio`: Focus window direction
- `Super + Shift + Layer + 1-6`: Move window to workspace

## Hardware & Firmware

- **Model**: Corne V4.1 (46-key split keyboard) by [foostan](https://github.com/foostan/crkbd/)
- **Base Firmware**: `corne_v4-1_46_keebart_vial.uf2` from [Keebart](https://www.keebart.com/products/corne)
- **Source**: VIAL-compatible QMK firmware with GUI configuration support
- **Flashing**: Drag-and-drop to bootloader drive (no compilation required)

### Layout Files

- **Original Base**: `corne_nwwy.vil` - Community-contributed starting point
- **Default**: `corne_v4-1_default.vil` - Standard VIAL configuration
- **Custom Layout**: `corne_v4-1_custom.vil` - Personal customization
- **Home Row Modifiers**: `corne_v4-1_custom_hrmods.vil` - Enhanced ergonomic layout with mod-tap functionality

## Testing & Validation

The repository includes comprehensive testing infrastructure to ensure layout validity and system integrity.

### Essential Test Scripts

#### `test-all.sh`

Main test runner that executes all essential tests:

- Layout validation (VIAL files)
- Configuration validation (Nix flake)
- Optional: GitHub Actions local testing

**Usage:**

```bash
# Run basic tests
./scripts/test-all.sh

# Include GitHub Actions testing (slow on first run)
RUN_ACT_CHECK=true ./scripts/test-all.sh
```

#### `validate-vial.sh`

Validates all `.vil` files in the firmware directory:

- JSON syntax validation
- Basic VIAL structure checks
- Layer count verification

**Usage:**

```bash
./scripts/validate-vial.sh firmware/
```

#### `test-configs.sh`

Validates Nix flake configuration:

- Flake syntax checking
- Configuration validation

**Usage:**

```bash
./scripts/test-configs.sh
```

#### `act-check.sh`

Tests GitHub Actions workflows locally using `act`:

- Requires Docker daemon running
- Downloads container images on first run (slow)
- Tests firmware CI/CD pipeline

**Usage:**

```bash
# Test default workflow (firmware-ci.yml)
./scripts/act-check.sh

# Test specific workflow
./scripts/act-check.sh .github/workflows/layout-images-ci.yml
```

### Code Quality Tools

The project uses **prek** (a drop-in replacement for pre-commit) with automated hooks:

```bash
# Run all code quality checks
prek run --all-files

# Available in nix shell as aliases:
lint-all         # Run all prek hooks
update-hooks     # Update prek hooks
run-hooks        # Run all prek hooks
```

**Configured hooks:**

- **alejandra**: Nix code formatter
- **deadnix**: Dead code detection
- **statix**: Lints and suggestions for Nix code
- **flake-checker**: Flake health checks
- **yamlfmt**: YAML formatter

### Testing Recommendations

**For quick validation before commit:**

```bash
./scripts/test-all.sh
```

**For comprehensive testing (including GitHub Actions):**

```bash
RUN_ACT_CHECK=true ./scripts/test-all.sh
```

**For debugging specific issues:**

- Layout problems: `./scripts/validate-vial.sh firmware/`
- Nix configuration: `./scripts/test-configs.sh`
- Docker/act issues: `./scripts/act-check.sh`

## Advanced Tools & Scripts

The repository includes modern Rust-based tools and efficient workflows for keyboard management:

### Modern Tool Integration

**Screenshot & Annotation System** - Modern Wayland-native workflow using Rust tools:

- **wayshot** - Blazing-fast Rust-based screenshot tool for wlroots compositors
- **satty** - Modern screenshot annotation with comprehensive editing features
- **slurp** - Precise region selection for Wayland environments

**Idle Management** - Simple and effective power management:

- **vigiland** - Direct Wayland idle inhibitor with minimal overhead
- No wrapper scripts needed - simple `vigiland` command for toggle functionality

**RGB Configuration** - GUI-based lighting control:

- **VIAL GUI** - Complete RGB Matrix configuration through user-friendly interface
- Per-layer RGB setup, effect selection, and color customization
- No CLI scripting required - full visual control

### Usage Examples

```bash
# Modern screenshot workflows
wayshot -s "$(slurp)" --stdout | satty --filename - --fullscreen  # Region with annotation
wayshot --stdout | satty --filename - --fullscreen               # Full screen with annotation
wayshot -s "$(slurp)" -f ~/Pictures/screenshot-$(date '+%Y%m%d-%H:%M:%S').png  # Direct save

# Simple idle management
vigiland                                    # Start idle inhibition (ctrl+c to stop)
pkill -x vigiland || vigiland &            # Background toggle (used in keyboard macro)

# VIAL RGB configuration
vial                                        # Open VIAL GUI for RGB and layout configuration
```

## Documentation & Visualization

### Layout Images

Visual representations of keyboard layers of `corne_nwwy.vil` are stored in `images/nwwy/`.

### Generating New Images

Images are automatically generated via GitHub Actions (see "Layout Visualization & Image Generation" section above).
All `.vil` file changes trigger automatic image updates with no manual intervention required.

## Learning Resources

### Typing Practice

- [Keybr.com](https://www.keybr.com/) - Adaptive typing lessons for Colemak-DH
- [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/) - Visual layout design

### Development Resources

- [VIAL Documentation](https://get.vial.today/manual/) - Complete VIAL usage guide
- [QMK Firmware](https://docs.qmk.fm/) - Advanced firmware customization
- [Colemak-DH](https://colemakmods.github.io/mod-dh/) - Layout theory and optimization

## Motivation & Design Philosophy

### Ergonomic Goals

After [researching ergonomic keyboard layouts](https://colemakmods.github.io/mod-dh/compare.html), this configuration
aims to:

- **Reduce finger movement** through optimal key placement
- **Minimize same-finger typing** with better letter distribution
- **Improve typing comfort** via split keyboard design
- **Enhance coding efficiency** with programming-focused symbol layers

### Integration Philosophy

The layout is designed for seamless integration with:

- **NixOS**: Reproducible system configuration and package management
- **Niri/Hyprland**: Wayland compositors with tiling window management
- **Helix/Neovim**: Modal text editors with consistent navigation patterns
- **Development Workflow**: Nix/Rust/Python programming with customized symbol access

## Acknowledgments

This project builds upon excellent work from the keyboard community:

### Community Contributors

- **[nwwy](https://www.reddit.com/user/nwwy/)** - Original base layout (`corne_nwwy.vil`) and layer visualization images
- **foostan** - Corne keyboard hardware design
- **VIAL Team** - Open-source keyboard configuration software
- **QMK Contributors** - Firmware foundation and features

### Tools & Visualization

- **[caksoylar](https://github.com/caksoylar)** - keymap-drawer visualization tool
- **[YellowAfterlife](https://github.com/YAL-Tools)** - vial-to-keymap-drawer conversion utility
- **Keebart** - VIAL firmware compilation and distribution

## Key Features

- **Home Row Modifiers**: Enhanced ergonomic layout with mod-tap functionality

  - Available in `corne_v4-1_custom_hrmods.vil`
  - Tap for letters, hold for modifiers (GUI/Alt/Ctrl/Shift)
  - Significantly reduces finger travel and improves ergonomics
  - A(GUI), R(Alt), S(Ctrl), T(Shift) | N(Shift), E(Ctrl), I(Alt), O(GUI)

- **Modern Tool Integration**: Rust-based tools for efficient workflows

  - wayshot + slurp for fast Wayland screenshots
  - satty for screenshot annotation
  - vigiland for simple idle management
  - VIAL GUI for RGB and layout configuration

- **Automated CI/CD**: Zero-maintenance image generation and validation
  - Automatic layout image updates when .vil files change
  - Firmware validation and building
  - Pre-commit hooks for code quality

## Contributing

Suggestions and improvements are welcome! Feel free to:

- Open issues for suggestions or questions
- Submit pull requests for improvements
- Share your own layout customizations
- Contribute to documentation and visualization

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

The layout builds upon community contributions and open-source tools. All original work is available under MIT license,
while respecting the licenses of underlying projects and community contributions.
