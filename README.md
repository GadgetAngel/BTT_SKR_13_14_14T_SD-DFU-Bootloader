# Bootloaders for BIGTREETECH Boards:

## This repository can be downloaded as one large zip file from my Google drive: https://drive.google.com/file/d/1xhxnywoOksBOjhFACwzbBQJ6E4Dk4YA9/view?usp=sharing

This is a fork of https://github.com/triffid/LPC17xx-DFU-Bootloader and https://github.com/ardiehl/BTT_SKR_13_14_14T_SD-DFU-Bootloader

# Puropse of this repository

The purpose of this repository is two folded. The first purpose to to back up the original boot loaders for the BIGTREETECH boards.  The second purpose of this repository is to provide a way for a user to compile a custom bootloader for the SKR V1.3, SKR V1.4, SKR V1.4 Turbo and SKR E3 Turbo boards.

# The first purpose is to back up the original boot loaders for the BIGTREETECH boards

The backups can be found  here: https://github.com/GadgetAngel/BTT_SKR_13_14_14T_SD-DFU-Bootloader/tree/main/bootloader_bin/backed_up_original_bootloaders

There are PDF files for both Windows 10 based computers and Linux computers.  These PDF files are GUIDES for the Marlin user on how to backup the bootloader file and the bootloader-and-firware file for the BIGTREETECH boards.

The "Bigtreetech boards Bootloaders_vX.X.pdf" contains the layout of the flash memory for each of the BIGTREETECH boards. You will need this file and a GUIDE if you want to backup or restore your bootloader file to a BIGTREETECH board.

---

The GUIDES for Windows 10 machines are listed below:

If you plan on restoring or backing up the bootloader using a Windows 10 machine than use one of these GUIDES:

1) "Guide UART Flash SKR V1.3 or SKR V1.4 or SKR V1.4 TURBO using Windows 10.pdf" which should only be used for SKR 1.3, SKR V1.4 or SKR V1.4 Turbo boards. The PDF uses the BIGTREETECH BTT writer device to communicate with the board.

2) "Guide UART Flash SKR V1.3 or SKR V1.4 or SKR V1.4 TURBO using USB to TTL adapter using Windows 10.pdf" which should only be used for SKR V1.3, SKR V1.4, or SKR V1.4 Turbo boards.  This PDF uses the USB to TTL adapter to communicate to the board.

3) "Guide UART Flash SKR E3 TURBO board using Windows 10.pdf" or "Guide UART Flash SKR E3 TURBO board_reducedsize using Windows 10.pdf" which should only be used for the SKR (mini) E3 Turbo board or SKR E3 Turbo board. The BIGTREETECH BTT Writer device is used to communicate with the board.

4) "Guide ST-LINK Utility on Windows 10 for ST-Link-V2-Clones for accessing STM32 Processors.pdf" which can be used on the following BIGTREETECH boards: BTT002 V1.0, GTR V1.0, SKR CR6 V1,0, SKR E3 DIP, SKR mini E3 V1.2, SKR mini E3 V2.0, SKR mini MZ V1.0, SKR mini 1.1, SKR PRO V1.1, or SKR PRO V1.2 boards. This GUIDE can be used for either ST-LINK-V2 clones or ST-LINK-V2 offical device from STelectronics.

The "Bigtreetech boards Bootloaders_vX.X.pdf" contains the layout of the flash memory for each of the BIGTREETECH boards. You will need this file and a GUIDE if you want to backup or restore your bootloader file to a BIGTREETECH board.

---

The GUIDES for LINUX machines are listed below:

If you plan on restoring or backing up the bootloader using Linux (Linux Mint 20.1 or Ubuntu or a Debian Derivative):

1) "Guide st-flash Utility for ST-Link-V2-Clones With NRST for accessing STM32 Processors using LINUX.pdf" which can be used on the following BIGTREETECH boards: BTT002 V1.0, GTR V1.0, SKR CR6, SKR E3 DIP, SKR mini E3 V1.2, SKR mini E3 V2.0, SKR mini MZ V1.0, SKR mini 1.1, SKR PRO V1.1, or SKR PRO V1.2 boards.  This GUIDE covers the st-flash utility and the eblink utility. The ST-LINK-V2 clone is the device used to communicate with the board and uses 5 wires to connect to the SWD port. One of these wires is the RST wire and the RST wire is connected to the SWD connector.

2) "Guide st-flash Utility for ST-Link-V2-Clones WO NRST for accessing STM32 Processors using LINUX.pdf" which can be used on the following BIGTREETECH boards: BTT002 V1.0, GTR V1.0, SKR CR6, SKR E3 DIP, SKR mini E3 V1.2, SKR mini E3 V2.0, SKR mini MZ V1.0, SKR mini 1.1, SKR PRO V1.1, or SKR PRO V1.2 boards.  This GUIDE covers the st-flash utility and the eblink utility. The ST-LINK-V2 clone is the device used to communicate with the board and uses 4 wires to connect to the SWD port. One of these wires is the RST wire and the RST wire is NOT connected to the SWD connector.

3) "Guide STM32CubeProgrammer Utility for ST-Link-V2 (offical) for accessing STM32 Processors using LINUX.pdf" which can be used on the following BIGTREETECH boards: BTT002 V1.0, GTR V1.0, SKR CR6, SKR E3 DIP, SKR mini E3 V1.2, SKR mini E3 V2.0, SKR mini MZ V1.0, SKR mini 1.1, SKR PRO V1.1, or SKR PRO V1.2 boards.  This GUIDE covers the STM32CubeProgrammer utility. The ST-LINK-V2 offical device is the item used to communicate with the board and uses 4/5 wires to connect to the SWD port. One of these wires is the RST wire and the RST wire is connected to the SWD connector. You can also not connect up the RST wire but it is recommened that you do connect the RST wire.  But some MCU processor may not connect to the ST-LINK-V2 device if the RST line is used, if that is the case, try again but this time disconnect the RST line.

The "Bigtreetech boards Bootloaders_vX.X.pdf" contains the layout of the flash memory for each of the BIGTREETECH boards. You will need this file and a GUIDE if you want to backup or restore your bootloader file to a BIGTREETECH board.

---

The backups can be found in the backup folder located: https://github.com/GadgetAngel/BTT_SKR_13_14_14T_SD-DFU-Bootloader/tree/main/bootloader_bin/backed_up_original_bootloaders

The backup folder is organized by the name of the BIGTREETECH board.
Each board has three subfolders: "Bootloader" folder, "Bootloader ONLY" folder, and "Bootloader_and_Firmware" folder. Optional folders are: "old" folder, "From the web" folder, or "Problem with using ST-LINK Utility" folder

1) Bootloader folder: contains the bootloader file that is the size determined by Marlin. The name of the file is the "board name-bootloader.zip". There are two files contained within the .zip folder which are named "board_name-bootloader.bin" and "board_name-bootloader.hex".  If you have a choice to flash either file please use the .hex file because it knows where in flash memory the data needs to reside. The reason the files are contained within a zip folder is to assist with downloading these types of files from the web.

2) Bootloader ONLY folder: contains the bootloader file that is the actual size of the boot loader. The name of the file is the "board_name-bootloader-only.zip".  There are two files contained within the .zip folder which are named "board_name-bootloader-only.bin" and "board_name-bootloader-only.hex".  If you have a choice to flash either file please use the .hex file because it knows where in flash memory the data needs to reside.  The reason the files are contained within a zip folder is to assist with downloading these types of files from the web.

3) Bootloader_and_Firmware folder: contains the bootloader-and-firmware file which is a copy of the full flash memory of the board. The name of the file is the "board_name-bootloader-and-firmware.zip" There are two files contained within the .zip folder which are named "board_name-bootloader-and-firmware.bin" and "board_name-bootloader-and-firmware.hex". If you have a choice to flash either file please use the .hex file because it knows where in flash memory the data needs to reside.  The reason the files are contained within a zip folder is to assist with downloading these types of files from the web.

4) Optional folders: "old" folder or "From the web" or "Problem with using ST-LINK Utility" folder

The "old" folder: contains files for my puposes.
The "From the web" folder: contains boot loader files I found on the web.

The "Problem with using ST-LINK Utility" contains information for obtaining the bootloader from the SKR MINI E3 V2.0 board and a hardware fix.

The hardware fix will use another PIN on the processor to replace the SWCLK line that is being reused by the board for the USB port. This hardware fix will free up the SWCLK pin to be used only for the SWD connector. The SKR MINI E3 2.0 board, disables the SWD connector's pins. If you preform this hardware hack you must also adjust the Marli firmware to match the hardware change.

The folders within the SKR mini E3 2.0 backup directory also contains another folder called "FW File needed to turn ON SWD connector" which contains the Marlin firmware that is needed to be installed on the SKR MINI E3 V2.0 board PRIOR to trying to back up the bootloader file.  This Marlin firmware or "FW File" enables the SWD connector pins (PA13 & PA14) so that the  ST-LINK-V2 offical device can connect to the processor. If this "FW File" is not placed onto a micro-SD card and placed inside the micro-SD card reader the ST-LINK-V2 probe device will NOT be allowed to connect to the processor because you have not enabled PA13 and PA14 yet.  Once you have backup your bootloader file you can compile your own version of Marlin firmware for you printer and upload it to the SKR CR6 V1.0 board via the onboard SD Card reader. I fact, I would not run a 3D printer with the FW File.  ST-LINK-V2 Clone devices will not work with this board, they appear to work (they save the file) but when you look at the file contents it is mostly all 0xFF.  Therefore, I have found that an official ST-LINK-V2 device from STelectronics will connect and properly save the files.  An official ST-LINK-V2 can be bought from Amazon for $37.00 [https://www.amazon.com/ST-LINK-V2-circuit-debugger-programmer/dp/B00SDTEM1I].
##### NOTE: I just determined that if you have the offical ST-LINK-V2 from STelectronics (not a clone) the offical device CAN connect to the board even tho the SWCLK and SWDIO lines are repurposed.  So try connecting to the SKR MINI E3 V2.0 board first to see if the ST-LINK-Utility on Windows will connect, if it connects then you are good to save the bootloader, if it does not connect then I would try to load the "FW File" located in subdirectory "FW File needed to turn ON SWD connector".

The SKR CR6 V1.0 board also repuposes the SWCLK line and SWDIO line for other signals on the board. Therefore I have provided you with the Marlin firmware that needs to be loaded on to the board PRIOR to you trying to back up the bootloader from the board. The Marlin firmware is a basic setup but it enables the PA13 and PA14 to work only with the SWD Connector. If this new firware is not placed onto a SD card and placed inside the SD card reader the ST-LINK-V2 probe device will NOT be allowed to connect to the processor because you have not enabled PA13 and PA14 yet.  This "FW File" is located in a subdirectory of the SRK CR6 V1.0 directory and is called "FW File needed to turn ON SWD connector".  Once you have backup your bootloader file you can compile your own version of Marlin firmware for you printer and upload it to the SKR CR6 V1.0 board via the onboard SD Card reader. I fact, I would not run a 3D printer with the FW File.  ST-LINK-V2 Clone devices will not work with this board, they appear to work (they save the file) but when you look at the file contents it is mostly all 0xFF.  Therefore, I have found that an official ST-LINK-V2 device from STelectronics will connect and properly save the files.  An official ST-LINK-V2 can be bought from Amazon for $37.00 [https://www.amazon.com/ST-LINK-V2-circuit-debugger-programmer/dp/B00SDTEM1I].
##### NOTE: I just determined that if you have the offical ST-LINK-V2 from STelectronics (not a clone) the offical device CAN connect to the board even tho the SWCLK and SWDIO lines are repurposed.  So try connecting to the SKR CR6 V1.0 board first to see if the ST-LINK-Utility on Windows will connect, if it connects then you are good to save the bootloader, if it does not connect then I would try to load the "FW File" located in subdirectory "FW File needed to turn ON SWD connector".


---

#### ~IMPORTANT NOTE: If you use the _backup_up_original_bootloader_ folder, you need to be aware that the _bootloader files_ on the following boards are the orignals _but_ the _Marlin firmware_ that is saved as part of the the xxxx-bootloader-and-firmware.bin or xxxx-bootloader-and-fimrware.hex files will work but when you reboot the board you might get a MIN TEMP error (SKR V1.3, SKR V1.4, SKR V1.4 Turbo, SKR PRO V1.1, and GTR V1.0), and the Marlin Firmware will cause the printer to shut off.~  I have bought brand new boards for the SKR V1.3, SKR V1.4 and SKR V1.4 TURBO and went and did a do over on ripping the bootloader and firmware files from those boards.  The bootloader and firmware files are what come straight from the factory. I you run into a problem with the SKR PRO V1.1 or GTR V1.0 board (getting a MIN TEMP error, the bootloaders are also as they came from the factory) then the way to solve this issue is for you to _recompile the Marlin Firmware_ from the release branch of Marlin on github.com and take your compiled firmware.bin file that was built for your printer and place it on the micro-SD card and put it into the onboard micro-SD card reader so that your version of Marlin will get up loaded to the board. If you can not up load via the micro-SD card you can upload your Marlin firmware via the SWD connector and a ST-LINK-V2 device.  Please look at the Bigtreetech boards Bootloader V2.0.pdf to see at which hex address the Marlin firmware begins for each board. THe GUIDES will explain how to restore files. The only thing that is different from the GUIDES is that you are uploading only the Marlin firmware which means the starting hex address will be different and the file uploaded will be different but the procedure is the same.

#### I was performing testing for Marlin concerning the MAX31865 board and PT100 and when I started this project. I apologize for any inconvience this might have caused.  The bootloader for each board are SOLID, and have never been changed so if you manage to up load the bootloader files and your board still does not boot then the next solution would be to upload your compiled Marlin firmware only.

#### I would appriciate people reporting any board might have issues. ~I know the SKR V1.4 Turbo board has Marlin firmware that will produce a MIN temp error.~ I bought another SKR V1.4 Turbo board and ripped the files again. User NezSez, who reported the issue about the SKR V1.4 Turbo board has sent me the bootloader-and-firmware.bin file that he dumped from his new SKR V1.4 Turbo board. So the SKR V1.4 Turbo now has two copies of the bootlaoder-and-firmware.bin files.  I did a hex comparison on the files and they are now exactly alike.  ~Once recieved I will update the backup_up_original_bootloader folder for SKR V1.4 Turbo board with this file.~ The SKR V1.4 Turbo bootloader-and-firmware.bin file was recieved by NezSez from github.com. Thank you NezSez for finding, reporting and helping fix the problem.

---

# The second purpose of this repository is to provide a way for a Marlin user to compile thier own custom bootloader for the SKR V1.3, SKR V1.4, SKR V1.4 Turbo or SKR E3 Turbo boards.

The compiled custom bootloader for the SKR V1.3, SKR V1.4, SKR V1.4 Turbo, and SKR E3 Turbo boards can be found: https://github.com/GadgetAngel/BTT_SKR_13_14_14T_SD-DFU-Bootloader/tree/main/bootloader_bin/compiled_bootloader_bin

The compiled custom bootloader folder is organized by the name of the BIGTREETECH board.

This folder also contains PDF files:
"Bigtreetech boards Bootloader_v2.0.pdf" which contains all the flash memory information layout for each BIGTREETECH boards.
"Guide_xxxxxxxxxx.pdf" which contains a guide on how to back up and restore these files to the SKR (LPC1768 or LPC1769) boards.

Each board folder contains two subfolders: "Bootloader ONLY" and "Bootloader_and_Fimware" folders.

1) Bootloader ONLY folder: contains the bootloader file that is the actual size of the boot loader. The name of the file is the "board_name-DFU-Bootloader-only.zip".  There are two files contained within the .zip folder which are named "board_name-bootloader-only.bin" and "board_name-DFU-Bootloader-only.hex".  If you have a choice to flash either file please use the .hex file because it knows where in flash memory the data needs to reside.  The reason the files are contained within a zip folder is to assist with downloading these types of files from the web.

2) Bootloader_and_Firmware folder: contains the bootloader-and-firmware file which is a copy of the full flash memory of the board. The name of the file is the "board_name-bootloader-and-firmware.zip" There are two files contained within the .zip folder which are named "board_name-bootloader-and-firmware.bin" and "board_name-bootloader-and-firmware.hex". If you have a choice to flash either file please use the .hex file because it knows where in flash memory the data needs to reside.  The reason the files are contained within a zip folder is to assist with downloading these types of files from the web.

## Bigtreetech SKR 1.3, 1.4 and 1.4 TURBO;

This is a fork of https://github.com/triffid/LPC17xx-DFU-Bootloader

Supports Bigtreetech SKR 1.3, SKR 1.4 and SKR 1.4 TURBO with the same binary.

## Bootloader for Bigtreetech SKR (mini) E3 TURBO board

This is a fork of https://github.com/ardiehl/BTT_SKR_13_14_14T_SD-DFU-Bootloader

Supports Bigtreetech SKR 1.3, SKR 1.4 and SKR 1.4 TURBO and SKR E3 TURBO with the same binary.

### SD card support
By default for the SKR V1.3, SKR V1.4 and SKR V1.4 Turbo the SD card (SPI 1) on the SKR as well as the SD card on a connected display board (SPI 0) (if connected) may be used for updating the firmware.

By defulat for the SKR E3 TURBO the SD card (SPI 1) is the ONBOARD SD card reader and is the ONLY SD card reader availble to the SKR E3 TURBO board.

As in the original code a file named "firmware.bin" on the FAT32-formatted SD card's first partition will be flashed to the application region (0x4000 after the 16k space for the bootloader), and after successful flashing, the file will be renamed to "firmware.cur" - 'cur' meaning 'current'.

### USB support

In addition, Flash via USB is enabled as default. It will be activated:
- if no firmware is installed (Flash @16K = 0xffffffff)
- if the button on a connected display board (e.g. BTT LCD-35 or RepRapDiscount Full Graphic Smart Controller) is pressed while booting. (for SKR V1.3,
SRK V1.4, or SKR V1.4 Turbo the pin 2 of EXP1 = 0.28 connected to GND, for SKR E3 Turbo pin 2 [in pins_BTT_SKR_E3_TURBO.h file it is pin 9] of EXP1 = 0.16
connected to GND)

### Upload via USB
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

### Silent beeper

For SKR V1.3, SKR V1.4, or SKR V1.4 Turbo: EXP1 pin 1 (1.30) will be configured as output and set to 0 to avoid endless beeping of the beeper on a connected RepRapDiscount Full Graphic Smart Controller.

For SKR (mini) E3 Turbo: EXP1 pin 1 (P2_08) will be configured as output and set to 0 to avoid endless beeping of the beeper on a connected RepRapDiscount Full Graphic Smart Controller.

### Installing the bootloader

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

Here is the URL for instructions on how I, GadgetAngel, got it to work: https://github.com/GadgetAngel/BTT_SKR_13_14_14T_SD-DFU-Bootloader/blob/main/bootloader_bin/compiled_bootloader_bin/SKR%20E3%20Turbo/Guide%20UART%20Flash%20SKR%20E3%20TURBO%20board.pdf

### Configuration
By default, the bootloader will show status/debug messages via the serial port @115200 baud. You can configure these as well as disableing USB, disabling the second SD card and so on in `config.h`.

### Why
@ardiehl, I managed to destroy the SD card slot on my 1.4 board and while trying to solder a new one, i destroyed some pads on the board. Insead of using another board i now use the SD card slot on the display board for firmware uploads.

@GadgetAngel, I bought a SKR E3 TURBO board and it produced a high pitch sound out of the Speaker connected to my CR10_STOCKDISPLAY from my original Ender 3 Pro.
This high pitch sound continued to be produced until Marlin boot screen was displayed onto the display screen.  I recompiled this project to pull the beeper_pin
low while the bootloader was running to ensure a silent boot process.