{
  description = "Corne V4.1 Vial-QMK Firmware Builder";

  # Nix configuration for public caches (avoids requiring personal cachix credentials)
  nixConfig = {
    extra-substituters = [
      "https://cache.nixos.org"
      "https://nix-community.cachix.org"
    ];
    extra-trusted-public-keys = [
      "cache.nixos.org-1:6NCHdD59X431o0gWypbMrAURkbJ16ZPMQFGspcDShjY="
      "nix-community.cachix.org-1:mB9FSh9qf2dCimDSUo8Zy7bkq5CX+/rkCWyvRCYg3Fs="
    ];
  };

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05";
    nixos-unstable.url = "github:nixos/nixpkgs/nixos-unstable";

    # The vial fork of qmk, stores the keyboard config in on-keyboard memory, and
    # supports the `Vial` GUI key map config app.
    vial-qmk = {
      url = "github:vial-kb/vial-qmk/vial";
      flake = false;
    };

    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    nixpkgs,
    nixos-unstable,
    vial-qmk,
    flake-utils,
    ...
  }:
  # This effectively appends `.x86_64-linux` to the attributes returned
  # by the function passed in. The `system` parameter is also that string.
    flake-utils.lib.eachSystem ["x86_64-linux" "aarch64-linux"] (
      system: let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [
            # Add unstable packages
            (_final: prev: {
              unstable = nixos-unstable.legacyPackages."${prev.system}";
            })

            # Use python3.11 for compatibility with QMK
            (final: _prev: {
              python3 = final.python311;
              python3Packages = final.python311.pkgs;
            })
          ];
        };
      in {
        # Development shell for building firmware
        devShells.default = pkgs.mkShell {
          # Corne V4.1 specific configuration
          KEYBOARD = "crkbd/rev4_1"; # ✅ Correct for Corne V4.1
          KEYMAP = "vial";

          # Enhanced build configuration
          VIAL_QMK_DIR = "${vial-qmk}";

          # Build inputs with additional tools
          buildInputs = with pkgs; [
            qmk
            unstable.vial
            dfu-util # For flashing via DFU
            avrdude # For AVR microcontrollers
            gcc-arm-embedded # For ARM microcontrollers
            python3 # QMK requirements
            python3Packages.setuptools

            # QMK Python dependencies
            unstable.python3Packages.milc # Need >= 1.9.0 for via2json
            python3Packages.dotty-dict
            python3Packages.hid
            python3Packages.hjson
            python3Packages.jsonschema
            python3Packages.pyusb
            python3Packages.argcomplete
            python3Packages.platformdirs # MILC dependency
            python3Packages.appdirs # Legacy MILC dependency
            python3Packages.pyyaml # YAML parsing for pre-commit hooks
            git # For QMK setup
            which # Build dependency
            gnumake # Build system

            # Additional utilities
            jq # JSON processing for layout files
            unstable.keymap-drawer # Layout visualization tool

            # Rust tool: VIAL to QMK JSON converter
            (pkgs.rustPlatform.buildRustPackage {
              pname = "vil2json";
              version = "0.1.0";
              src = ./tools/vil2json;
              cargoLock.lockFile = ./tools/vil2json/Cargo.lock;
            })

            # Formatting and validation tools
            # prek is included for local development (from unstable)
            # GitHub Actions CI uses j178/prek-action instead
            unstable.prek # Pre-commit hook manager (meta-tool)
            alejandra # Nix formatter (prek)
            deadnix # Remove unused Nix code (prek)
            statix # Nix linter (prek)
            unstable.flake-checker # Flake validation (prek)
            yamlfmt # YAML formatter (prek)
            taplo # TOML formatter (prek)
            nodePackages.prettier # JSON/YAML/Markdown formatter (prek)
            ripsecrets # Secret detection (prek)
            act # Local GitHub Actions runner
            nodePackages.markdownlint-cli # Markdown linting
            yamllint # YAML validation
          ];

          # Alias commands for building and flashing
          shellHook = ''
            # Ensure QMK is properly configured
            export QMK_HOME="${vial-qmk}"

            # Add scripts directory to PATH for easy command access
            export PATH="$(pwd)/scripts:$PATH"

            # Simple build aliases (fish-compatible)
            alias build='make -C ${vial-qmk} BUILD_DIR=$(pwd)/build COPY=echo -j$(nproc) $KEYBOARD:$KEYMAP'
            alias flash='make -C ${vial-qmk} BUILD_DIR=$(pwd)/build COPY=echo -j$(nproc) $KEYBOARD:$KEYMAP:flash'
            alias clean='rm -rf build/ && echo "Build directory cleaned"'
            alias copy-uf2='cp build/*.uf2 firmware/ 2>/dev/null && echo "UF2 copied to firmware/" || echo "No UF2 files found"'

            # Environment setup
            echo "Corne V4.1 Vial-QMK Build Environment Ready"
            echo "==========================================="
            echo ""
            echo "Configuration:"
            echo "  Keyboard: \$KEYBOARD"
            echo "  Keymap:   \$KEYMAP"
            echo "  QMK Dir:  \$QMK_HOME"
            echo ""
            echo "Available commands:"
            echo ""
            echo "Build Commands:"
            echo "  build            - Build firmware (.uf2 file)"
            echo "  flash            - Build and flash firmware"
            echo "  clean            - Clean build directory"
            echo "  copy-uf2         - Copy built UF2 to firmware/"
            echo ""
            echo "Testing Commands:"
            echo "  test-all.sh      - Run complete test suite"
            echo "  test-layouts.sh  - Validate VIAL layout files"
            echo "  test-configs.sh  - Validate Nix configurations"
            echo "  act-check.sh     - Run firmware CI locally"
            echo ""
            echo "Code Quality:"
            echo "  prek run --all-files  - Run all code quality checks"
            echo "  prek run --fail-fast  - Run checks, stop on first failure"
            echo "  alejandra .           - Format Nix files"
            echo "  deadnix .             - Remove unused Nix code"
            echo "  statix check .        - Lint Nix files"
            echo "  yamlfmt .             - Format YAML files"
            echo ""
            echo "Utility Commands:"
            echo "  keymap           - Generate layout images from YAML files"
            echo "  validate-vial.sh - Manual VIAL file validation"
            echo ""
            echo "Build process:"
            echo "  1. Run 'build' to compile firmware"
            echo "  2. Put keyboard in bootloader mode"
            echo "  3. Copy .uf2 file to mounted drive"
            echo ""

            # Verify keyboard configuration exists
            if [ -d "${vial-qmk}/keyboards/crkbd/rev4_1" ]; then
              echo "✅ Corne V4.1 keyboard configuration found"
            else
              echo "❌ Corne V4.1 keyboard configuration NOT found"
            fi
            echo ""
          '';

          # Additional environment variables
          NIX_SHELL_PRESERVE_PROMPT = "1";
        };

        # Formatter for the flake
        formatter = pkgs.alejandra;
      }
    );
}
