#ifndef _GATE_DRIVER_SPI_H__
#define _GATE_DRIVER_SPI_H__

#include <stdint.h>

void GateDriverSpi_Config();

void GateDriverSpi_Write( uint8_t* dataOut, uint16_t size );

void GateDriverSpi_Read( uint8_t* dataOut, uint8_t* rxBuff, uint16_t size );

void GateDriverSpi_SetCS( uint8_t state );

#endif

