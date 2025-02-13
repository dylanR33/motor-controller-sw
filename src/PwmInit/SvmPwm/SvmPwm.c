#include "SvmPwm.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_tim_ex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

// PA8  Phase A PWM
// PA9  Phase B PWM
// PA10 Phase C PWM

static TIM_HandleTypeDef timer = { 0 };

static TIM_OC_InitTypeDef OCcfg = { 0 };

void SvmPwm_Config()
{
    TIM_ClockConfigTypeDef clkCfg = { 0 };
    TIM_MasterConfigTypeDef masterCfg = { 0 };
    TIM_BreakDeadTimeConfigTypeDef breakDeadTimeCfg = { 0 };

    timer.Instance = TIM1;
    timer.Init.Prescaler = 0;
    timer.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
    timer.Init.Period = 0x0FCA;

    timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    timer.Init.RepetitionCounter = 1;
    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init( &timer );
    
    clkCfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource( &timer, &clkCfg );

    HAL_TIM_PWM_Init( &timer );

    masterCfg.MasterOutputTrigger = TIM_TRGO_RESET;
    masterCfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization( &timer, &masterCfg );

    OCcfg.OCMode = TIM_OCMODE_PWM1;
    OCcfg.Pulse = 0x07FF; // 50% +Duty
    OCcfg.OCPolarity = TIM_OCPOLARITY_LOW;
    OCcfg.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    OCcfg.OCFastMode = TIM_OCFAST_ENABLE;
    OCcfg.OCIdleState = TIM_OCIDLESTATE_RESET;
    OCcfg.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    
    HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_1 );
    HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_2 );
    HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_3 );

    breakDeadTimeCfg.OffStateRunMode = TIM_OSSR_DISABLE;
    breakDeadTimeCfg.OffStateIDLEMode = TIM_OSSI_DISABLE;
    breakDeadTimeCfg.LockLevel = TIM_LOCKLEVEL_OFF;
    breakDeadTimeCfg.DeadTime = 0;
    breakDeadTimeCfg.BreakState = TIM_BREAK_DISABLE;
    breakDeadTimeCfg.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    breakDeadTimeCfg.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime( &timer, &breakDeadTimeCfg );


    GPIO_InitTypeDef pwmGpios = { 0 };
    
    __HAL_RCC_GPIOA_CLK_ENABLE();


    pwmGpios.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    pwmGpios.Mode = GPIO_MODE_AF_PP;
    pwmGpios.Pull = GPIO_NOPULL;
    pwmGpios.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    pwmGpios.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init( GPIOA, &pwmGpios );
    
    // Start PWM's
    HAL_TIM_PWM_Start( &timer, TIM_CHANNEL_1 );
    HAL_TIM_PWM_Start( &timer, TIM_CHANNEL_2 );
    HAL_TIM_PWM_Start( &timer, TIM_CHANNEL_3 );
}

void SvmPwm_SetPulseU( float pulse )
{
    //OCcfg.Pulse = pulse;
    //HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_1 );
    __HAL_TIM_SET_COMPARE( &timer, TIM_CHANNEL_1, timer.Instance->ARR * pulse );
}

void SvmPwm_SetPulseV( float pulse )
{
    //OCcfg.Pulse = pulse;
    //HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_2 );
    __HAL_TIM_SET_COMPARE( &timer, TIM_CHANNEL_2, timer.Instance->ARR * pulse );
}

void SvmPwm_SetPulseW( float pulse )
{
    //OCcfg.Pulse = pulse;
    //HAL_TIM_PWM_ConfigChannel( &timer, &OCcfg, TIM_CHANNEL_3 );
    __HAL_TIM_SET_COMPARE( &timer, TIM_CHANNEL_3, timer.Instance->ARR * pulse );
}
