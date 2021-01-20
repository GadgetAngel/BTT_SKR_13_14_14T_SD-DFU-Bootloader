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

#include "uart.h"

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "config.h"

// #include "debug.h"

// void dbgled(int l);
// void setled(int, int);

#ifndef ENTER_ISR
#define ENTER_ISR() do {} while (0)
#endif

#ifndef LEAVE_ISR
#define LEAVE_ISR() do {} while (0)
#endif

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)     (bufidx=0)
#define __BUF_INCR(bufidx)      (bufidx=(bufidx+1)&__BUF_MASK)

#define RB_MASK          (UART_RINGBUFFER_SIZE - 1)
#define RB_FULL(rb)      ((rb.tail & RB_MASK) == ((rb.head + 1) & RB_MASK))
#define RB_EMPTY(rb)     ((rb.head & RB_MASK) == ( rb.tail      & RB_MASK))
#define RB_ZERO(rb)      do { rb.head = rb.tail = 0; } while (0)
#define RB_INCR(ht)      do { ht = (ht + 1) & RB_MASK; } while (0)
#define RB_PUSH(rb, val) do { rb.data[rb.head++] = val; rb.head &= RB_MASK; } while (0)
#define RB_POP( rb, val) do { val = rb.data[rb.tail++]; rb.tail &= RB_MASK; } while (0)
#define RB_PEEK(rb, val) do { val = rb.data[rb.tail++]; } while (0)
#define RB_DROP(rb)      do { rb.tail = (rb.tail + 1) & RB_MASK; } while (0)
#define RB_CANREAD(rb)   (((UART_RINGBUFFER_SIZE + rb.head) -  rb.tail     ) & RB_MASK)
#define RB_CANWRITE(rb)  (((UART_RINGBUFFER_SIZE + rb.tail) - (rb.head + 1)) & RB_MASK)

LPC_UART_TypeDef * u;
// UART Ring buffers
UART_RINGBUFFER_T txbuf;
UART_RINGBUFFER_T rxbuf;

// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;
volatile uint8_t blocking;
#define true 1
#define false 0

int port;

void UART_init(PinName rxpin, PinName txpin, int baud)
{
    UART_pin_init(rxpin, txpin);
    UART_baud(baud);
}

void UART_pin_init(PinName rxpin, PinName txpin)
{
    blocking = true;

    PINSEL_CFG_Type PinCfg;

    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;

    if (txpin == P0_2 && rxpin == P0_3) {
        port = 0;
        u = LPC_UART0;
        PinCfg.Funcnum = 1;
    }
    else if (txpin == P0_0 && rxpin == P0_1) {
        port = 3;
        u = LPC_UART3;
        PinCfg.Funcnum = 2;
    }
    else if (txpin == P0_10 && rxpin == P0_11) {
        port = 2;
        u = LPC_UART2;
        PinCfg.Funcnum = 1;
    }
    else if (txpin == P0_15 && rxpin == P0_16) {
        port = 1;
        u = (LPC_UART_TypeDef *) LPC_UART1;
        PinCfg.Funcnum = 1;
    }
    else if (txpin == P0_25 && rxpin == P0_26) {
        port = 3;
        u = LPC_UART3;
        PinCfg.Funcnum = 3;
    }
    else if (txpin == P2_0 && rxpin == P2_1) {
        port = 1;
        u = (LPC_UART_TypeDef *) LPC_UART1;
        PinCfg.Funcnum = 2;
    }
    else if (txpin == P2_8 && rxpin == P2_9) {
        port = 2;
        u = LPC_UART2;
        PinCfg.Funcnum = 2;
    }
    else if (txpin == P4_28 && rxpin == P4_29) {
        port = 3;
        u = LPC_UART3;
        PinCfg.Funcnum = 3;
    }
    else {
        //TODO: software serial
        port = -1;
        return;
    }

    PinCfg.Portnum = (txpin >> 5) & 7;
    PinCfg.Pinnum = (txpin & 0x1F);
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Portnum = (rxpin >> 5) & 7;
    PinCfg.Pinnum = (rxpin & 0x1F);
    PINSEL_ConfigPin(&PinCfg);
}

typedef struct {
	uint32_t	baud;
	uint8_t	pd;
	uint16_t	dl;
	uint8_t	mulval;
	uint8_t	divaddval;
} uart_regs;

// prevent system_LPC17xx from polluting our namespace
#undef __LPC17XX__
// pull in system_LPC17xx so we can see __CORE_CLK
#include <system_LPC17xx.c>
#define __LPC17XX__

// check if we can use precalculated values
#if ((defined APPBAUD) && (__CORE_CLK == 120000000) && (APPBAUD == 2000000))
static inline int baud_space_search(uint32_t target_baud, uart_regs *r)
{
	r->baud      = 2000000;
	r->pd        = 0;
	r->dl        = 3;
	r->mulval    = 4;
	r->divaddval = 1;
	return 0;
}
#else

/* definition to expand macro then apply to pragma message */
// #define _STR(x) #x
// #define STR(x) _STR(x)

// #pragma message STR(APPBAUD)
// #pragma message STR(__CORE_CLK)

static uint32_t const uabs(const uint32_t a, const uint32_t b)
{
	if (a>=b)
		return a-b;
	return b-a;
}

static uint32_t const calc_baud(uint32_t pclk, uint32_t dl, uint32_t divaddval, uint32_t mulval)
{
	// 65535 * 14 * 16 is less than 2**24 so we have a spare 8 bits of precision
	// we can use them to increase our accuracy quite a bit
	// pclk is less than 2**27, so we have 5 spare bits for the numerator
	// this means we can do (numerator * 2**5) / (denominator * 2**8) * 2**3 to get the highest accuracy possible with 32 bit integers
	// denominator is 16 * (dl * (1 + (divadd / mul)) which can be expanded to
	// dl*16 + dl*16*divadd/mul which gives far more opportunity for using all our precision
	uint32_t dx = ((dl * 16 * 32 * 8) + ((dl * 16 * divaddval * 32 * 8) / mulval));
	return ((pclk * 32) / dx) * 8;
}

static int baud_space_search(uint32_t target_baud, uart_regs *r)
{
	uint32_t pd, dl, mulval, divaddval;
	int i = 0;
	r->baud = 0;
	for (pd = ((target_baud < 1000000)?3:1); pd < 4; pd--)
	{
		uint32_t pclk = SystemCoreClock / (1<<pd);
		for (mulval = 1; mulval < 16; mulval++)
		{
			for (divaddval = 0; divaddval < mulval; divaddval++)
			{
				i++;
				// baud = pclk / (16 * dl * (1 + (DivAdd / Mul))
				// solving for dl, we get dl = mul * pclk / (16 * baud * (divadd + mul))
				// we double the numerator, add 1 to the result then halve to effectivel round up when dl % 1 > 0.5
				dl = (((2 * mulval * pclk) / (16 * target_baud * (divaddval + mulval))) + 1) / 2;

				// dl is a 16 bit field, if result needs more then we search again
				if (dl > 65535)
					continue;

				// datasheet says if DLL==DLM==0, then 1 is used instead since divide by zero is ungood
					if (dl == 0)
						dl = 1;

					// datasheet says if DIVADDVAL > 0 then DL must be >= 2
						if ((divaddval > 0) && (dl < 2))
							dl = 2;

						uint32_t b = calc_baud(pclk, dl, divaddval, mulval);
						if (uabs(b, target_baud) < uabs(r->baud, target_baud))
						{
							r->baud      = b;
							r->pd        = pd;
							r->dl        = dl;
							r->mulval    = mulval;
							r->divaddval = divaddval;
							// 					printf("\t\t{%7d,%4d,%6d,%3d,%3d},\t// Actual baud: %7d, error %c%4.2f%%, %d iterations\n", target_baud, 1<<best.pd, best.dl, best.mulval, best.divaddval, b, ((b > target_baud)?'+':((b < target_baud)?'-':' ')), (uabs(target_baud, b) * 100.0) / target_baud, i);
							if (b == target_baud)
								return i;
							// within 0.08%
								if ((uabs(r->baud, target_baud) * 1536 / target_baud) < 1)
									return i;
						}
			}
		}
		// don't check higher pclk if we're within 0.5%
		if ((uabs(r->baud, target_baud) * 200 / target_baud) < 1)
			return i;
	}
	return i;
}
#endif

int UART_baud(int baud)
{
    TxIntStat = RESET;

    RB_ZERO(txbuf);
    RB_ZERO(rxbuf);

	uart_regs r = { 0, 0, 0, 0, 0 };

	baud_space_search(baud, &r);

	uint8_t pclkdiv;
	IRQn_Type c = 255;

	switch (r.pd)
	{
		case 0:
			pclkdiv = CLKPWR_PCLKSEL_CCLK_DIV_1;
			break;
		case 1:
			pclkdiv = CLKPWR_PCLKSEL_CCLK_DIV_2;
			break;
		case 2:
			pclkdiv = CLKPWR_PCLKSEL_CCLK_DIV_4;
			break;
		default:
		case 3:
			pclkdiv = CLKPWR_PCLKSEL_CCLK_DIV_8;
			break;
	}

// PCLKSEL0
#define PCLK_UART0 6
#define PCLK_UART1 8
// PCLKSEL1
#define PCLK_UART2 16
#define PCLK_UART3 18

	switch(port)
	{
		case 0:
			LPC_SC->PCONP |= CLKPWR_PCONP_PCUART0;
			LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & ~(3 << PCLK_UART0)) | pclkdiv << PCLK_UART0;
			c = UART0_IRQn;
			break;
		case 1:
			LPC_SC->PCONP |= CLKPWR_PCONP_PCUART1;
			LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & ~(3 << PCLK_UART1)) | pclkdiv << PCLK_UART1;
			c = UART1_IRQn;
			break;
		case 2:
			LPC_SC->PCONP |= CLKPWR_PCONP_PCUART2;
			LPC_SC->PCLKSEL1 = (LPC_SC->PCLKSEL1 & ~(3 << PCLK_UART2)) | pclkdiv << PCLK_UART2;
			c = UART2_IRQn;
			break;
		case 3:
			LPC_SC->PCONP |= CLKPWR_PCONP_PCUART3;
			LPC_SC->PCLKSEL1 = (LPC_SC->PCLKSEL1 & ~(3 << PCLK_UART3)) | pclkdiv << PCLK_UART3;
			c = UART3_IRQn;
			break;
	}

	u->LCR |= UART_LCR_DLAB_EN;
	u->DLM = (r.dl >> 8) & 0xFF;
	u->DLL = (r.dl & 0xFF);
	u->LCR &= ~(UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
	u->FDR = (r.divaddval & 0xF) | ((r.mulval & 0xF) << 4);

	u->FCR = (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS);
	u->LCR = UART_LCR_WLEN8;
	u->ICR = 0;
	u->TER |= UART_TER_TXEN;

	if (c < 128)
		NVIC_EnableIRQ(c);

    return baud;
}

void UART_deinit(void) {
	switch(port)
	{
		case 0:
			NVIC_DisableIRQ(UART0_IRQn);
			break;
		case 1:
			NVIC_DisableIRQ(UART1_IRQn);
			break;
		case 2:
			NVIC_DisableIRQ(UART2_IRQn);
			break;
		case 3:
			NVIC_DisableIRQ(UART3_IRQn);
			break;
	}
}

uint32_t UART_send(const uint8_t *data, uint32_t buflen) {
    uint32_t bytes = 0;

    // only fiddle interrupt status outside interrupt context
    uint8_t intr = __get_IPSR() & 0x1F;

    if (intr == 0) __disable_irq();

    while ((buflen > 0) && (!RB_FULL(txbuf) || blocking))
    {
        if (intr == 0) __enable_irq();

        if (RB_FULL(txbuf)) {
            if (blocking && (intr == 0))
            {
                while (RB_FULL(txbuf))
                    __WFI();
            }
            else {
                RB_DROP(txbuf);
            }
        }

        if (intr == 0) __disable_irq();

        RB_PUSH(txbuf, *data++);

        if (intr == 0) __enable_irq();

        bytes++;
        buflen--;

        if (intr == 0) __disable_irq();
    }
    if (intr == 0) __enable_irq();

    if (TxIntStat == RESET) {
        UART_tx_isr();
    }

    return bytes;
}

uint32_t UART_recv(uint8_t *buf, uint32_t buflen) {
    uint32_t bytes = 0;

    // only fiddle interrupt status outside interrupt context
    uint8_t intr = __get_IPSR() & 0x1F;

    if (intr == 0) __disable_irq();

    while ((buflen > 0) && (!(RB_EMPTY(rxbuf))))
    {
        RB_POP(rxbuf, *buf++);

        if (intr == 0) __enable_irq();

        bytes++;
        buflen--;

        if (intr == 0) __disable_irq();
    }
    if (intr == 0) __enable_irq();

    return bytes;
}

int UART_cansend(void) {
    return RB_CANWRITE(txbuf);
}

int UART_canrecv(void) {
    return RB_CANREAD(rxbuf);
}

int UART_busy(void) {
	if (u->LSR & UART_LSR_TEMT)
		return 0;
	return RB_CANREAD(txbuf);
}

void UART_isr(void)
{
    uint32_t intsrc, ls;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId(u) & UART_IIR_INTID_MASK;

    // Receive Line Status
    if (intsrc == UART_IIR_INTID_RLS)
    {
        // Check line status
        ls = UART_GetLineStatus(u);
        // Mask out the Receive Ready and Transmit Holding empty status
        ls &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (ls)
        {
            UART_err_isr(ls & 0xFF);
        }
    }

    // Receive Data Available or Character time-out
    if ((intsrc == UART_IIR_INTID_RDA) || (intsrc == UART_IIR_INTID_CTI))
    {
        UART_rx_isr();
    }

    // Transmit Holding Empty
    if (intsrc == UART_IIR_INTID_THRE)
    {
        UART_tx_isr();
    }
}

void UART_tx_isr(void) {
    // Disable THRE interrupt
    UART_IntConfig(u, UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy(u) == SET);

    while (!RB_EMPTY(txbuf))
    {
		RB_POP(txbuf, u->THR);
    }

    /* If there is no more data to send, disable the transmit
     *       interrupt - else enable it or keep it enabled */
    if (RB_EMPTY(txbuf)) {
        UART_IntConfig(u, UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        TxIntStat = RESET;
    }
    else{
        // Set Tx Interrupt state
        TxIntStat = SET;
        UART_IntConfig(u, UART_INTCFG_THRE, ENABLE);
    }
}

void UART_rx_isr(void) {
    uint8_t c;
    uint32_t r;

    while(1){
        // If data received
        r = u->LSR;
        if (r & UART_LSR_RDR){
			c = u->RBR;
			/* Check if buffer is more space
             * If no more space, remaining character will be trimmed out
             */
            if (!RB_FULL(rxbuf)){
                RB_PUSH(rxbuf, c);
            }
        }
        // no more data
        else {
            break;
        }
    }
}

void UART_err_isr(uint8_t bLSErrType) {
//    uint8_t test;
    // Loop forever
    while (1){
        // For testing purpose
//        test = bLSErrType;
    }
}

// extern "C" {
void UART0_IRQHandler(void)
{
	ENTER_ISR();
// 	if (UART_irqrouter[0])
// 		UART_irqrouter[0]->isr();
	UART_isr();
	LEAVE_ISR();
}
/*
void UART1_IRQHandler(void)
{
	ENTER_ISR();
	if (UART_irqrouter[1])
		UART_irqrouter[1]->isr();
	LEAVE_ISR();
}

void UART2_IRQHandler(void)
{
	ENTER_ISR();
	if (UART_irqrouter[2])
		UART_irqrouter[2]->isr();
	LEAVE_ISR();
}

void UART3_IRQHandler(void)
{
	ENTER_ISR();
	if (UART_irqrouter[3])
		UART_irqrouter[3]->isr();
	LEAVE_ISR();
}
// }*/
