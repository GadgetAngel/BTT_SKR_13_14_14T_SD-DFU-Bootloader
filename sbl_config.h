/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    SBL_CONFIG.H
 *      Purpose: USB Flash updater
 *      Version: V1.0
 *----------------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *---------------------------------------------------------------------------*/

/***********************************************************************
 * Code Red Technologies - Minor modifications to original code for use
 * in RDB1768 secondary USB bootloader based on LPCUSB USB stack.
 * *********************************************************************/

#ifndef  _SBL_CONFIG_H
#define  _SBL_CONFIG_H

//*** <<< Use Configuration Wizard in Context Menu >>> ***

/*
// <h> Flash Configuration
//   <o0> User Start Sector <0-29>
//   <o1> Device Type
//   <7=>  LPC17x1 - 8 KB
//   <15=> LPC17x2 - 64 KB
//   <17=> LPC17x4 - 128 KB
//   <21=> LPC17x5/6 - 256 KB
//   <29=> LPC17x8 - 512 KB
//   <o2> Code Read Protection
//        <0x11223344=> NO CRP <0x12345678=> CRP1 <0x87654321=> CRP2 <0x43218765=> CRP3
// </h>
*/

// 16K = 4 sectors for the bootloader, user code starts at 0x4000
#define USER_START_SECTOR 4
#define MAX_USER_SECTOR 29

/*
// <h> Update Entry Pin
//   <o0> Port
//        <0x2009C014=> Port 0 <0x2009C034=> Port 1 <0x2009C054=> Port 2 <0x2009C074=> Port 3 <0x2009C094=> Port 4
//   <o1> Pin <0-31>
// </h>
*/

/*
 * CodeRed - joystick button is P0_6.
 */
#define ISP_ENTRY_GPIO_REG 0x2009C054  /* Port */
#define ISP_ENTRY_PIN 	   10          /* Pin  */

/*
 * CodeRed - change FLASH_BUF_SIZE from 256 to 512 to match buffer
 * size used by SCSI layer of LPCUSB
 */

#define SECTOR_START(sector)	((sector < 16)?( sector * 0x1000)         :( (sector - 14) * 0x8000)          )
#define SECTOR_END(sector)		((sector < 16)?((sector * 0x1000) + 0xFFF):(((sector - 14) * 0x8000) + 0x7FFF))

#define FLASH_BUF_SIZE 512
#define USER_FLASH_START SECTOR_START(USER_START_SECTOR)
#define USER_FLASH_END	 SECTOR_END(MAX_USER_SECTOR)
#define USER_FLASH_SIZE  ((USER_FLASH_END - USER_FLASH_START) + 1)
#define MAX_FLASH_SECTOR 30

/* Define start address of each Flash sector */
#define SECTOR_0_START      0x00000000
#define SECTOR_1_START      0x00001000
#define SECTOR_2_START      0x00002000
#define SECTOR_3_START      0x00003000
#define SECTOR_4_START      0x00004000
#define SECTOR_5_START      0x00005000
#define SECTOR_6_START      0x00006000
#define SECTOR_7_START      0x00007000
#define SECTOR_8_START      0x00008000
#define SECTOR_9_START      0x00009000
#define SECTOR_10_START     0x0000A000
#define SECTOR_11_START     0x0000B000
#define SECTOR_12_START     0x0000C000
#define SECTOR_13_START     0x0000D000
#define SECTOR_14_START     0x0000E000
#define SECTOR_15_START     0x0000F000
#define SECTOR_16_START     0x00010000
#define SECTOR_17_START     0x00018000
#define SECTOR_18_START     0x00020000
#define SECTOR_19_START     0x00028000
#define SECTOR_20_START     0x00030000
#define SECTOR_21_START     0x00038000
#define SECTOR_22_START     0x00040000
#define SECTOR_23_START     0x00048000
#define SECTOR_24_START     0x00050000
#define SECTOR_25_START     0x00058000
#define SECTOR_26_START     0x00060000
#define SECTOR_27_START     0x00068000
#define SECTOR_28_START     0x00070000
#define SECTOR_29_START     0x00078000



/* Define end address of each Flash sector */
#define SECTOR_0_END        0x00000FFF
#define SECTOR_1_END        0x00001FFF
#define SECTOR_2_END        0x00002FFF
#define SECTOR_3_END        0x00003FFF
#define SECTOR_4_END        0x00004FFF
#define SECTOR_5_END        0x00005FFF
#define SECTOR_6_END        0x00006FFF
#define SECTOR_7_END        0x00007FFF
#define SECTOR_8_END        0x00008FFF
#define SECTOR_9_END        0x00009FFF
#define SECTOR_10_END       0x0000AFFF
#define SECTOR_11_END       0x0000BFFF
#define SECTOR_12_END       0x0000CFFF
#define SECTOR_13_END       0x0000DFFF
#define SECTOR_14_END       0x0000EFFF
#define SECTOR_15_END       0x0000FFFF
#define SECTOR_16_END       0x00017FFF
#define SECTOR_17_END       0x0001FFFF
#define SECTOR_18_END       0x00027FFF
#define SECTOR_19_END       0x0002FFFF
#define SECTOR_20_END       0x00037FFF
#define SECTOR_21_END       0x0003FFFF
#define SECTOR_22_END       0x00047FFF
#define SECTOR_23_END       0x0004FFFF
#define SECTOR_24_END       0x00057FFF
#define SECTOR_25_END       0x0005FFFF
#define SECTOR_26_END       0x00067FFF
#define SECTOR_27_END       0x0006FFFF
#define SECTOR_28_END       0x00077FFF
#define SECTOR_29_END       0x0007FFFF

#endif  /* __SBL_CONFIG_H__ */
