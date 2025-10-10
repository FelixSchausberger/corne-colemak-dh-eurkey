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
- **Version Control**: Jujutsu (jj) with dev-main branching strategy

## Development Workflow

### Jujutsu Workflow with Dev-Main Branching

The project uses Jujutsu (jj) for version control with a dev-main branch strategy:

**Branch Strategy:**

- **dev**: Daily development branch (default working branch)
- **main**: Stable, always-green branch
- PRs go from dev → main when ready for stable release

**Daily Workflow on Dev Branch:**

```bash
# Work directly on dev branch
jj describe -m "feat: add new feature"

# Or use AI-powered commit message (if lumen is available)
jjdescribe

# Push changes to remote dev branch
jj git push
```

**Creating Feature Branches (Optional):**

For experimental features that need isolation from dev:

```bash
jjbranch  # or jjb
# Interactive prompts:
# 1. Select type: feat, fix, chore, docs, test, refactor, perf
# 2. Enter description (lowercase, hyphens only)
# Result: Creates branch from current revision, commits with conventional format

# When ready, use jjpush to create PR back to dev
jjpush
```

**Merging Dev to Main:**

When dev is stable and ready for release, create a PR from dev → main manually via GitHub UI.

**Key points:**

- Work happens on `dev` by default
- `main` only receives tested, stable changes from `dev`
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

- `keymap-drawer` - Layout visualization (from nixpkgs)
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

The custom layout (`corne_v4-1_custom.vil`) features:

**Layer 0**: Base Colemak-DH layout
**Layer 1**: Numbers (1-6) and navigation arrows with volume/brightness controls
**Layer 2**: Programming symbols + EurKey characters (äöüß)
**Layer 3**: Function keys (F1-F12) and RGB controls
**Layer 4**: NixOS/Niri window management (Super+1-6 workspaces, neio navigation)
**Layer 5**: System controls (vigiland idle inhibitor, satty-based screenshots)

### Key Optimizations

- **EurKey Integration**: AltGr mappings for German characters
- **Programming Focus**: Optimized for Nix/Rust/Python development
- **Window Management**: Colemak-DH native navigation (neio pattern)
- **Consistent Navigation**: Matches Helix editor keybindings
- **Modern Rust Tooling**: satty for screenshots, vigiland for idle management
- **WM-Agnostic Design**: Unified functionality across niri/hyprland/cosmic
- **System Control Integration**: Single source of truth for volume, brightness, system functions

## System Integration

### Modern Architecture

The keyboard layout integrates seamlessly with modern Rust-based system tools:

- **Screenshot System**: Uses satty (Rust) for unified, WM-agnostic screenshot functionality
- **Idle Management**: Direct vigiland integration without wrapper scripts for clean architecture
- **Window Management**: Consistent keybindings across niri, hyprland, and cosmic compositors

### Macro Implementation

- **M0**: `sh -c 'pkill -x vigiland || vigiland &'` - Direct vigiland toggle (already updated)
- **M1**: `wayshot -s "$(slurp)" --stdout | satty --filename - --fullscreen` - Modern region screenshot with annotation
- **M2**: `wayshot --stdout | satty --filename - --fullscreen` - Modern full screen screenshot with annotation

**Note**: The VIAL files still contain old screenshot commands that need updating in the actual layout files.

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

#### Automated Process (Primary)

The repository includes GitHub Actions automation for layout image generation:

- **Trigger**: Changes to `firmware/*.vil` files or manual workflow dispatch
- **Process**:
  1. Detects changed `.vil` files using git diff
  2. Uses headless browser automation with Puppeteer to convert `.vil` → `.yaml` via web tool
  3. Generates `.svg` and `.png` images using keymap-drawer in Nix environment
  4. Auto-commits updated images to `images/generated/` directory
  5. Updates README.md with image references
- **Benefits**: Zero-maintenance image updates, always in sync with layout changes

#### Manual Process (Development)

For local preview and iteration:

1. **Convert Layout**: Use [vial-to-keymap-drawer web tool](https://yal-tools.github.io/vial-to-keymap-drawer/)
   to convert `.vil` → YAML
2. **Generate Images**: Use `keymap-drawer draw layout.yaml -o output.svg` in nix shell
3. **Preview Changes**: View generated images locally before committing

### Common Tasks

- **Modify key layout**: Edit `.vil` files using VIAL software
- **Add new layer**: Use VIAL to create additional keyboard layers
- **Flash firmware**: Use `.uf2` files with keyboard in bootloader mode
- **Update images**: Automatic via GitHub Actions (or manual workflow for local preview)
- **Build firmware**: Use `build` command in development shell
- **Run tests**: Use `./scripts/test-all.sh` for comprehensive validation
- **Code quality**: Use `prek run --all-files` or `lint-all` in nix shell

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

- **Purpose**: Automatically generates layout visualization images
- **Triggers**: Changes to `firmware/*.vil` files or manual dispatch
- **Process**:
  1. **Change Detection**: Git diff to identify modified `.vil` files
  2. **VIAL to YAML Conversion**: Headless browser automation using Puppeteer
  3. **Image Generation**: keymap-drawer in Nix environment for SVG/PNG creation
  4. **Documentation Update**: Auto-commit images and README updates
  5. **PR Integration**: Comments with generation results and artifact links
- **Benefits**: Zero-maintenance visual documentation, always current with layout changes

### Automation Architecture

- **Reproducible Builds**: Nix flakes ensure consistent environments across local/CI
- **Dependency Management**: All tools (QMK, VIAL, keymap-drawer) managed via Nix
- **Security**: Minimal permissions for core functionality
- **Efficiency**: Smart change detection, parallel workflows, artifact caching
