#ifndef _GPIO_H
#define	_GPIO_H

#include <stdint.h>

#define	GPIO_DIR_INPUT 0
#define	GPIO_DIR_OUTPUT 1

#include "pins.h"

void GPIO_init(PinName);
void GPIO_initpulldown(PinName);

void GPIO_setup(PinName);
void GPIO_set_direction(PinName, uint8_t direction);
void GPIO_output(PinName);
void GPIO_input(PinName);
void GPIO_write(PinName, uint8_t value);
void GPIO_set(PinName);
void GPIO_clear(PinName);
uint8_t GPIO_get(PinName);

#endif /* _GPIO_H */
