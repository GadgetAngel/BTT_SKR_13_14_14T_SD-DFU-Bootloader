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

#include "usbhw.h"

#include <LPC17xx.h>

#include <lpc17xx_clkpwr.h>

#include "lpc17xx_usb.h"

#include <stdio.h>

#if !(defined DEBUG)
#define printf(...) do {} while (0)
#endif

/// pointers for callbacks to EP1-15 both IN and OUT
usb_callback_pointer EPcallbacks[30];

void usb_init(void)
{
	// enable USB hardware
	LPC_SC->PCONP |= CLKPWR_PCONP_PCUSB;

	// enable clocks
	LPC_USB->USBClkCtrl |= DEV_CLK_EN | AHB_CLK_EN;
	// wait for clocks to stabilise
	while (LPC_USB->USBClkSt != (DEV_CLK_ON | AHB_CLK_ON));

	// configure USBD+ and USBD-
	LPC_PINCON->PINSEL1 &= 0xc3ffffff;
	LPC_PINCON->PINSEL1 |= 0x14000000;

	SIE_Disconnect();

	// configure USB Connect
	LPC_PINCON->PINSEL4 &= 0xfffcffff;
	LPC_PINCON->PINSEL4 |= 0x00040000;
}

void usb_connect(void)
{
	LPC_USB->USBDevIntEn = DEV_STAT | EP_SLOW;
	usb_realise_endpoint(EP0IN, 64);
	usb_realise_endpoint(EP0OUT, 64);

	SIE_Connect();
}

void usb_disconnect(void)
{
	SIE_Disconnect();
}

void usb_set_callback(uint8_t bEP, usb_callback_pointer callback)
{
	EPcallbacks[EP(bEP)] = callback;
}

void usb_realise_endpoint(uint8_t bEP, uint16_t packet_size)
{
	__disable_irq();
	LPC_USB->USBDevIntClr = EP_RLZD;
	LPC_USB->USBReEp |= EP(bEP);
	LPC_USB->USBEpInd = EP2IDX(bEP);
	LPC_USB->USBMaxPSize = packet_size;
	__enable_irq();
	while (!(LPC_USB->USBDevIntSt & EP_RLZD));
	LPC_USB->USBDevIntClr = EP_RLZD;
	LPC_USB->USBEpIntEn |= EP(bEP);
}

int usb_read_packet(uint8_t bEP, void *buffer, int buffersize)
{
	int i = 0;
	int l = 0;
	uint32_t j = 0;
	__disable_irq();

	LPC_USB->USBCtrl = RD_EN | ((bEP & 0xF) << 2);
	while ((LPC_USB->USBRxPLen & 0x800) != 0x800);
	l = LPC_USB->USBRxPLen & 0x3FF;

	if (l > buffersize)
	{
// 		printf("Not enough room in buffer (got %d need %d), failing to read\n", buffersize, l);
		__enable_irq();
		return l;
	}

	if (l == 0)
		j = LPC_USB->USBRxData;
	else
	{
		for (i = 0; i < buffersize; i++)
		{
			if ((i & 3) == 0)
			{
				j = LPC_USB->USBRxData;
			}

			((uint8_t *) buffer)[i] = j & 0xFF;
			j >>= 8;

			if ((i & 3) == 3)
			{
				if (!(LPC_USB->USBCtrl & RD_EN))
				{
					SIE_SelectEndpoint(bEP);
					SIE_ClearBuffer();
					__enable_irq();
					return l;
				}
			}
		}
	}

	SIE_SelectEndpoint(bEP);
	SIE_ClearBuffer();
	__enable_irq();
	return l;
}

int usb_write_packet(uint8_t bEP, void *data, int packetlen)
{
	int i = 0;
	uint8_t *d = (uint8_t *) data;
	__disable_irq();
	LPC_USB->USBCtrl = WR_EN | ((bEP & 0xF) << 2);
	LPC_USB->USBTxPLen = packetlen;
	if (packetlen)
	{
		for (i = 0;(LPC_USB->USBCtrl & WR_EN) && (i < packetlen);)
		{
			// 		printf("[%x]",d);
			LPC_USB->USBTxData = ((d[0]) << 0) | ((d[1]) << 8) | ((d[2]) << 16) | ((d[3]) << 24);
			d += 4;
			i += 4;
		}
	}
	else
	{
		LPC_USB->USBTxData = 0;
	}
	SIE_SelectEndpoint(bEP);
	SIE_ValidateBuffer();
	__ISB();
	__enable_irq();
	if (i > packetlen)
		return packetlen;
	return i;
}

void usb_ep_stall(uint8_t bEP)
{
	SIE_SetEndpointStatus(bEP, SIE_EPST_ST);
}

void usb_ep_unstall(uint8_t bEP)
{
	SIE_SetEndpointStatus(bEP, 0);
}

void usb_ep0_stall(void)
{
	SIE_SetEndpointStatus(EP0OUT, SIE_EPST_CND_ST);
}

void usb_task(void)
{
	if (LPC_USB->USBDevIntSt & FRAME)
	{
// 		USBEvent_Frame(SIEgetFrameNumber());
		LPC_USB->USBDevIntClr = FRAME;
	}
	if (LPC_USB->USBDevIntSt & DEV_STAT)
	{
		LPC_USB->USBDevIntClr = DEV_STAT;

		uint8_t devStat = SIE_GetDeviceStatus();

		if (devStat & SIE_DEVSTAT_SUS_CH)
		{
// 			USBEvent_suspendStateChanged(devStat & SIE_DEVSTAT_SUS);
// 			printf("USB:Suspend\n");
		}

		if (devStat & SIE_DEVSTAT_RST)
		{
			printf("USB:Bus Reset\n");
			USBEvent_busReset();

			usb_realise_endpoint(EP0IN , 64);
			usb_realise_endpoint(EP0OUT, 64);

			SIE_SetMode(SIE_MODE_INAK_CI);
		}

		if (devStat & SIE_DEVSTAT_CON_CH)
		{
// 			printf("USB:Connect\n");
// 			USBEvent_connectStateChanged(devStat & SIE_DEVSTAT_CON);
		}
	}
	if (LPC_USB->USBDevIntSt & EP_SLOW)
	{
		int st = LPC_USB->USBEpIntSt;
// 		printf("INTST: %x\n", st);
		if (st & EP(EP0OUT))
		{
			int st;
			if ((st = SIE_SelectEndpointClearInterrupt(EP0OUT)) & SIE_EP_STP) // SETUP packet
			{
// 				printf("EP0SETUP_ST: 0x%x\n", st);
				// this is a setup packet
				EP0setup();
			}
			else if (st & SIE_EP_FE) // OUT endpoint and FE = 1 (buffer has data)
			{
// 				printf("EP0OUT_ST: 0x%x\n", st);
				EP0out();
			}
			else // EP0OUT, FE = 0 (no data) - why are we interrupting?
			{
				uint8_t b[8];
// 				int l = usb_read_packet(EP0OUT, b, 8);
				usb_read_packet(EP0OUT, b, 8);
// 				printf("EP0OUT: spurious interrupt (0x%x, %d)\n", st, l);
			}
		}
		if (st & EP(EP0IN))
		{
			int st = SIE_SelectEndpointClearInterrupt(EP0IN);
			if ((st & SIE_EP_FE) == 0)
				EP0in();
// 			else
// 				printf("Interrupt on EP0IN: FE:%d ST:%d STP:%d PO:%d EPN:%d B1:%d B2:%d\n", st & 1, (st >> 1) & 1, (st >> 2) & 1, (st >> 3) & 1, (st >> 4) & 1, (st >> 5) & 1, (st >> 6) & 1);
		}
		if (st & ~(3UL))
		{
			int i = 3;
			uint32_t bitmask = 1<<3;
			for (;
				i < 32;
				i++, bitmask <<= 1
			)
			{
				if (LPC_USB->USBEpIntSt & bitmask)
				{
					int st = SIE_SelectEndpointClearInterrupt(IDX2EP(i));
					if (
						((i & 1) && ((st & SIE_EP_FE) == 0)) || // IN endpoint and FE = 0 (space in buffer)
						(((i & 1) == 0) && (st & SIE_EP_FE))    // OUT endpoint and FE = 1 (buffer has data)
						)
					{
						// TODO: check for event receivers
					}
				}
			}
		}
		LPC_USB->USBDevIntClr = EP_SLOW;
	}
}

#ifndef __CC_ARM
__attribute__ ((interrupt)) void USB_IRQHandler() {
// 	usb_task();
}
#endif
