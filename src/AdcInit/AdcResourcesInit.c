#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

// ADC Pins
//  currentPhaseA: PC0     ADC123_IN10 ADC1
//  currentPhaseB: PC1     ADC123_IN11 ADC2
//  currectPhaseC: PC2     ADC123_IN12 ADC3

void HAL_ADC_MspInit( ADC_HandleTypeDef* hadc )
{
    if ( hadc->Instance == ADC1 )
    {
        // Enable adc clock __HAL_RCC_ADC_CLK_ENABLE()
        __HAL_RCC_ADC1_CLK_ENABLE();

        // Enable gpio port clocks
        __HAL_RCC_GPIOC_CLK_ENABLE();

        // Configure pins to analog mode
        GPIO_InitTypeDef phaseA = { 0 };

        phaseA.Pin  = GPIO_PIN_0;
        phaseA.Mode = GPIO_MODE_ANALOG;
        phaseA.Pull = GPIO_NOPULL;
     
        HAL_GPIO_Init( GPIOC, &phaseA );
    }
    else if ( hadc->Instance == ADC2 )
    {
        // Enable adc clock __HAL_RCC_ADC_CLK_ENABLE()
        __HAL_RCC_ADC2_CLK_ENABLE();

        // Enable gpio port clocks
        __HAL_RCC_GPIOC_CLK_ENABLE();

        // Configure pins to analog mode
        GPIO_InitTypeDef phaseB = { 0 };

        phaseB.Pin  = GPIO_PIN_1;
        phaseB.Mode = GPIO_MODE_ANALOG;
        phaseB.Pull = GPIO_NOPULL;
     
        HAL_GPIO_Init( GPIOC, &phaseB );
    }
    else if ( hadc->Instance == ADC3 )
    {
        // Enable adc clock __HAL_RCC_ADC_CLK_ENABLE()
        __HAL_RCC_ADC3_CLK_ENABLE();

        // Pin config
        // Enable gpio port clocks
        __HAL_RCC_GPIOC_CLK_ENABLE();

        // Configure pins to analog mode
        GPIO_InitTypeDef phaseC = { 0 };

        phaseC.Pin  = GPIO_PIN_2;
        phaseC.Mode = GPIO_MODE_ANALOG;
        phaseC.Pull = GPIO_NOPULL;
     
        HAL_GPIO_Init( GPIOC, &phaseC );
    }
}
