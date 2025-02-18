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
    RotaryEncoder_hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    RotaryEncoder_hspi.Init.NSS = SPI_NSS_SOFT;
    RotaryEncoder_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    RotaryEncoder_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    RotaryEncoder_hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    RotaryEncoder_hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    RotaryEncoder_hspi.Init.CRCPolynomial = 0;

    // Initialize SPI registers: HAL_SPI_Init()
    HAL_SPI_Init( &RotaryEncoder_hspi );
}


static void splitDataIntoBuffer( uint32_t data, uint8_t* buffer )
{
    buffer[0] = data >> 24;
    buffer[1] = data >> 16;
    buffer[2] = data >> 8;
    buffer[3] = data;
}

void RotaryEncoderSpi_Write( uint32_t cmdOut )
{
    uint8_t dataOut[ sizeof( uint32_t ) ];
    splitDataIntoBuffer( cmdOut, dataOut );
    RotaryEncoder_hspi.TxXferSize = sizeof( dataOut );
    RotaryEncoder_hspi.pTxBuffPtr = dataOut;
    //// Pull CS low
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_RESET );

    // Write
    HAL_SPI_Transmit( &RotaryEncoder_hspi, dataOut, sizeof( dataOut ), 100 );

    // Pull CS back high
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET );
}

static uint32_t extractDataFromBuffer( uint8_t* buffer )
{
    return buffer[0] << 24 |
           buffer[1] << 16 |
           buffer[2] << 8  |
           buffer[3];
}

uint32_t RotaryEncoderSpi_Read( uint32_t cmdOut )
{
    uint8_t dataOut[ sizeof( uint32_t ) ];
    splitDataIntoBuffer( cmdOut, dataOut );

    uint8_t dataIn[ sizeof( uint32_t ) ];
    RotaryEncoder_hspi.RxXferSize = sizeof( dataIn );
    RotaryEncoder_hspi.pRxBuffPtr = dataIn;
    // Pull CS low
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_RESET );

    // Read
    HAL_SPI_Receive( &RotaryEncoder_hspi, dataOut, sizeof( dataOut ), 100 );
    
    // Pull CS back high and return data
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET );
    return extractDataFromBuffer( dataIn );
}
