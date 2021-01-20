/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SDCard.h"
// #include "usbdisk.h"	/* Example: USB drive control */
// #include "atadrive.h"	/* Example: SDCard drive control */
// #include "sdcard.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each media */
#define SDCard		0
// #define MMC		1
// #define USB		2


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (drv) {
	case SDCard :
		result = SDCard_disk_initialize();

		// translate the reslut code here
		stat = result?RES_ERROR:RES_OK;

		return stat;

// 	case MMC :
// 		result = MMC_disk_initialize();
//
// 		// translate the reslut code here
//
// 		return stat;
//
// 	case USB :
// 		result = USB_disk_initialize();
//
// 		// translate the reslut code here
//
// 		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (drv) {
	case SDCard :
		result = SDCard_disk_status();

		// translate the reslut code here
		stat = result?RES_ERROR:RES_OK;

		return stat;

// 	case MMC :
// 		result = MMC_disk_status();
//
// 		// translate the reslut code here
//
// 		return stat;
//
// 	case USB :
// 		result = USB_disk_status();

		// translate the reslut code here

// 		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case SDCard :
		// translate the arguments here

		for (; count; count--, sector += 512)
			SDCard_disk_read(buff, sector);
		result = 0;

		// translate the reslut code here
		res = result?RES_ERROR:RES_OK;

		return res;

// 	case MMC :
// 		// translate the arguments here
//
// 		result = MMC_disk_read(buff, sector, count);
//
// 		// translate the reslut code here
//
// 		return res;
//
// 	case USB :
// 		// translate the arguments here
//
// 		result = USB_disk_read(buff, sector, count);
//
// 		// translate the reslut code here
//
// 		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case SDCard :
		// translate the arguments here

		for (; count; count--, sector += 512)
			SDCard_disk_write(buff, sector);
		result = 0;

		// translate the reslut code here
		res = result?RES_ERROR:RES_OK;

		return res;

// 	case MMC :
// 		// translate the arguments here
//
// 		result = MMC_disk_write(buff, sector, count);
//
// 		// translate the reslut code here
//
// 		return res;
//
// 	case USB :
// 		// translate the arguments here
//
// 		result = USB_disk_write(buff, sector, count);
//
// 		// translate the reslut code here
//
// 		return res;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result = 1;

	switch (drv) {
	case SDCard :
		// pre-process here
		switch(ctrl)
		{
			case CTRL_SYNC:
				result = 0;
				break;
			case GET_SECTOR_SIZE:
			{
				*((uint16_t *) buff) = SDCard_disk_blocksize();
				result = (*((uint16_t *) buff))?1:0;
				break;
			}
			case GET_SECTOR_COUNT:
			{
				*((uint32_t *) buff) = SDCard_disk_sectors();
				result = (*((uint32_t *) buff))?1:0;
				break;
			}
			case GET_BLOCK_SIZE:
			{
				*((uint32_t *) buff) = SDCard_disk_blocksize();
				result = (*((uint32_t *) buff))?1:0;
				break;
			}
			case CTRL_ERASE_SECTOR:
			{
// 				SDCard_disk_erase(((uint32_t *) buff)[0], ((uint32_t *) buff)[1] - ((uint32_t *) buff)[0] + 1);
				result = 0;
				break;
			}
		}

		// post-process here
		res = result?RES_ERROR:RES_OK;

		return res;

// 	case MMC :
// 		// pre-process here
//
// 		result = MMC_disk_ioctl(ctrl, buff);
//
// 		// post-process here
//
// 		return res;
//
// 	case USB :
// 		// pre-process here
//
// 		result = USB_disk_ioctl(ctrl, buff);
//
// 		// post-process here
//
// 		return res;
	}
	return RES_PARERR;
}
#endif
