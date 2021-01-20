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

#include "gpio.h"

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

void GPIO_init(PinName pin) {
    GPIO_setup(pin);
}


void GPIO_setup(PinName pin) {
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	PinCfg.Portnum = PORT(pin);
	PinCfg.Pinnum = PIN(pin);
	PINSEL_ConfigPin(&PinCfg);
}

void GPIO_set_direction(PinName pin, uint8_t direction) {
	FIO_SetDir(PORT(pin), 1UL << PIN(pin), direction);
}

void GPIO_output(PinName pin) {
	GPIO_set_direction(pin, 1);
}

void GPIO_input(PinName pin) {
	GPIO_set_direction(pin, 0);
}

void GPIO_write(PinName pin, uint8_t value) {
	GPIO_output(pin);
	if (value)
		GPIO_set(pin);
	else
		GPIO_clear(pin);
}

void GPIO_set(PinName pin) {
	FIO_SetValue(PORT(pin), 1UL << PIN(pin));
}

void GPIO_clear(PinName pin) {
	FIO_ClearValue(PORT(pin), 1UL << PIN(pin));
}

uint8_t GPIO_get(PinName pin) {
	return (FIO_ReadValue(PORT(pin)) & (1UL << PIN(pin)))?255:0;
}
