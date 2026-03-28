# PENOR
### Portable Ergonomic NKRO Ortholinear Receiver

![penor](./images/thumbnail.jpg)
## Disclaimers
* This is just a project I made for myself, so I didn't try and make it as easy as possible for others to reproduce or anything like that. The firmware is also extremely specific to my usecase (Colemak-DH with Swedish letters and a fuck-ton of esoteric custom features). I'll try my best to write instructions as clear as possible with the time I am willing to spend on this (as I find it unlikely that anyone will see this, and thus want to actually make the keyboard).
* The case itself is decently thick (at least for a 6x3 keyboard) at a bit more than 1 cm of thickness. This is because the RP2040-Zero and TRRS-jack is underneath the PCB, thus meaning that wires will need to be soldered from the PCB to the RP2040-Zero and TRRS-jack. For this I used AWG30 copper wire, but even with this thin wire it was still a pain in the ass to put the PCB into the case and screwing it down without any wires getting pinched.
* The case was designed to allow access to the reset switch of the RP2040-Zeros at the bottom. This turned out to be completely useless, as you can't enter the bootloader on an RP2040-Zero with that button, where you are actually supposed to hold down the boot button. I had this misconception from my previous keyboard I assembled which I thought used an MCU that functioned in the same way as an RP2040-Zero. If you want to you can edit the FreeCAD model to either remove the hole or move it to where the boot button is. In any case you won't need access to the boot button as you can hold down the bootmagic keys (discussed further down) to enter the bootloader.

## How to flash firmware
* **Before assembly**: Hold down the boot button whilst plugging in the RP2040-Zero to your computer.
* **After assembly**: (this works for each individual side) hold down the key in the corner that is closest to the USB-port as you are plugging the USB in.
Once you have done either of these the RP2040-Zero should appear in your file browser. Open it and place your desired firmware (.uf2 file) there, which will then flash it.

## Assembly instructions
1. Before assembling anything you should already flash the firmware to BOTH of your RP2040-Zeros, as it contains logic for allowing to enter the bootloader without the usage of the boot button which won't be acessible in the finished assembly. Just hold down the boot button whilst plugging it into your computer, and it should appear in your file manager. Place the .uf2 file from this repo (or compile the firmware yourself) into the RP2040-Zeros. If your RP2040-Zeros has "perferations" on the edges from when it was snapped of from a larger board then you should sand these down so that the edges of the board are flat, as otherwise they won't fit properly in the case.
2. Solder all the diodes to the PCB. Remember to look at the icons on the PCB that specify what direction the diodes should have so that you don't acidentally assemble some in the wrong direction, as sometimes the direction is different from what you might expect (almost all point downwards, however a few point up). Once soldered, clip the legs so that they are pretty short, so that they fit for the next step.
3. On the side that has the leg joints, put the 3D-printed switch plate on the PCB, and put all of the switches through the switch plate and PCB, and then solder all of the switches to the PCB.
4. Solder loose wires to every hole on the PCB that is labeled as "row n" and "col n" (where n is a number). In total you should have 10 loose wires. The longer they are, the more you will want to shoot yourself when trying to assemble the plate with the case without any wires being in the way, however if they are too short then everything will be extremely tight. I recommend making the wires furthest away from where the RP2040-Zero will be in the case be around 10 cm, where the closest ones can be around 6 cm or so. For the left half I made ALL of the wires 15 cm long and the walls in my room almost became red.
5. Solder wires from the RP2040-Zero to the TRRS-jack. Remember to look at how the RP2040-Zero will lay in the case to ensure that you solder the wires onto the pins in ways that the wires will actually be able to lay in so that they don't have to make any really sharp turns, concidering how there is a wall directly next to the RP2040-Zero on one side (don't forget that the side the wall is on changes for each half of the keyboard!). You need three wires for this, which I recommend making around 10 cm. Technically the legs you decide to use for the TRRS-jack doesn't matter *as long as you do the same for both halves*, but I'll say which ones I used. Remember to try and solder it to the legs so that the solder is on the inside of the legs and not the outside, so that the jack can lay flat without the solder being in the way. Also make note of how the jacks will sit in the case, so that you solder the wires to go in the correct direction so that they don't have to bend sharply.
    1. Solder a wire from the **GND** pin to the leg that is alone on its side.
    2. Solder a second wire from the **3V3** pin to the leg that is the closest to the leg you previously soldered to.
    3. Solder the last wire from **pin 0** to the leg that is next to the second leg you soldered to.
6. If you are assembling the left half (and ONLY if it is the left half), solder a short wire from **3V3** to **pin 29**. This is to make it able to tell that it is the left half of the keyboard automatically.
7. Solder all of the wires onto the RP2040-Zero. Remember to concider the walls to make the wires not have to make any really sharp turns by the pins. Also remember to concider how the part that will hold down the RP2040-Zero will cover up the pins at the bottom, which will thus also to some extent be like a wall that needs to be router around.
   * col0 -> 2
   * col1 -> 3
   * col2 -> 4
   * col3 -> 5
   * col4 -> 6
   * col5 -> 7
   * row0 -> pin 15
   * row1 -> pin 26
   * row2 -> pin 27
   * row3 -> pin 28
8. Put the TRRS-jack by its hole in the case. Add a nut to the hole next to it, and use a bolt to screw the small 3D-printed part that holds it in place. Then put the RP2040-Zero where it should be in the case, add a nut to the 3D-printed part that holds it down, and fasten it with a bolt. If you have no clue which parts are supposed to be used for this then you can look in the FreeCAD model in this repo and see the location of all the parts.
9. Put the PCB onto the case, making sure that no wires are getting pinched between the PCB and the case. You will most likely need some long and slim object like a chopstick to try and nudge wires away from areas that the PCB will lay on as you put the PCB on. Once there is no wobble, or at least extremely minimal wobble, you can screw the two bolts into the holes in the PCB. If you have printed the case in black like shown in the image at the top of this file then you can optionally put electrical tape onto the screw heads. It does't look absolutely perfect, but it does make it look way neater than seeing two big-ass silvery bolt heads between your keys.







----------
OLD CONTENTS. NOT DONE WITH THE README YET.

* Hardware Supported: idfk what I'm supposed to write here. It was designed to use a RP2040-Zero with a TRRS-jack, khali choc switches, and those THT diode things. You also need some wire so that you can bridge the MCU to the PCB as they are seperate. You also probably want a 3D-printer so that you can print the case. Also it is designed to use tenting feet, and also some small anti-slip rubber bead thingimabobs (remember to mention the nuts and bolts you got, and the lengths and how you had to cut them)

Make example for this keyboard (after setting up your build environment):

    make penor:default

Flashing example for this keyboard:

    make penor:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
