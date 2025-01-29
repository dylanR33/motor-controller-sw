#ifndef _GATE_DRIVER_SPI_H__
#define _GATE_DRIVER_SPI_H__

#include <stdint.h>

void GateDriverSpi_Config();

void GateDriverSpi_Write( uint16_t cmdOut );

uint16_t GateDriverSpi_Read( uint16_t cmdOut );

#endif

