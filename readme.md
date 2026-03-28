# penor

![penor](./images/thumbnail.jpg)

Portable Ergonomic NKRO Ortholinear Receiver. No shut up the name isn't forced at all. This is a split keyboard with each half being 6x3, with 4 thumb-keys.

* Keyboard Maintainer: [baksoBoy](https://github.com/theBaksoBoy)
* Hardware Supported: idfk what I'm supposed to write here. It was designed to use a RP2040 with a TRRS-jack, khali choc switches, and those THT diode things. You also need some wire so that you can bridge the MCU to the PCB as they are seperate. You also probably want a 3D-printer so that you can print the case. Also it is designed to use tenting feet, and also some small anti-slip rubber bead thingimabobs
* Hardware Availability: idfk kys

Make example for this keyboard (after setting up your build environment):

    make penor:default

Flashing example for this keyboard:

    make penor:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
