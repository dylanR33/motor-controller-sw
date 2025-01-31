#include "PhaseCurrentAdc.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

static ADC_HandleTypeDef phaseA_hadc;
static ADC_HandleTypeDef phaseB_hadc;
static ADC_HandleTypeDef phaseC_hadc;

void PhaseCurrentAdc_Config()
{
    // ------------- Phase A ADC (master) config ----------------------
    phaseA_hadc.Instance = ADC1;
    phaseA_hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    phaseA_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseA_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseA_hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    phaseA_hadc.Init.ContinuousConvMode = DISABLE;
    phaseA_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init( &phaseA_hadc );

    ADC_MultiModeTypeDef multi =
    {
        .Mode = ADC_TRIPLEMODE_REGSIMULT,
        .TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES
    };

    HAL_ADCEx_MultiModeConfigChannel( &phaseA_hadc, &multi );

    ADC_ChannelConfTypeDef phaseAchan = 
    {
        .Channel = ADC_CHANNEL_10,
        .Rank = 1, // 0 - 16
        .SamplingTime = ADC_SAMPLETIME_3CYCLES
    };

    HAL_ADC_ConfigChannel( &phaseA_hadc, &phaseAchan );

    // ------------- Phase B ADC config ----------------------
    phaseB_hadc.Instance = ADC2;
    phaseB_hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    phaseB_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseB_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseB_hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    phaseB_hadc.Init.ContinuousConvMode = DISABLE;
    phaseB_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init( &phaseB_hadc );

    ADC_ChannelConfTypeDef phaseBchan = 
    {
        .Channel = ADC_CHANNEL_11,
        .Rank = 1, // 0 - 16
        .SamplingTime = ADC_SAMPLETIME_3CYCLES
    };

    HAL_ADC_ConfigChannel( &phaseB_hadc, &phaseBchan );

    // ------------- Phase C ADC config ----------------------
    phaseC_hadc.Instance = ADC3;
    phaseC_hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    phaseC_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseC_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseC_hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    phaseC_hadc.Init.ContinuousConvMode = DISABLE;
    phaseC_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init( &phaseC_hadc );

    ADC_ChannelConfTypeDef phaseCchan = 
    {
        .Channel = ADC_CHANNEL_12,
        .Rank = 1, // 0 - 16
        .SamplingTime = ADC_SAMPLETIME_3CYCLES
    };

    HAL_ADC_ConfigChannel( &phaseC_hadc, &phaseCchan );

    // Start ADC's
    HAL_ADC_Start( &phaseA_hadc );
    HAL_ADC_Start( &phaseB_hadc );
    HAL_ADC_Start( &phaseC_hadc );
}


uint32_t PhaseCurrentAdc_GetPhaseA()
{
    return HAL_ADC_GetValue( &phaseA_hadc );
}

uint32_t PhaseCurrnetAdc_GetPhaseB()
{
    return HAL_ADC_GetValue( &phaseB_hadc );
}

uint32_t PhaseCurrentAdc_GetPhaseC()
{
    return HAL_ADC_GetValue( &phaseC_hadc );
}
