# Custom Keyboard Configuration: Corne V4.1 Colemak-DH (EurKey)

## Overview

This repository documents my journey into ergonomic typing with a [Corne V4.1 (46 keys)](https://github.com/foostan/crkbd/) split keyboard. It's a personal project focused on optimizing typing efficiency and comfort.

### Configuration Details

- **Keyboard Layout**: [EurKey](https://eurkey.steffen.bruentjen.eu/)
- **Key Arrangement**: [Colemak-DH (Matrix)](https://colemakmods.github.io/mod-dh/)
- **Firmware**: [VIAL](https://get.vial.today/)

### Learning Resources

- [Keybr.com](https://www.keybr.com/) for layout practice
- [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/)
- Information on how compile and flash your own firmware can be found [here](https://github.com/foostan/kbd_firmware).

## Motivation

After [researching ergonomic keyboard layouts](https://colemakmods.github.io/mod-dh/compare.html), I chose Colemak-DH to:
- Reduce finger movement
- Minimize same-finger typing
- Improve overall typing comfort and speed

## Repository Structure

```
.
├── firmware/
│   ├── corne_v4-1_46_keebart_vial.uf2  # VIAL firmware with up to 16 layers
│   ├── corne_v4-1_default_layout.vil   # Standard layout for VIAL
│   └── corne_v4-1_custom.vil           # Custom layout
├── images/
│   ├── keyboard_layout.png
│   └── keyboard_layout.xcf
├── README.md
└── LICENSE
```

## Planned Improvements

- Explore efficient modifier key placement: [Home Row Modifiers](https://precondition.github.io/home-row-mods)
- Optimize shortcuts for:
  - [Cosmic DE](https://system76.com/cosmic/)
  - [Helix](https://helix-editor.com/)
  - [(Neo-)Vim](https://neovim.io/)

## Contributing

Suggestions and improvements are welcome! Feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
