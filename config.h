/*****************************************************************************
 *                                                                            *
 * DFU/SD/SDHC Bootloader for LPC17xx                                         *
 *                                                                            *
 *****************************************************************************/

#pragma once

#include "macros.h"

// Baudrate for debug messages
#ifndef APPBAUD
#define APPBAUD 115200
#endif

// if 1, serial messages will be printed out
#ifndef DEBUG_MESSAGES
  #define DEBUG_MESSAGES 1
#endif

// if 1, serial debug messages for the USB part will be printed as well, needs DEBUG_MESSAGES
#ifndef DEBUG_MESSAGES_DFU
  #define DEBUG_MESSAGES_DFU 0
#endif


// if defined, the original led code is includes (useless for the BTT SKR boards)
#ifndef LEDS
  #define LEDS 0
#endif

// originally set for 1 for the SKR V1.3/V1.4/V1.4Turbo but for the SKR E3 Turbo it does not have two EXP connectors
//the SKR E3 TURBO on has one EXP connector for the display which means it is more like an EXP3 connector which
//combines the fuction of EXP1 and EXP2 into one connector but you do not have the SD card reader on the LCD display (0)
//
// if 1, sdcard on display board will be checked as well
#ifndef SD_DISPLAYBOARD
  #define SD_DISPLAYBOARD 0
#endif

// this was from the original code, not needed on SKR 1.3/1.4 as there are pulldown resistors installed
//#define HEATER

// if 1, USB client code will be included
#ifndef DFU
  #define DFU 1
#endif

// if set, debug outputs will be prefixed by "echo: " instead of "Boot: "
#ifndef OUTPUT_ECHO
  #define OUTPUT_ECHO 0
#endif

// for SKR V1.3, SKR V1.4 and SKR V1.4 Turbo boards: 
// BTN_ENC from the display board (BTT TFT35 or RepRap Full Graphic Smart 
// Controller) is on pin 2 of EXP1 -> 0.28 (SKR 1.3 and 1.4), pressing the 
// button while resetting forces DFU mode if no valid sd card containing
// firmware.bin was found
//
// originally set to P0_28 for SKR V1.3/ V1.4/ V1.4 Turbo board
//
// for the SKR E3 TURBO board: 
// BTN_ENC from the display board (BTT TFT35 or RepRap Full Graphic Smart
// Controller)  is on pin 2 of EXP1 -> 0.16 (SKR E3 TURBO), pressing the 
// button while resetting forces DFU mode if no valid sd card containing
// firmware.bin was found (P0_16)
#ifndef DFU_PIN
  #define DFU_BTN     P0_16
#endif

// For the SKR V1.3, SKR V1.4 and SKR V1.4Turbo boards:
// beeper on RepRap Full Graphic Smart Controller will beep on start due
// to pullup or some other source, let's try to configure as input with
// pulldown to avoid beeps during start (if defined)
// EXP1 pin 1 -> 1.30 (P1_30)
//
// originally set for P1_30 for SKR V1.3/ V1.4/V1.4 Turbo 
//
// For the SKR E3 TURBO board:
// beeper on RepRap Full Graphic Smart Controller will beep on start due
// to pullup or some other source, let's try to configure as input with
// pulldown to avoid beeps during start (if defined)
// EXP1 pin 1 -> 2.08 (P2_8)
#ifndef BEEPER_PIN
  #define BEEPER_PIN	P2_8
#endif

// SPI speed for SD card, defaults to 3 MHz (4 MHz does not work on 1.4 TURBO with BTT TFT-35)
#ifndef SPI_SPEED
  #define SPI_SPEED 2000000
#endif


//----------------------------------------------------------------------------

#if ENABLED(OUTPUT_ECHO)
  #define PFX "echo: "
  #define EPFX "error: "
#else
  #define PFX "Boot: "
  #define EPFX "Boot-error: "
#endif


