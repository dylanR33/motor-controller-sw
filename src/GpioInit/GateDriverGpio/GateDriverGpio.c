#include "GateDriverGpio.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

void GateDriverGpio_Config()
{
    GPIO_InitTypeDef gateEnable =
    {
        .Pin = GPIO_PIN_11,
        .Mode = GPIO_MODE_OUTPUT_PP,
    };

    HAL_GPIO_Init( GPIOA, &gateEnable );

}

void GateDriverGpio_SetEnablePin( uint8_t state )
{
    if ( state )
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_11, GPIO_PIN_SET );
    else 
        HAL_GPIO_WritePin( GPIOA, GPIO_PIN_11, GPIO_PIN_RESET );
}

