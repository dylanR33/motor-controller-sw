#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

void HAL_SPI_MspInit( SPI_HandleTypeDef* hspi )
{
    // -----------Rotary Encoder SPI Pins-------------

    // Enable SPIx interface clock
    //  SPI3 Clock Enable is in RCC_APB1ENR bit 15
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;

    // Pin config
    //      Enable clocks for gpios
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //      config as alt fnc push-pull
    GPIO_InitTypeDef rotaryMiso, rotaryMosi, rotarySck, rotaryNss;
    
    // Port C pins
    rotaryMiso.Pin       = GPIO_PIN_6;
    rotaryMiso.Alternate = GPIO_AF5_SPI1;
    rotaryMiso.Mode      = GPIO_MODE_AF_PP;
    rotaryMiso.Speed     = GPIO_SPEED_FREQ_MEDIUM;

    rotaryMosi.Pin       = GPIO_PIN_7;
    rotaryMosi.Alternate = GPIO_AF5_SPI1;
    rotaryMosi.Mode      = GPIO_MODE_AF_PP;
    rotaryMosi.Speed     = GPIO_SPEED_FREQ_MEDIUM;

    rotarySck.Pin        = GPIO_PIN_5;
    rotarySck.Alternate  = GPIO_AF5_SPI1;
    rotarySck.Mode       = GPIO_MODE_AF_PP;
    rotarySck.Speed      = GPIO_SPEED_FREQ_MEDIUM;

    rotaryNss.Pin        = GPIO_PIN_4;
    //Nss.Alternate  = GPIO_AF5_SPI1;
    rotaryNss.Mode       = GPIO_MODE_OUTPUT_PP;
    //Nss.Speed      = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init( GPIOC, &rotaryMiso );
    HAL_GPIO_Init( GPIOC, &rotaryMosi );
    HAL_GPIO_Init( GPIOC, &rotarySck  );
    HAL_GPIO_Init( GPIOA, &rotaryNss );

    // Set CS's high
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET );


    // ------------Gate Driver SPI Pins--------------

    // Enable SPIx interface clock
    //  SPI1 Clock Enable is in RCC_APB2ENR bit 12
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Pin config
    //      Enable clocks for gpios (already done above GPIOA)

    //      config as alt fnc push-pull
    GPIO_InitTypeDef gateMiso, gateMosi, gateSck, gateNss;
    
    // Port C pins
    gateMiso.Pin       = GPIO_PIN_6;
    gateMiso.Alternate = GPIO_AF5_SPI1;
    gateMiso.Mode      = GPIO_MODE_AF_PP;
    gateMiso.Speed     = GPIO_SPEED_FREQ_MEDIUM;

    gateMosi.Pin       = GPIO_PIN_7;
    gateMosi.Alternate = GPIO_AF5_SPI1;
    gateMosi.Mode      = GPIO_MODE_AF_PP;
    gateMosi.Speed     = GPIO_SPEED_FREQ_MEDIUM;

    gateSck.Pin        = GPIO_PIN_5;
    gateSck.Alternate  = GPIO_AF5_SPI1;
    gateSck.Mode       = GPIO_MODE_AF_PP;
    gateSck.Speed      = GPIO_SPEED_FREQ_MEDIUM;

    // Port A pins
    gateNss.Pin        = GPIO_PIN_4;
    //Nss.Alternate  = GPIO_AF5_SPI1;
    gateNss.Mode       = GPIO_MODE_OUTPUT_PP;
    //Nss.Speed      = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init( GPIOA, &gateMiso );
    HAL_GPIO_Init( GPIOA, &gateMosi );
    HAL_GPIO_Init( GPIOA, &gateSck  );
    HAL_GPIO_Init( GPIOA, &gateNss );

    // Set CS's high
    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_SET );
}

