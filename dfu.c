/*****************************************************************************
 *                                                                            *
 * DFU/SD/SDHC Bootloader for LPC17xx                                         *
 *                                                                            *
 * by Triffid Hunter                                                          *
 *                                                                            *
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


#include "config.h"

#include "dfu.h"

#include <stdio.h>

#include "usbcore.h"
#include "usbhw.h"

#include "descriptor.h"

#include "sbl_iap.h"

#include "string.h"

#define DFU_BLOCK_SIZE 512

#if ENABLED(LEDS)
extern voidsetleds(int);
#endif

typedef struct
__attribute__ ((packed))
{
	usbdesc_device device;
	usbdesc_configuration configuration;
	usbdesc_interface	interface;
	DFU_functional_descriptor dfufunc;
	usbdesc_language lang;
	usbdesc_string_l(11) iManufacturer;
	usbdesc_string_l(11) iProduct;
	usbdesc_string_l(12) iInterface;
	usbdesc_base endnull;
} DFU_APP_Descriptor;

DFU_APP_Descriptor desc =
{
	{
		DL_DEVICE,
		DT_DEVICE,
		USB_VERSION_2_0,	// bcdUSBVersion
		0,							// bDeviceClass
		0,							// bDeviceSubClass
		0,							// bDeviceProtocol
		64,						// bMaxPacketSize
		0x1D50,					// idVendor
		0x6015,					// idProduct
		0x0040,					// bcdDevice (serial number)
		1,							// iManufacturer
		2,							// iProduct
		0,							// iSerial
		1							// bNumConfigurations
	},
	{
		DL_CONFIGURATION,
		DT_CONFIGURATION,
		DL_CONFIGURATION + DL_INTERFACE + DL_DFU_FUNCTIONAL_DESCRIPTOR,
		1,							// bNumInterfaces
		1,							// bConfigurationValue
		0,							// iConfiguration
		CA_BUSPOWERED,	// bmAttributes
		500 mA					// bMaxPower
	},
	{
		DL_INTERFACE,
		DT_INTERFACE,
		0,							// bInterfaceNumber
		0,							// bAlternate
		0,							// bNumEndpoints
		DFU_INTERFACE_CLASS,		// bInterfaceClass
		DFU_INTERFACE_SUBCLASS,		// bInterfaceSubClass
		DFU_INTERFACE_PROTOCOL_DFUMODE,		// bInterfaceProtocol
		3							// iInterface
	},
	{
		DL_DFU_FUNCTIONAL_DESCRIPTOR,
		DT_DFU_FUNCTIONAL_DESCRIPTOR,
		DFU_BMATTRIBUTES_WILLDETACH | DFU_BMATTRIBUTES_CANDOWNLOAD | DFU_BMATTRIBUTES_CANUPLOAD,
		500,						// wDetachTimeout - time in milliseconds between receiving detach request and issuing usb reset
		DFU_BLOCK_SIZE,				// wTransferSize - the size of each packet of firmware sent from the host via control transfers
		DFU_VERSION_1_1	// bcdDFUVersion
	},
	{
		DL_LANGUAGE,
		DT_LANGUAGE,
		{ SL_USENGLISH }
	},
#ifndef __CC_ARM
	//usbstring(12, "SmoothieWare"),
	usbstring(11, "Bigtreetech"),

	//usbstring(8 , "Smoothie"),
	usbstring(11 ,  "SKR 1.3/1.4"),
	usbstring(12, "Smoothie DFU"),
#else
    //{ 2 + 2 * 12, DT_STRING, 'S', 'm', 'o', 'o', 't', 'h', 'i', 'e', 'W', 'a', 'r', 'e' },
    { 2 + 2 * 11, DT_STRING, 'B', 'i', 'g', 't', 'r', 'e', 'e', 't', 'e', 'c', 'h' },

    //{ 2 + 2 * 8,  DT_STRING, 'S', 'm', 'o', 'o', 't', 'h', 'i', 'e' },
    { 2 + 2 * 8,  DT_STRING, 'S', 'K', 'R', ' ', '1', '.', 'x', ' ' },

    { 2 + 2 * 12, DT_STRING, 'S', 'm', 'o', 'o', 't', 'h', 'i', 'e', ' ', 'D', 'F', 'U' },
#endif
	{
		0,							// bLength
		0							// bDescType
	}
};

typedef struct
__attribute__ ((packed))
{
	uint8_t		bStatus;			// status of most recent command
	uint32_t	bwPollTimeout:24;	// time to next poll
	uint8_t		bState;				// state that we're about to enter
	uint8_t		iString;			// optional string description for status
} DFU_STATUS_t;

typedef enum
{
	OK,					// No error
	errTARGET,			// File is not for this device
	errFILE,			// File is for this device but isn't right for some vendor-specific reason
	errWRITE,			// unable to write
	errERASE,			// erase failed
	errCHECK_ERASED,	// erase seemed to work but there's still data there
	errPROG,			// program memory function failed
	errVERIFY,			// verification failed
	errADDRESS,			// address out of range
	errNOTDONE,			// received end-of-data marker but we don't think we're finished yet
	errFIRMWARE,		// downloaded firmware is corrupt, can't exit DFU mode
	errVENDOR,			// iString indicates vendor specific error
	errUSBR,			// received unexpected USB reset
	errPOR,				// unexpected power-on reset
	errUNKNOWN,			// something went wrong, we just don't know what!
	errSTALLEDPKT		// device stalled something unexpected
} DFU_STATUS_enum;

typedef enum
{
	appIDLE,			// device is running firmware
	appDETACH,			// device is waiting for USB reset to begin DFU
	dfuIDLE,			// DFU mode is waiting for commands
	dfuDNLOADSYNC,		// device has a block and is waiting for host to issue GETSTATUS
	dfuDNBUSY,			// device is flashing a block
	dfuDNLOADIDLE,		// device is waiting for more data
	dfuMANIFESTSYNC,	// device is waiting GETSTATUS so we can enter or exit manifestation
	dfuMANIFEST,		// device is flashing
	dfuMANIFESTWAITRESET,	// device has finished flashing and is waiting for USB reset
	dfuUPLOADIDLE,		// device is waiting for UPLOAD requests
	dfuERROR,			// device has experienced an error, is waiting for CLRSTATUS
} DFU_STATE_enum;

DFU_STATE_enum current_state;

DFU_STATUS_t DFU_status = {
	OK,
	500,
	dfuIDLE,
	0
};

uint8_t block_buffer[DFU_BLOCK_SIZE];
const uint8_t * flash_p;

#ifndef __CC_ARM
extern const uint8_t _user_flash_start;
extern const uint8_t _user_flash_size;
#else
// static const uint32_t *_real_flash_size = (uint32_t *)0x7c000;
const uint8_t _user_flash_start __attribute__((at(0x4000)));
const uint8_t _user_flash_size __attribute__((at(0x7c000)));
#endif

#include "LPC17xx.h"
#include "lpc17xx_usb.h"

void DFU_init(void)
{
	usb_provideDescriptors(&desc);
	flash_p = &_user_flash_start;
// 	printf(PFX "user flash: %p\n", flash_p);
}

void DFU_GetStatus(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
	printf(PFX "DFU:GETSTATUS\n");
#endif
	control->buffer = &DFU_status;
	control->bufferlen = 6;
}

void DFU_GetState(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
	printf(PFX "DFU:GETSTATE\n");
#endif
	control->buffer = &current_state;
	control->bufferlen = 1;
}

void DFU_Download(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
	printf(PFX "DFU:DNLOAD\n");
#endif
	control->buffer = block_buffer;
	control->bufferlen = control->setup.wLength;

	flash_p = (&_user_flash_start) + (control->setup.wValue * DFU_BLOCK_SIZE);

	if (control->setup.wLength > 0)
	{
// 		printf(PFX "WRITE: %p\n", flash_p);
		if ((flash_p + control->setup.wLength) <= ((&_user_flash_start) + ((uint32_t)(&_user_flash_size))))
		{
			current_state = dfuDNLOADSYNC;
			DFU_status.bState = dfuDNLOADIDLE;
		}
		else {
			current_state = dfuERROR;
			DFU_status.bStatus = errADDRESS;
			DFU_status.bState = dfuERROR;
		}
	}
}

void DFU_Upload(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
//	printf(PFX "DFU:UPLOAD\n");
#endif
	current_state = dfuUPLOADIDLE;
	flash_p = &_user_flash_start + (control->setup.wValue * DFU_BLOCK_SIZE);
	if ((flash_p + control->setup.wLength) <= ((&_user_flash_start) + ((uint32_t)(&_user_flash_size))))
	{
		control->buffer = (uint8_t *) flash_p;
		control->bufferlen = control->setup.wLength;
	}
	else
	{
#if ENABLED(DEBUG_MESSAGES_DFU)
		printf(PFX "Upload Complete\n");
#endif
		control->bufferlen = 0;
		control->zlp = 1;
	}
}

void DFU_ClearStatus(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
	printf(PFX "DFU:CLRSTATUS\n");
#endif
	DFU_status.bStatus = OK;
	DFU_status.bState = dfuIDLE;
	flash_p = &_user_flash_start;
}

void DFU_Abort(CONTROL_TRANSFER *control)
{
#if ENABLED(DEBUG_MESSAGES_DFU)
	printf(PFX "DFU:ABORT\n");
#endif
	DFU_status.bStatus = OK;
	DFU_status.bState = dfuIDLE;
	flash_p = &_user_flash_start;
}

void DFU_controlTransfer(CONTROL_TRANSFER *control)
{
	// 0x20 is CLASS request
	// 0x01 is INTERFACE target
	// MSBit is transfer direction
	if ((control->setup.bmRequestType & 0x7F) == 0x21)
	{
		switch(control->setup.bRequest)
		{
			case DFU_DETACH:
				// shouldn't happen, we're already in DFU mode
				break;
			case DFU_DNLOAD:
				DFU_Download(control);
				break;
			case DFU_UPLOAD:
				DFU_Upload(control);
				break;
			case DFU_GETSTATUS:
				DFU_GetStatus(control);
				break;
			case DFU_CLRSTATUS:
				DFU_ClearStatus(control);
				break;
			case DFU_GETSTATE:
				DFU_GetState(control);
				break;
			case DFU_ABORT:
				DFU_Abort(control);
				break;
		}
	}
}

void DFU_transferComplete(CONTROL_TRANSFER *control)
{
	if ((control->setup.bmRequestType & 0x7F) == 0x21)
	{
		switch(control->setup.bRequest)
		{
			case DFU_GETSTATUS:
			{
				current_state = DFU_status.bState;
#if ENABLED(DEBUG_MESSAGES_DFU)
				printf(PFX "new state is %d\n", current_state);
#endif
				if (current_state == dfuMANIFESTWAITRESET)
				{
					usb_disconnect();
#if ENABLED(DEBUG_MESSAGES_DFU)
					printf(PFX "MANIFEST COMPLETE, usb disconnected\n");
#endif
				}

				break;
			}
			case DFU_DNLOAD:
			{
				if (control->setup.wLength > 0)
				{
#if ENABLED(DEBUG_MESSAGES_DFU)
					printf(PFX "WRITE %p\n", flash_p);
#endif
#if ENABLED(LEDS)
					setleds(((uint32_t) (flash_p - 0x4000)) >> 15);
#endif
					// we must pass DFU_BLOCK_SIZE to write_flash for some reason, it does not flash if we pass a smaller length
					int r = write_flash((void *) flash_p, (char *) block_buffer, DFU_BLOCK_SIZE);
// 					int r;
// 					for (r = 0; r < control->setup.wLength; r++)
// 					{
// 						printf(PFX "0x%x ", flash_p[r]);
// 						if ((r & 31) == 31)
// 							printf("\n");
// 					}
					if (r == 0)
					{
						flash_p += control->setup.wLength;
						DFU_status.bState = dfuDNLOADIDLE;
					}
					else
					{
#if ENABLED(DEBUG_MESSAGES_DFU)
						printf(PFX "write flash error %d\n", r);
#endif
						DFU_status.bStatus = errPROG;
						DFU_status.bState = dfuERROR;
					}
				}
				else
				{
					current_state = dfuMANIFESTSYNC;
					DFU_status.bState = dfuMANIFESTWAITRESET;
				}
				break;
			}
			case DFU_UPLOAD:
				DFU_status.bState = dfuUPLOADIDLE;
				flash_p += control->setup.wLength;
				break;
		}
	}
}

int DFU_complete(void)
{
	return (current_state == dfuMANIFESTWAITRESET);
}

void USBEvent_busReset(void)
{
	if (current_state == dfuMANIFESTWAITRESET || current_state == dfuMANIFESTSYNC ||current_state == dfuMANIFEST)
	{
		usb_disconnect();
		NVIC_SystemReset();
	}
}
