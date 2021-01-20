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

#include "lpc17xx_usb.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <LPC17xx.h>

void SIE_CMD(uint8_t cmd)
{
	LPC_USB->USBDevIntClr = CCEMPTY | CDFULL;
	LPC_USB->USBCmdCode = CMD_CODE(cmd) | CMD_PHASE_COMMAND;
	while (!(LPC_USB->USBDevIntSt & CCEMPTY));
// 	LPC_USB->USBDevIntClr = CCEMPTY;
}

uint8_t SIE_CMD_READ(uint8_t cmd)
{
	LPC_USB->USBDevIntClr = CDFULL;
	LPC_USB->USBCmdCode = CMD_CODE(cmd) | CMD_PHASE_READ;
	while (!(LPC_USB->USBDevIntSt & CDFULL));
// 	LPC_USB->USBDevIntClr = CDFULL;
	return LPC_USB->USBCmdData;
}

void SIE_CMD_WRITE(uint8_t data)
{
	LPC_USB->USBDevIntClr = CCEMPTY;
	LPC_USB->USBCmdCode = CMD_WDATA(data) | CMD_PHASE_WRITE;
	while (!(LPC_USB->USBDevIntSt & CCEMPTY));
// 	LPC_USB->USBDevIntClr = CCEMPTY;
}

void		SIE_SetAddress(uint8_t address)
{
	__disable_irq();
	SIE_CMD(SIE_DEV_CMD_SetAddress);
	SIE_CMD_WRITE((address & SIE_SETADDR_DEV_ADDR_MASK) | SIE_SETADDR_DEV_EN);
	__enable_irq();
}

void		SIE_ConfigureDevice(uint8_t conf_device)
{
	__disable_irq();
	SIE_CMD(SIE_DEV_CMD_Configure);
	SIE_CMD_WRITE(conf_device);
	__enable_irq();
}

void		SIE_SetMode(uint8_t mode)
{
	__disable_irq();
	SIE_CMD(SIE_DEV_CMD_SetMode);
	SIE_CMD_WRITE(mode);
	__enable_irq();
}

uint16_t	SIE_ReadCurrentFrameNumber(void)
{
	uint16_t r;
	__disable_irq();
	          SIE_CMD(SIE_DEV_CMD_ReadFrameNo);
	r =  SIE_CMD_READ(SIE_DEV_CMD_ReadFrameNo);
	r |= SIE_CMD_READ(SIE_DEV_CMD_ReadFrameNo) << 8;
	__enable_irq();
	return r;
}

uint16_t	SIE_ReadTestRegister(void)
{
	uint16_t r;
	__disable_irq();
	      SIE_CMD(SIE_DEV_CMD_ReadTestReg);
	r =  SIE_CMD_READ(SIE_DEV_CMD_ReadTestReg);
	r |= SIE_CMD_READ(SIE_DEV_CMD_ReadTestReg) << 8;
	__enable_irq();
	return r;
}

void		SIE_SetDeviceStatus(uint8_t status)
{
	__disable_irq();
	SIE_CMD(SIE_DEV_CMD_SetDevStatus);
	SIE_CMD_WRITE(status);
	__enable_irq();
}

uint8_t		SIE_GetDeviceStatus(void)
{
	uint8_t r;
	__disable_irq();
	     SIE_CMD(SIE_DEV_CMD_GetDevStatus);
	r = SIE_CMD_READ(SIE_DEV_CMD_GetDevStatus);
	__enable_irq();
	return r;
}

uint8_t		SIE_GetErrorCode(void)
{
	uint8_t r;
	__disable_irq();
	     SIE_CMD(SIE_DEV_CMD_GetErrorCode);
	r = SIE_CMD_READ(SIE_DEV_CMD_GetErrorCode);
	__enable_irq();
	return r;
}

uint8_t		SIE_ReadErrorStatus(void)
{
	uint8_t r;
	__disable_irq();
	     SIE_CMD(SIE_DEV_CMD_ReadErrorSt);
	r = SIE_CMD_READ(SIE_DEV_CMD_ReadErrorSt);
	__enable_irq();
	return r;
}

// endpoint commands
uint8_t		SIE_SelectEndpoint(uint8_t bEP)
{
	uint8_t r;
	__disable_irq();
	         SIE_CMD(SIE_EP_CMD_Select | (EP2IDX(bEP) & 0x1F));
	r = SIE_CMD_READ(SIE_EP_CMD_Select | (EP2IDX(bEP) & 0x1F));
	__enable_irq();
	return r;
}

uint8_t		SIE_SelectEndpointClearInterrupt(uint8_t bEP)
{
	uint8_t r;
	__disable_irq();
// 	         SIE_CMD(SIE_EP_CMD_SelectClearInt | (EP2IDX(bEP) & 0x1F));
// 	r = SIE_CMD_READ(SIE_EP_CMD_SelectClearInt | (EP2IDX(bEP) & 0x1F));
	LPC_USB->USBEpIntClr = EP(bEP);
	while (!(LPC_USB->USBDevIntSt & CDFULL));
	r = LPC_USB->USBCmdData;
	__enable_irq();
	return r;
}

void		SIE_SetEndpointStatus(uint8_t bEP, uint8_t status)
{
	__disable_irq();
	SIE_CMD(SIE_EP_CMD_SetEpStatus | (EP2IDX(bEP) & 0x1F));
	SIE_CMD_WRITE(status);
	__enable_irq();
}

uint8_t		SIE_ClearBuffer(void)
{
	uint8_t r;
	__disable_irq();
	     SIE_CMD(SIE_EP_CMD_ClearBuffer);
	r = SIE_CMD_READ(SIE_EP_CMD_ClearBuffer);
	__enable_irq();
	return r;
}

void		SIE_ValidateBuffer(void)
{
	__disable_irq();
	SIE_CMD(SIE_EP_CMD_ValidateBuffer);
	__enable_irq();
}

void		SIE_Connect(void)
{
	uint8_t status = SIE_GetDeviceStatus();
	SIE_SetDeviceStatus(status |  SIE_DEVSTAT_CON);
}

void		SIE_Disconnect(void)
{
	uint8_t status = SIE_GetDeviceStatus();
	SIE_SetDeviceStatus(status & ~SIE_DEVSTAT_CON);
}


#ifdef __cplusplus
} /* extern "C" */
#endif
