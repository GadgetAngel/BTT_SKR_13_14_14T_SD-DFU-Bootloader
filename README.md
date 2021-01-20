# Bootloader for the following:

# Bigtreetech SKR 1.3, 1.4 and 1.4 TURBO; and
# For ALL BigTreeTech's 3D Printer boards that I could find!


## This repository can be downloaded as one large zip file from my google drive: https://drive.google.com/file/d/1hRSgYUa6hYUhNHsw1xSV_DP3yMu5Fhgo/view?usp=sharing


This is a fork of https://github.com/triffid/LPC17xx-DFU-Bootloader

Supports Bigtreetech SKR 1.3, SKR 1.4 and SKR 1.4 TURBO with the same binary.

# Bootloader for Bigtreetech SKR (mini) E3 TURBO board; and All BIGTREETECH boards.

This is a fork of https://github.com/ardiehl/BTT_SKR_13_14_14T_SD-DFU-Bootloader

Supports Bigtreetech SKR 1.3, SKR 1.4 and SKR 1.4 TURBO and SKR E3 TURBO with the same binary.

## SD card support
By default for the SKR V1.3, SKR V1.4 and SKR V1.4 Turbo the SD card (SPI 1) on the SKR as well as the SD card on a connected display board (SPI 0) (if connected) may be used for updating the firmware.

By defulat for the SKR E3 TURBO the SD card (SPI 1) is the ONBOARD SD card reader and is the ONLY SD card reader availble to the SKR E3 TURBO board.

As in the original code a file named "firmware.bin" on the FAT32-formatted SD card's first partition will be flashed to the application region (0x4000 after the 16k space for the bootloader), and after successful flashing, the file will be renamed to "firmware.cur" - 'cur' meaning 'current'.

## USB support

In addition, Flash via USB is enabled as default. It will be activated:
- if no firmware is installed (Flash @16K = 0xffffffff)
- if the button on a connected display board (e.g. BTT LCD-35 or RepRapDiscount Full Graphic Smart Controller) is pressed while booting. (for SKR V1.3,
SRK V1.4, or SKR V1.4 Turbo the pin 2 of EXP1 = 0.28 connected to GND, for SKR E3 Turbo pin 2 [in pins_BTT_SKR_E3_TURBO.h file it is pin 9] of EXP1 = 0.16
connected to GND)

## Upload via USB
For SKR V1.3, SKR V1.4 or SKR V1.4 Turbo, Press the button on the display board or connect EXP1 pin 2 to ground, then press reset. After about 2 seconds you should see the device, e.g. with lsusb:

```
ad@e5w ~]$ lsusb
...
Bus 001 Device 072: ID 1d50:6015 OpenMoko, Inc. Smoothieboard
```

Firmware can than be uploaded via dfu-util, e.g. (dfu-util ia a utility that run on Linux or Ubuntu platforms)

```
sudo dfu-util --download firmware.bin

dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release!!!
Opening DFU capable USB device...
ID 1d50:6015
Run-time device DFU version 0101
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 0101
Device returned transfer size 512
Copying data from PC to DFU device
Download	[=========================] 100%       243508 bytes
Download done.
state(8) = dfuMANIFEST-WAIT-RESET, status(0) = No error condition is present
Done!

```

For SKR (mini) E3 Turbo, Press the button on the display board or connect EXP1 pin 2 (P0_16) to ground, then press reset. After about 2 seconds you should see the device, e.g. with lsusb:

```
ad@e5w ~]$ lsusb
...
Bus 001 Device 072: ID 1d50:6015 OpenMoko, Inc. Smoothieboard
```

Firmware can than be uploaded via dfu-util, e.g.(dfu-util ia a utility that run on Linux or Ubuntu platforms)

```
sudo dfu-util --download firmware.bin

dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release!!!
Opening DFU capable USB device...
ID 1d50:6015
Run-time device DFU version 0101
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 0101
Device returned transfer size 512
Copying data from PC to DFU device
Download	[=========================] 100%       243508 bytes
Download done.
state(8) = dfuMANIFEST-WAIT-RESET, status(0) = No error condition is present
Done!

```

## Silent beeper

For SKR V1.3, SKR V1.4, or SKR V1.4 Turbo: EXP1 pin 1 (1.30) will be configured as output and set to 0 to avoid endless beeping of the beeper on a connected RepRapDiscount Full Graphic Smart Controller.

For SKR (mini) E3 Turbo: EXP1 pin 1 (P2_08) will be configured as output and set to 0 to avoid endless beeping of the beeper on a connected RepRapDiscount Full Graphic Smart Controller.

## Installing the bootloader

@ardiehl I tried it with a serial connection first but somehow that did not worked for me. I got the "Synced.." message after sending ? but most other characters send were echo'ed differently. Seens to be some baudrate problem, gave up after several hours of probing.

Using a ST-Link V2 clone with openocd worked fine.

Install openocd and connect the ST-Link with 3 wires (btt powered via USB or 12/24V) as follows:

```
ST-Link     BTT
----------------------------
SWDIO       SWDIO (J2 Pin 2)
SWCLK       SWDCLK (J2 Pin 4)
GND         GND (J2 Pin 3) or some other GND pin, e.g. middle pin of one endswitch connector
```

and run `make upload` or start the `writebootloader` script in bttskr/.
You can backup your current bootboader or the whole flash using the scripts `readbootloader` or `readflash`.

@GadgetAngel I tried it with BTT Writer and Flashmagic another user @Sineos used a USB to TTL adapter and got it to work with Flashmagic

Here is the URL for instructions on how Sineos got it to work: https://github.com/bigtreetech/BIGTREETECH-SKR-V1.3/issues/346#issuecomment-754120640

Here is the URL for instructions on how I, GadgetAngel, got it to work: https://github.com/GadgetAngel/BTT_SKR_13_14_14T_SD-DFU-Bootloader/blob/main/Guide%20UART%20Flash%20SKR%20E3%20TURBO%20board.pdf

## Configuration
By default, the bootloader will show status/debug messages via the serial port @115200 baud. You can configure these as well as disableing USB, disabling the second SD card and so on in `config.h`.

## Why
@ardiehl, I managed to destroy the SD card slot on my 1.4 board and while trying to solder a new one, i destroyed some pads on the board. Insead of using another board i now use the SD card slot on the display board for firmware uploads.

@GadgetAngel, I bought a SKR E3 TURBO board and it produced a high pitch sound out of the Speaker connected to my CR10_STOCKDISPLAY from my original Ender 3 Pro.
This high pitch sound continued to be produced until Marlin boot screen was displayed onto the display screen.  I recompiled this project to pull the beeper_pin
low while the bootloader was running to ensure a silent boot process.