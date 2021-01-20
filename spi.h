#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

#include "spi_hal.h"

#include "pins.h"


void SPI_init(PinName mosi, PinName miso, PinName sclk);

void SPI_frequency(uint32_t);
uint8_t SPI_write(uint8_t);

int SPI_writeblock(uint8_t *, int);

int SPI_can_DMA(void);
int setup_DMA_rx(DMA_REG *);
int setup_DMA_tx(DMA_REG *);

void SPI_irq(void);

typedef void (*fptr)(void);
extern fptr isr_dispatch[N_SPI_INTERRUPT_ROUTINES];


#endif /* _SPI_H */
