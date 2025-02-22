#include "GateDriverSpi.h"

#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

#include "DRV8323.h"

// SPI pins in use
//  MISO: PA6      SPI1_MISO
//  MOSI: PA7      SPI1_MOSI
//  CLK:  PA5      SPI1_SCK
//  CS:   PA4      SPI1_NSS

// Gate Driver SPI specs
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

static SPI_HandleTypeDef GateDriver_hspi;

void GateDriverSpi_Config()
{
    // Program the
    //      mode
    //      data size
    //      NSS management
    //      CPOL & CPHA
    //      FirstBit
    GateDriver_hspi.Instance                = SPI1;
    GateDriver_hspi.Init.Mode               = SPI_MODE_MASTER;
    GateDriver_hspi.Init.Direction          = SPI_DIRECTION_2LINES;
    GateDriver_hspi.Init.DataSize           = SPI_DATASIZE_8BIT;
    GateDriver_hspi.Init.CLKPolarity        = SPI_POLARITY_LOW;
    GateDriver_hspi.Init.CLKPhase           = SPI_PHASE_2EDGE;
    GateDriver_hspi.Init.NSS                = SPI_NSS_SOFT;
    GateDriver_hspi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_16;
    GateDriver_hspi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    GateDriver_hspi.Init.TIMode             = SPI_TIMODE_DISABLE;
    GateDriver_hspi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    GateDriver_hspi.Init.CRCPolynomial      = 0;

    // Initialize SPI registers: HAL_SPI_Init()
    HAL_SPI_Init( &GateDriver_hspi );
}


static void splitDataIntoBuffer( uint16_t data, uint8_t* buffer )
{
    buffer[0] = data >> 8;
    buffer[1] = data;
}

void GateDriverSpi_Write( uint16_t cmdOut )
{
    uint8_t dataOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( cmdOut, dataOut );
    GateDriver_hspi.TxXferSize = sizeof( dataOut );
    GateDriver_hspi.pTxBuffPtr = dataOut;
    //// Pull CS low
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_RESET );

    // Write
    HAL_SPI_Transmit( &GateDriver_hspi, dataOut, sizeof( dataOut ), 100 );

    // Pull CS back high
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
}

static uint32_t extractDataFromBuffer( uint8_t* buffer )
{
    return buffer[0] << 8 |
           buffer[1];
}

uint16_t GateDriverSpi_Read( uint16_t cmdOut )
{
    uint8_t dataOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( cmdOut, dataOut );

    uint8_t dataIn[ sizeof( uint16_t ) ];
    GateDriver_hspi.RxXferSize = sizeof( dataIn );
    GateDriver_hspi.pRxBuffPtr = dataIn;
    // Pull CS low
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_RESET );

    // Read
    HAL_SPI_Receive( &GateDriver_hspi, dataOut, sizeof( dataOut ), 100 );
    
    // Pull CS back high and return data
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
    return extractDataFromBuffer( dataIn );
}

void GateDriverSpi_SetCS( uint8_t state )
{
    if ( state != 0 )
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
    else
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_RESET );
}


