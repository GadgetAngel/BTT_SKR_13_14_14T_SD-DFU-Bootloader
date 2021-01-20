#ifndef _USBCORE_H
#define _USBCORE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

// standard requests
typedef enum
{
	REQ_GET_STATUS				= 0,
	REQ_CLEAR_FEATURE			= 1,
	REQ_SET_FEATURE				= 3,
	REQ_SET_ADDRESS				= 5,
	REQ_GET_DESCRIPTOR			= 6,
	REQ_SET_DESCRIPTOR			= 7,
	REQ_GET_CONFIGURATION		= 8,
	REQ_SET_CONFIGURATION		= 9,
} USB_REQUEST;

#define DATA_DIRECTION_HOST_TO_DEVICE 0
#define DATA_DIRECTION_DEVICE_TO_HOST 1

typedef struct
__attribute__ ((packed))
{
	union {
		struct {
			uint8_t		bmRequestType_Recipient:5;
			uint8_t		bmRequestType_Type:2;
			uint8_t		bmRequestType_Data_Transfer_Direction:1;
		};
		uint8_t bmRequestType;
	};

	uint8_t		bRequest;
	uint16_t	wValue;
	uint16_t	wIndex;
	uint16_t	wLength;
} SETUP_PACKET;

typedef struct
{
	void *buffer;
	int bufferlen;

	uint8_t zlp;
	uint8_t complete;

	SETUP_PACKET setup;
} CONTROL_TRANSFER;



void usb_provideDescriptors(void *d);

void EP0setup(void);
void EP0in(void);
void EP0out(void);

#ifdef __cplusplus
}
#endif

#endif /* _USBCORE_H */
