#ifndef _UART_H
#define	_UART_H

#include <stdint.h>

#include "pins.h"

#include "lpc17xx_uart.h"

#define UART_RINGBUFFER_SIZE 512

typedef struct
{
    volatile uint32_t head;                /*!< UART ring buffer head index */
    volatile uint32_t tail;                /*!< UART ring buffer tail index */
    volatile uint8_t  data[UART_RINGBUFFER_SIZE];  /*!< UART Tx data ring buffer */
} UART_RINGBUFFER_T;

// UART_init(PinName rxpin, PinName txpin);
void		UART_init(PinName rxpin, PinName txpin, int baud);
void        UART_deinit(void);
void		UART_pin_init(PinName rxpin, PinName txpin);
int			UART_baud(int baud);
uint32_t	UART_send(const uint8_t *buf, uint32_t buflen);
// uint32_t	UART_send(const char *buf, uint32_t buflen);
uint32_t	UART_recv(uint8_t *buf, uint32_t buflen);
int			UART_cansend(void);
int			UART_canrecv(void);
int			UART_busy(void);
void		UART_isr(void);
void		UART_tx_isr(void);
void		UART_rx_isr(void);
void		UART_err_isr(uint8_t);

#endif /* _UART_H */
