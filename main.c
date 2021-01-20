/*****************************************************************************
 *                                                                            *
 * DFU/SD/SDHC Bootloader for LPC17xx                                         *
 *                                                                            *
 * by Triffid Hunter                                                          *
 *                                                                            *
 * lightly modified GadgetAngel for BTT SKR (mini) E3 TURBO                   *
 *  - DFU can be included and will be launched if flash is empty or if the    *
 *    encoder button of the display board is pressed while booting            *
 *  - Bootloader will hold down the BEEPER_PIN on EXP1 while marlin boots to  *
 *    have a silent boot process                                              *
 *                                                                            *
 * lightly modified Armin Diehl for BTT SKR 1.3 and 1.4[T]                    *
 *  - DFU can be included and will be launched if flash is empty or if the    *
 *    encoder button of the display board is pressed while booting            *
 *  - Firware can also be loaded from the sdcard on the display board (on my  *
 *    board the sd card slot on the skr board was broken)                     *
 *                                                                            *
 * This firmware is Copyright (C) 2009-2010 Michael Moon aka Triffid_Hunter   *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 2 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 *                                                                            *
 *****************************************************************************/

#define VERSION "1.05 SKR"

__attribute__((used)) static const char * version = VERSION;

// AD 16 Nov 2020
//   added support for second sd card (on display port) connected to SPI 0
//   disabled DFU mode / pin

// A LOW level after reset at pin DFU_BTN is considered an external hardware 
// request to start the ISP command handler.


#include "usbhw.h"
#include "usbcore.h"

#include "uart.h"

#include "SDCard.h"

#include "gpio.h"

#include "sbl_iap.h"
#include "sbl_config.h"

#include "ff.h"

#include "dfu.h"

#include "min-printf.h"

#include "lpc17xx_wdt.h"

//---------------------------------
// pin definitions and defines
#include "config.h"
//--------------------------------

//#define printf(...) do {} while (0)

FATFS	fat;
FIL	file;

const char *firmware_file = "firmware.bin";
const char *firmware_old  = "firmware.cur";

#if ENABLED(LEDS)
void setleds(int leds)
{
	GPIO_write(LED1, leds &  1);
	GPIO_write(LED2, leds &  2);
	GPIO_write(LED3, leds &  4);
	GPIO_write(LED4, leds &  8);
	GPIO_write(LED5, leds & 16);
}
#endif

#if ENABLED(DFU)
int dfu_btn_pressed(void)
{
	return GPIO_get(DFU_BTN);
}

void start_dfu(void)
{
	DFU_init();
	usb_init();
	usb_connect();
	while (DFU_complete() == 0)
		usb_task();
	usb_disconnect();
}
#else
int dfu_btn_pressed(void)
{
	return 1;
}
#endif

int check_sd_firmware(void)
{
	int r;
// 	DBGPRINTF("Check SD\n");
	f_mount(0, &fat);
	if ((r = f_open(&file, firmware_file, FA_READ)) == FR_OK)
	{
 		DBGPRINTF("Flashing firmware...\n");
		uint8_t buf[512];
		unsigned int r = sizeof(buf);
		uint32_t address = USER_FLASH_START;
		while (r == sizeof(buf))
		{
			if (f_read(&file, buf, sizeof(buf), &r) != FR_OK)
			{
				f_close(&file);
				return 0;
			}
#if ENABLED(LEDS)
			setleds((address - USER_FLASH_START) >> 15);
#endif
// 			printf(PFX "\t0x%lx\n", address);

			write_flash((void *) address, (char *)buf, sizeof(buf));
			address += r;
		}
		f_close(&file);
		if (address > USER_FLASH_START)
		{
			DBGPRINTF("Success\n");
			r = f_unlink(firmware_old);
			r = f_rename(firmware_file, firmware_old);
			return 1;
		}
	} else {
		DBGPRINTF("%s not found\n", firmware_file);
	}
	return 0;
}

// this seems to fix an issue with handoff after poweroff
// found here http://knowledgebase.nxp.trimm.net/showthread.php?t=2869
typedef void __attribute__((noreturn))(*exec)();

static void boot(uint32_t a)
{
	uint32_t *start;

	__set_MSP(*(uint32_t *)USER_FLASH_START);
	start = (uint32_t *)(USER_FLASH_START + 4);
	((exec)(*start))();
}

static uint32_t delay_loop(uint32_t count)
{
	volatile uint32_t j, del;
	for(j=0; j<count; ++j){
		del=j; // volatiles, so the compiler will not optimize the loop
	}
	return del;
}

static void new_execute_user_code(void)
{
	uint32_t addr=(uint32_t)USER_FLASH_START;
	// delay
	delay_loop(3000000);
	// relocate vector table
	SCB->VTOR = (addr & 0x1FFFFF80);
	// switch to RC generator
	LPC_SC->PLL0CON = 0x1; // disconnect PLL0
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;
	while (LPC_SC->PLL0STAT&(1<<25));
	LPC_SC->PLL0CON = 0x0;    // power down
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;
	while (LPC_SC->PLL0STAT&(1<<24));
	// disable PLL1
	LPC_SC->PLL1CON   = 0;
	LPC_SC->PLL1FEED  = 0xAA;
	LPC_SC->PLL1FEED  = 0x55;
	while (LPC_SC->PLL1STAT&(1<<9));

	LPC_SC->FLASHCFG &= 0x0fff;  // This is the default flash read/write setting for IRC
	LPC_SC->FLASHCFG |= 0x5000;
	LPC_SC->CCLKCFG = 0x0;     //  Select the IRC as clk
	LPC_SC->CLKSRCSEL = 0x00;
	LPC_SC->SCS = 0x00;		    // not using XTAL anymore
	delay_loop(1000);
	// reset pipeline, sync bus and memory access
	__asm (
		   "dmb\n"
		   "dsb\n"
		   "isb\n"
		  );
	boot(addr);
}

int main(void)
{
	int flashed;
	int dfu;

#ifdef BEEPER_PIN
	GPIO_output(BEEPER_PIN); GPIO_write(BEEPER_PIN,0);
#endif

	flashed=0;
	WDT_Feed();
#if ENABLED(DFU)
	GPIO_init(DFU_BTN); GPIO_input(DFU_BTN);  // GPIO_init enables pullup as well
#endif

#if ENABLED(LEDS)
	GPIO_init(LED1); GPIO_output(LED1);
	GPIO_init(LED2); GPIO_output(LED2);
	GPIO_init(LED3); GPIO_output(LED3);
	GPIO_init(LED4); GPIO_output(LED4);
	GPIO_init(LED5); GPIO_output(LED5);
#endif

#if ENABLED(HEATER)
	// turn off heater outputs
	#error check/adjust pins for heater
	GPIO_init(P2_4); GPIO_output(P2_4); GPIO_write(P2_4, 0);
	GPIO_init(P2_5); GPIO_output(P2_5); GPIO_write(P2_5, 0);
	GPIO_init(P2_6); GPIO_output(P2_6); GPIO_write(P2_6, 0);
	GPIO_init(P2_7); GPIO_output(P2_7); GPIO_write(P2_7, 0);
#endif

#if ENABLED(LEDS)
	setleds(31);
#endif

	UART_init(UART_RX, UART_TX, APPBAUD);

	// give SD card time to wake up
	// AD: the card in the TFT35 needs more time
	//for (volatile int i = (1UL<<19); i; i--);
	DBGPRINTF("%s DFU:%d\n", version, DFU);

	// check if dfu button pressed to avoid checking SD card(s)
	dfu = (dfu_btn_pressed() == 0);

	if (! dfu) delay_loop(5000000);

// check onboard sd card
// P0_9: MOSI1 P0_8: MISO1 P0_7: SCK1 P0_6: SSEL1
	SDCard_init(P0_9, P0_8, P0_7, P0_6);
	if (SDCard_disk_initialize() == 0)
		flashed = check_sd_firmware();
	else DBGPRINTF("no sdcard\n");

#if ENABLED(SD_DISPLAYBOARD)
// check sd card on display board
// P0_18: MOSI0 P0_17: MISO0 P0_15: SCK0 P0_16: SSEL0
	if (! dfu) {		// do not check SD cards if dfu button pressed
		if (!flashed) {
			SDCard_init(P0_18, P0_17, P0_15, P0_16);
        		if (SDCard_disk_initialize() == 0)
                		flashed = check_sd_firmware();
			else {
				DBGPRINTF("no sdcard on display board\n");
			}
		}
	}
#endif  // SD_DISPLAYBOARD

#if ENABLED(DFU)
	if (dfu)
	{
		DBGPRINTF("ISP button pressed");
	}
	else if (WDT_ReadTimeOutFlag()) {
		WDT_ClrTimeOutFlag();
		DBGPRINTF("WATCHDOG reset");
		dfu++;
	} else if (*(uint32_t *)USER_FLASH_START == 0xFFFFFFFF) {
        	DBGPRINTF("User flash empty");
        	dfu++;
	}

	if (dfu) {
		DBGPRINTF_NOPFX(", entering DFU mode\n");
		start_dfu();
	}
#endif

#ifdef WATCHDOG
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);
	WDT_Start(1<<22);
#endif

	// grab user code reset vector
#if ENABLED(DEBUG_MESSAGES)
	DBGPRINTF("Jumping to 0x%x\n", (unsigned int)USER_FLASH_START+4);
#endif

	while (UART_busy());
	UART_deinit();

	new_execute_user_code();

#if ENABLED(DEBUG_MESSAGES)
    	UART_init(UART_RX, UART_TX, APPBAUD);
	printf(PFX "This should never happen\n");
	while (UART_busy());
#endif

	delay_loop(9000000);
	NVIC_SystemReset();
}


DWORD get_fattime(void)
{
#define	aYEAR	2020
#define aMONTH	11
#define aDAY	20
#define aHOUR	12
#define aMINUTE	42
#define aSECOND	42
	return	((aYEAR  & 127) << 25) |
		((aMONTH &  15) << 21) |
		((aDAY   &  31) << 16) |
		((aHOUR  &  31) << 11) |
		((aMINUTE & 63) <<  5) |
		((aSECOND & 63) <<  0);
}

int _write(int fd, const char *buf, int buflen)
{
	if (fd < 3)
	{
		while (UART_cansend() < buflen);
		return UART_send((const uint8_t *)buf, buflen);
	}
	return buflen;
}

void NMI_Handler() {
#if ENABLED(DEBUG_MESSAGES)
 	DBGPRINTF("NMI\n");
#endif
	for (;;);
}
void HardFault_Handler() {
#if ENABLED(DEBUG_MESSAGES)
	DBGPRINTF("HardFault\n");
#endif
	for (;;);
}
void MemManage_Handler() {
#if ENABLED(DEBUG_MESSAGES)
 	DBGPRINTF("MemManage\n");
#endif
	for (;;);
}
void BusFault_Handler() {
#if ENABLED(DEBUG_MESSAGES)
 	DBGPRINTF("BusFault\n");
#endif
	for (;;);
}
void UsageFault_Handler() {
	DBGPRINTF("UsageFault\n");
	for (;;);
}
