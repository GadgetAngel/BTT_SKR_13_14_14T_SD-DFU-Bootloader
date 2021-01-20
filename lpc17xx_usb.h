#ifndef _LPC17XX_USB_H
#define _LPC17XX_USB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define EP2IDX(bEP)			((((bEP)&0xF)<<1)|(((bEP)&0x80)>>7))
#define IDX2EP(idx)			((((idx)<<7)&0x80)|(((idx)>>1)&0xF))

#define EP(x)				(1UL<<EP2IDX(x))

// USBClkCtrl
#define DEV_CLK_EN			(1UL<<1)
#define AHB_CLK_EN			(1UL<<4)

// USBClkSt
#define DEV_CLK_ON			(1UL<<1)
#define AHB_CLK_ON			(1UL<<4)

// USBIntSt
#define USB_INT_REQ_LP		(1UL<<0)
#define USB_INT_REQ_HP		(1UL<<1)
#define USB_INT_REQ_DMA		(1UL<<2)

#define USB_NEED_CLK		(1UL<<8)

#define EN_USB_INTS			(1UL<<31)

// USBDevIntSt / USBDevIntClr / USBDevIntSet / USBDevIntPri
#define FRAME				(1UL<<0)
#define EP_FAST				(1UL<<1)
#define EP_SLOW				(1UL<<2)
#define DEV_STAT			(1UL<<3)
#define CCEMPTY				(1UL<<4)
#define CDFULL				(1UL<<5)
#define RxENDPKT			(1UL<<6)
#define TxENDPKT			(1UL<<7)
#define EP_RLZD				(1UL<<8)
#define ERR_INT				(1UL<<9)

// USBRxPlen / USBTxPlen
#define PKT_LNGTH_MASK		(511UL<<0)
#define DV					(1UL<<10)
#define PKT_RDY				(1UL<<11)

// USBCtrl
#define RD_EN				(1UL<<0)
#define WR_EN				(1UL<<1)
#define LOG_ENDPOINT_MASK	(15UL<<2)

// USBDMAIntSt / USBDMAIntEn
#define EOT					(1UL<<0)
#define NDDR				(1UL<<1)
#define ERR					(1UL<<2)

// USBCmdCode
#define CMD_PHASE_WRITE		(1UL<<8)
#define CMD_PHASE_READ		(2UL<<8)
#define CMD_PHASE_COMMAND	(5UL<<8)

#define CMD_CODE(a)			(((a) & 255UL)<<16)
#define CMD_WDATA(a)		(((a) & 255UL)<<16)

// SIE commands
// 		device commands
#define SIE_DEV_CMD_SetAddress		0xD0
#define SIE_DEV_CMD_Configure		0xD8
#define SIE_DEV_CMD_SetMode			0xF3
#define SIE_DEV_CMD_ReadFrameNo		0xF5
#define SIE_DEV_CMD_ReadTestReg		0xFD
#define SIE_DEV_CMD_SetDevStatus	0xFE
#define SIE_DEV_CMD_GetDevStatus	0xFE
#define SIE_DEV_CMD_GetErrorCode	0xFF
#define SIE_DEV_CMD_ReadErrorSt		0xFB

// 		endpoint commands
#define SIE_EP_CMD_Select			0x0
#define SIE_EP_CMD_SelectClearInt	0x40
#define SIE_EP_CMD_SetEpStatus		0x40
#define SIE_EP_CMD_ClearBuffer		0xF2
#define SIE_EP_CMD_ValidateBuffer	0xFA

// SIE Set Address Bits
/// DEV_ADDR - The device address
#define SIE_SETADDR_DEV_ADDR_MASK	(0x7F)
/// DEV_EN - Device will respond to requests destined for DEV_ADDR
#define SIE_SETADDR_DEV_EN			(1UL<<7)

// SIE Modes (use with SIE_SetMode)
/// Always Clock: When asserted, the USB clock is kept running if the core enters suspend powerdown
#define SIE_MODE_AP_CLK				(1UL<<0)
/// INAK_CI - Interrupt on NAK for Control In
#define SIE_MODE_INAK_CI			(1UL<<1)
/// INAK_CO - Interrupt on NAK for Control Out
#define SIE_MODE_INAK_CO			(1UL<<2)
/// INAK_II - Interrupt on NAK for Isochronous In
#define SIE_MODE_INAK_II			(1UL<<3)
/// INAK_IO - Interrupt on NAK for Isochronous Out
#define SIE_MODE_INAK_IO			(1UL<<4)
/// INAK_BI - Interrupt on NAK for Bulk In
#define SIE_MODE_INAK_BI			(1UL<<5)
/// INAK_BO - Interrupt on NAK for Bulk Out
#define SIE_MODE_INAK_BO			(1UL<<6)

// SIE Device Statuses
/// CON: set this bit to enable the pull-up that indicates to the host that a USB Full Speed device is present. Cleared by hardware if VBus dips for 3ms and we're monitoring VBus
#define SIE_DEVSTAT_CON				(1UL<<0)
/// CON_CH: Connection state changed, triggers DEV_STAT interrupt
#define SIE_DEVSTAT_CON_CH			(1UL<<1)
/// SUS: Suspended. Write 0 to issue a remote wakeup IF we're connected AND suspended. Writing otherwise has no effect.
#define SIE_DEVSTAT_SUS				(1UL<<2)
/// SUS_CH: Suspend state changed. Triggers DEV_STAT interrupt
#define SIE_DEVSTAT_SUS_CH			(1UL<<3)
/// RST: received a Bus Reset. Triggers DEV_STAT interrupt.
#define SIE_DEVSTAT_RST				(1UL<<4)

// SIE Error Codes - returned from SIE_GetErrorCode
/// No Error
#define SIE_ERRCODE_NOERROR			0x0
/// PID Encoding Error
#define SIE_ERRCODE_PIDENCODE		0x1
/// Unknown PID
#define SIE_ERRCODE_UNKNOWNPID		0x2
/// Unexpected Packet - any packet sequence violation from the specification
#define SIE_ERRCODE_UNEXPECTED		0x3
/// Error in Token CRC
#define SIE_ERRCODE_TOKENCRC		0x4
/// Error in Data CRC
#define SIE_ERRCODE_DATACRC			0x5
/// Timeout Error
#define SIE_ERRCODE_TIMEOUT			0x6
/// Babble
#define SIE_ERRCODE_BABBLE			0x7
/// Error in End of Packet
#define SIE_ERRCODE_EOP				0x8
/// Sent/Received NAK
#define SIE_ERRCODE_NAK				0x9
/// Sent Stall
#define SIE_ERRCODE_STALL			0xA
/// Buffer Overrun Error
#define SIE_ERRCODE_OVERRUN			0xB
/// Sent Empty Packet (Isochronous Endpoints only)
#define SIE_ERRCODE_ISO_EMPTYPACKET	0xC
/// Bitstuff Error
#define SIE_ERRCODE_BITSTUFF		0xD
/// Error in Sync
#define SIE_ERRCODE_SYNC			0xE
/// Wrong toggle bit in Data PID, packet ignored
#define SIE_ERRCODE_TOGGLEBIT		0xF

/// The Error Active bit will be reset when this register is read
#define SIE_EA						(1UL<<4)

// SIE Error Statuses - returned from SIE_ReadErrorStatus
/// PID Encoding Error or Unknown PID or Token CRC
#define SIE_ERRST_PID_ERR			(1UL<<0)
/// Unexpected Packet Error
#define SIE_ERRST_UEPKT				(1UL<<1)
/// Data CRC Error
#define SIE_ERRST_DCRC				(1UL<<2)
/// Timeout Error
#define SIE_ERRST_TIMEOUT			(1UL<<3)
/// End of Packet Error
#define SIE_ERRST_EOP				(1UL<<4)
/// Buffer Overrun Error
#define SIE_ERRST_B_OVRN			(1UL<<5)
/// Bitstuff Error
#define SIE_ERRST_BTSTF				(1UL<<6)
/// Wrong toggle bit in data PID, packet ignored
#define SIE_ERRST_TGL_ERR			(1UL<<7)

// SIE Configure Device
/// CONF_DEVICE: device is configured
#define SIE_CONF_DEVICE				(1UL<<0)

// SIE Endpoint Statuses - returned from SIE_SelectEndpoint and SIE_SelectEndpointClearInterrupt
/// Full/Empty bit. For IN endpoints, equal to B_1_FULL & B_2_FULL. For OUT endpoints, equal to B_1_FULL | B_2_FULL. For single buffered endpoints, same as B_1_FULL
#define SIE_EP_FE					(1UL<<0)
/// Stalled Indicator. 0 = not stalled, 1 = stalled
#define SIE_EP_ST					(1UL<<1)
/// SETUP bit: the last received packet was a SETUP packet
#define SIE_EP_STP					(1UL<<2)
/// Packet over-written bit: A regular packet was overwritten by a SETUP packet
#define SIE_EP_PO					(1UL<<3)
/// EP Nacked Bit: Asserted when a NAK was sent and interrupt on NAK mode is enabled
#define SIE_EP_EPN					(1UL<<4)
/// Buffer 1 status: 0 = empty, 1 = full
#define SIE_EP_B_1_FULL				(1UL<<5)
/// Buffer 2 status: 0 = empty, 1 = full
#define SIE_EP_B_2_FULL				(1UL<<6)

// SIE Endpoint Statuses - set with SIE_SetEndpointStatus
/// Stall endpoint
#define SIE_EPST_ST					(1UL<<0)
/// Disable endpoint
#define SIE_EPST_DA					(1UL<<5)
/// Rate Feedback mode
#define SIE_EPST_RF_MO				(1UL<<6)
/// Conditional Stall - Stall both control endpoints unless STP is asserted in SIE_SelectEndpoint register. Only for OUT endpoints
#define SIE_EPST_CND_ST				(1UL<<7)

// SIE Clear Buffer result
/// Packet Overwritten bit: Asserted when a SETUP packet overwrites a received packet
#define SIE_CLRBUF_PO				(1UL<<0)

// device commands
void		SIE_SetAddress(uint8_t);
void		SIE_ConfigureDevice(uint8_t);
void		SIE_SetMode(uint8_t);
uint16_t	SIE_ReadCurrentFrameNumber(void);
uint16_t	SIE_ReadTestRegister(void);
void		SIE_SetDeviceStatus(uint8_t);
uint8_t		SIE_GetDeviceStatus(void);
uint8_t		SIE_GetErrorCode(void);
uint8_t		SIE_ReadErrorStatus(void);

// endpoint commands
uint8_t		SIE_SelectEndpoint(uint8_t bEP);
uint8_t		SIE_SelectEndpointClearInterrupt(uint8_t bEP);
void		SIE_SetEndpointStatus(uint8_t bEP, uint8_t status);
uint8_t		SIE_ClearBuffer(void);
void		SIE_ValidateBuffer(void);

// utility commands - derived from the above
void		SIE_Connect(void);
void		SIE_Disconnect(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LPC17XX_USB_H */
