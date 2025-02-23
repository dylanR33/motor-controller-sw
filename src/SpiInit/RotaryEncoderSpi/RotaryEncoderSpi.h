#ifndef __ROTARY_ENCODER_SPI_H__
#define __ROTARY_ENCODER_SPI_H__

#include <stdint.h>

void RotaryEncoderSpi_Config();

void RotaryEncoderSpi_Write( uint8_t* data, uint16_t size );

void RotaryEncoderSpi_Read( uint8_t* dataOut, uint8_t* rxBuff, uint16_t size );

void RotaryEncoderSpi_SetCS( uint8_t state );

#endif

