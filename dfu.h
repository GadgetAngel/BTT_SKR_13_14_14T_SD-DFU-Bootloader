#ifndef _DFU_H
#define _DFU_H

#include <stdint.h>

#define REQ_DFU_DETACH		0x0
#define REQ_DFU_DNLOAD		0x1
#define REQ_DFU_UPLOAD		0x2
#define REQ_DFU_GETSTATUS	0x3
#define REQ_DFU_CLRSTATUS	0x4
#define REQ_DFU_GETSTATE	0x5
#define REQ_DFU_ABORT		0x6

#define DL_DFU_FUNCTIONAL_DESCRIPTOR	0x09
#define DT_DFU_FUNCTIONAL_DESCRIPTOR	0x21

#define DFU_VERSION_1_1					0x0101

#define DFU_INTERFACE_CLASS				0xFE
#define DFU_INTERFACE_SUBCLASS			0x01
#define DFU_INTERFACE_PROTOCOL_RUNTIME	0x01
#define DFU_INTERFACE_PROTOCOL_DFUMODE	0x02

#define DFU_BMATTRIBUTES_WILLDETACH			(1<<3)
#define DFU_BMATTRIBUTES_MANIFEST_TOLERANT	(1<<2)	/* device keeps talking while flashing? */
#define DFU_BMATTRIBUTES_CANUPLOAD			(1<<1)
#define DFU_BMATTRIBUTES_CANDOWNLOAD		(1<<0)

#define DFU_DETACH		0
#define DFU_DNLOAD		1
#define DFU_UPLOAD		2
#define DFU_GETSTATUS	3
#define DFU_CLRSTATUS	4
#define DFU_GETSTATE	5
#define DFU_ABORT		6

#include "usbcore.h"

typedef struct
{
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint8_t		bmAttributes;
	uint16_t	wDetachTimeout;
	uint16_t	wTransferSize;
	uint16_t	bcdDFUVersion;
} DFU_functional_descriptor;

typedef struct
{
	uint8_t		bStatus;
	uint8_t		bwPollTimeout[3];
	uint8_t		bState;
	uint8_t		iString;
} DFU_Status_Response;

void DFU_init(void);

void DFU_controlTransfer(CONTROL_TRANSFER *);
void DFU_transferComplete(CONTROL_TRANSFER *);
int DFU_complete(void);

#endif /* _DFU_H */
