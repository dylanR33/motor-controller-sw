#include "RotaryEncoderSpi.h"

#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

#include "AS5047P.h"

// SPI pins in use
//  MISO: PC11      SPI3_MISO
//  MOSI: PC12      SPI3_MOSI
//  CLK:  PC10      SPI3_SCK
//  CS:   PA15      SPI3_NSS

// Encoder SPI specs
//      Clock rates up to 10Mhz
//      SPI Mode 1 ( CPOL = 0, CPHA = 1 )
//      Data transfer starts w falling edge of CS
//      Sampling of MOSI on falling edge of CLK
//      MSB first
//      Serial Clock period min of 100 ns

// STM SPI specs
//      Programmable clock polarity and phase
//      8 bit or 16 bit transfer frame selection
//      Programmable data order (MSB first or LSB first)
//      CS is called NSS (slave select)
//
//      options wanted
//          Full duplex (regular spi pin out miso,mosi,clk,cs)

// Configuration steps
//      1. config gpio for miso mosi etc.
//      2. serial clock baud rate 
//         CPOL and CPHA
//         duplex mode
//         frame format (LSB or MSB first)
//         set DFF(data from format 8 or 16 bit)

static SPI_HandleTypeDef RotaryEncoder_hspi;

void RotaryEncoderSpi_Config()
{
    // Program the
    //      mode
    //      data size
    //      NSS management
    //      CPOL & CPHA
    //      FirstBit
    RotaryEncoder_hspi.Instance = SPI3;
    RotaryEncoder_hspi.Init.Mode = SPI_MODE_MASTER;
    RotaryEncoder_hspi.Init.Direction = SPI_DIRECTION_2LINES;
    RotaryEncoder_hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    RotaryEncoder_hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    RotaryEncoder_hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
    RotaryEncoder_hspi.Init.NSS = SPI_NSS_SOFT;
    RotaryEncoder_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    RotaryEncoder_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    RotaryEncoder_hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    RotaryEncoder_hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    RotaryEncoder_hspi.Init.CRCPolynomial = 0;

    // Initialize SPI registers: HAL_SPI_Init()
    HAL_SPI_Init( &RotaryEncoder_hspi );
}


void RotaryEncoderSpi_Write( uint8_t* data, uint16_t size )
{
    RotaryEncoder_hspi.TxXferSize = size;
    RotaryEncoder_hspi.pTxBuffPtr = data;
    // Write
    HAL_SPI_Transmit( &RotaryEncoder_hspi, data, size, 100 );
}


void RotaryEncoderSpi_Read( uint8_t* dataOut, uint8_t* rxBuff, uint16_t size )
{
    RotaryEncoder_hspi.RxXferSize = size;
    RotaryEncoder_hspi.pRxBuffPtr = rxBuff;
    // Read
    HAL_SPI_TransmitReceive( &RotaryEncoder_hspi, dataOut, rxBuff, size, 100 );
}

void RotaryEncoderSpi_SetCS( uint8_t state )
{
    if ( state )
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET );
    else
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_RESET );
}

