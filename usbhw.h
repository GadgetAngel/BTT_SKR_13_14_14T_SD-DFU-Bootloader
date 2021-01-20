#ifndef _USBHW_H
#define _USBHW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define EP0OUT		(0x00)
#define EP0IN		(0x80)

typedef void (*usb_callback_pointer)(void);

extern usb_callback_pointer EPcallbacks[30];



extern void EP0setup(void);

extern void EP0in(void);
extern void EP0out(void);

extern void USBEvent_busReset(void);
extern void USBEvent_connect(uint8_t connected);
extern void USBEvent_suspend(uint8_t suspended);
extern void USBEvent_Frame(void);


void usb_init(void);

void usb_task(void);

void usb_set_callback(uint8_t bEP, usb_callback_pointer callback);

void usb_realise_endpoint(uint8_t bEP, uint16_t packet_size);

int usb_can_write(uint8_t bEP);
int usb_write_packet(uint8_t bEP, void *data, int packetlen);

int usb_can_read(uint8_t bEP);
int usb_read_packet(uint8_t bEP, void *buffer, int buffersize);

void usb_connect(void);
void usb_disconnect(void);

void usb_ep_stall(uint8_t bEP);
void usb_ep_unstall(uint8_t bEP);
void usb_ep0_stall(void);

#ifdef __cplusplus
}
#endif


#endif /* _USBHW_H */
