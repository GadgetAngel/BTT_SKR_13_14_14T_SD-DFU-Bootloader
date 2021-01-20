#ifndef SDCARD_H
#define SDCARD_H

#include "spi.h"
// #include "gpio.hpp"

// #include "disk.h"

// #include "DMA.h"

/** Create the File System for accessing an SD Card using SPI
	*
	* @param mosi SPI mosi pin connected to SD Card
	* @param miso SPI miso pin conencted to SD Card
	* @param sclk SPI sclk pin connected to SD Card
	* @param cs   DigitalOut pin used as SD Card chip select
	* @param name The name used to access the virtual filesystem
	*/
void SDCard_init(PinName, PinName, PinName, PinName);
int SDCard_disk_initialize(void);
int SDCard_disk_write(const uint8_t *buffer, uint32_t block_number);
int SDCard_disk_read(uint8_t *buffer, uint32_t block_number);
int SDCard_disk_status(void);
int SDCard_disk_sync(void);
uint32_t SDCard_disk_sectors(void);
uint64_t SDCard_disk_size(void);
uint32_t SDCard_disk_blocksize(void);
int SDCard_disk_erase(uint32_t block_number, int count);
int SDCard_disk_canDMA(void);
void dma_source_event(void);
void dma_dest_event(void);

int busy(void);
#endif
