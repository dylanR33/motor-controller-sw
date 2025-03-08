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
    GateDriver_hspi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_256;
    GateDriver_hspi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    GateDriver_hspi.Init.TIMode             = SPI_TIMODE_DISABLE;
    GateDriver_hspi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    GateDriver_hspi.Init.CRCPolynomial      = 0;

    // Initialize SPI registers: HAL_SPI_Init()
    HAL_SPI_Init( &GateDriver_hspi );
}


void GateDriverSpi_Write( uint8_t* dataOut, uint16_t size )
{
    GateDriver_hspi.TxXferSize = size;
    GateDriver_hspi.pTxBuffPtr = dataOut;
    // Write
    HAL_SPI_Transmit( &GateDriver_hspi, dataOut, size, 100 );
}


void GateDriverSpi_Read( uint8_t* dataOut, uint8_t* rxBuff, uint16_t size )
{
    GateDriver_hspi.RxXferSize = size;
    GateDriver_hspi.pRxBuffPtr = rxBuff;
    // Read
    HAL_SPI_TransmitReceive( &GateDriver_hspi, dataOut, rxBuff, size, 100 );
}

void GateDriverSpi_SetCS( uint8_t state )
{
    if ( state )
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
    else
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_RESET );
}


