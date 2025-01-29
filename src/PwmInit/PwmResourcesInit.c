#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
    if ( htim->Instance == TIM1 )
    {
        __HAL_RCC_TIM1_CLK_ENABLE();

        HAL_NVIC_SetPriority( TIM1_UP_TIM10_IRQn, 0, 0 );
        HAL_NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn );
    }
}
