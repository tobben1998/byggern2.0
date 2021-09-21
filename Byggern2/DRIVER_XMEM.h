//Utilities for external memory

#ifndef DRIVER_XMEM_H_
#define DRIVER_XMEM_H_

//Init function
void xmem_init(void);

//Write function
void xmem_write(uint8_t data, uint16_t addr);

//Read function
uint8_t xmem_read(uint16_t addr);

void SRAM_test(void);

#endif