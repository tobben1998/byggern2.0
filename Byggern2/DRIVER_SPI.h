#ifndef DRIVER_SPI_H
#define DRIVER_SPI_H

#include <stdint.h>

void spi_init(void);
void spi_send(uint8_t cData);
uint8_t spi_read();















#endif
