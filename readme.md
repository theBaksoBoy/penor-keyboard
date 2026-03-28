# PENOR
### Portable Ergonomic NKRO Ortholinear Receiver

![penor](./images/thumbnail.jpg)
## Disclaimers
* This is just a project I made for myself, so I didn't try and make it as easy as possible for others to reproduce or anything like that. The firmware is also extremely specific to my usecase (Colemak-DH with Swedish letters and a fuck-ton of esoteric custom features). I'll try my best to write instructions as clear as possible with the time I am willing to spend on this (as I find it unlikely that anyone will see this, and thus want to actually make the keyboard).
* The case itself is decently thick (at least for a 6x3 keyboard) at a bit more than 1 cm of thickness. This is because the RP2040 and TRRS-jack is underneath the PCB, thus meaning that wires will need to be soldered from the PCB to the RP2040 and TRRS-jack. For this I used AWG30 copper wire, but even with this thin wire it was still a pain in the ass to put the PCB into the case and screwing it down without any wires getting pinched.
* The case was designed to allow access to the reset switch of the RP2040s at the bottom. This turned out to be completely useless, as you can't enter the bootloader on RP2040 with that button, where you are actually supposed to hold down the boot button. I had this misconception from my previous keyboard I assembled which I thought used an MCU that functioned in the same way as an RP2040. If you want to you can edit the FreeCAD model to either remove the hole or move it to where the boot button is. In any case you won't need access to the boot button as you can hold down the bootmagic keys (discussed further down) to enter the bootloader.

## How to flash firmware
* **Before assembly**: Hold down the boot button whilst plugging in the RP2040 to your computer.
* **After assembly**: (this works for each individual side) hold down the key in the corner that is closest to the USB-port as you are plugging the USB in.
Once you have done either of these the rp2040 should appear in your file browser. Open it and place your desired firmware (.uf2 file) there, which will then flash it.

## Assembly instructions
1. Before assembling anything you should already flash the firmware to BOTH of your rp2040, as it contains logic for allowing to enter the bootloader without the usage of the boot button which won't be acessible in the finished assembly. Just hold down the boot button whilst plugging it into your computer, and it should appear in your file manager. Place the .uf2 file from this repo (or compile the firmware yourself) into the rp2040.
2. Solder all the diodes to the PCB. Remember to look at the icons on the PCB that specify what direction the diodes should have so that you don't acidentally assemble some in the wrong direction, as sometimes the direction is different from what you might expect (almost all point downwards, however a few point up). Once soldered, clip the legs so that they are pretty short, so that they fit for the next step.
3. On the side that has the leg joints, put the 3D-printed switch plate on the PCB, and put all of the switches through the switch plate and PCB, and then solder all of the switches to the PCB. 








OLD CONTENTS:


* Keyboard Maintainer: [baksoBoy](https://github.com/theBaksoBoy)
* Hardware Supported: idfk what I'm supposed to write here. It was designed to use a RP2040 with a TRRS-jack, khali choc switches, and those THT diode things. You also need some wire so that you can bridge the MCU to the PCB as they are seperate. You also probably want a 3D-printer so that you can print the case. Also it is designed to use tenting feet, and also some small anti-slip rubber bead thingimabobs
* Hardware Availability: idfk kys

Make example for this keyboard (after setting up your build environment):

    make penor:default

Flashing example for this keyboard:

    make penor:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
