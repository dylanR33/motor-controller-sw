#include "PhaseCurrentAdc.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_adc_ex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

static ADC_HandleTypeDef phaseA_hadc;
static ADC_HandleTypeDef phaseB_hadc;
static ADC_HandleTypeDef phaseC_hadc;

void PhaseCurrentAdc_Config()
{
    // ------------- Phase A ADC (master) config ----------------------
    phaseA_hadc.Instance = ADC1;
    phaseA_hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    phaseA_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseA_hadc.Init.ScanConvMode = DISABLE;
    phaseA_hadc.Init.ContinuousConvMode = ENABLE;
    phaseA_hadc.Init.DiscontinuousConvMode = DISABLE;
    phaseA_hadc.Init.NbrOfDiscConversion = 0;
    phaseA_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    phaseA_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    phaseA_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseA_hadc.Init.NbrOfConversion = 1;
    phaseA_hadc.Init.DMAContinuousRequests = DISABLE;
    phaseA_hadc.Init.EOCSelection = DISABLE;

    HAL_ADC_Init( &phaseA_hadc );

    ADC_ChannelConfTypeDef phaseAchan = 
    {
        .Channel = ADC_CHANNEL_10,
        .Rank = 1, // 0 - 16
        .SamplingTime = ADC_SAMPLETIME_3CYCLES
    };

    HAL_ADC_ConfigChannel( &phaseA_hadc, &phaseAchan );

    // ------------- Phase B ADC config ----------------------
    phaseB_hadc.Instance = ADC2;
    phaseB_hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    phaseB_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseB_hadc.Init.ScanConvMode = DISABLE;
    phaseB_hadc.Init.ContinuousConvMode = ENABLE;
    phaseB_hadc.Init.DiscontinuousConvMode = DISABLE;
    phaseB_hadc.Init.NbrOfDiscConversion = 0;
    phaseB_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    phaseB_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    phaseB_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseB_hadc.Init.NbrOfConversion = 1;
    phaseB_hadc.Init.DMAContinuousRequests = DISABLE;
    phaseB_hadc.Init.EOCSelection = DISABLE;

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
    phaseC_hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    phaseC_hadc.Init.Resolution = ADC_RESOLUTION_12B;
    phaseC_hadc.Init.ScanConvMode = DISABLE;
    phaseC_hadc.Init.ContinuousConvMode = ENABLE;
    phaseC_hadc.Init.DiscontinuousConvMode = DISABLE;
    phaseC_hadc.Init.NbrOfDiscConversion = 0;
    phaseC_hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    phaseC_hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    phaseC_hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    phaseC_hadc.Init.NbrOfConversion = 1;
    phaseC_hadc.Init.DMAContinuousRequests = DISABLE;
    phaseC_hadc.Init.EOCSelection = DISABLE;

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

void PhaseCurrentAdc_PollForConversion()
{
    HAL_ADC_PollForConversion( &phaseA_hadc, 10000 );
    HAL_ADC_PollForConversion( &phaseB_hadc, 10000 );
    HAL_ADC_PollForConversion( &phaseC_hadc, 10000 );
}

uint32_t PhaseCurrentAdc_GetPhaseA()
{
    return HAL_ADC_GetValue( &phaseA_hadc );
}

uint32_t PhaseCurrentAdc_GetPhaseB()
{
    return HAL_ADC_GetValue( &phaseB_hadc );
}

uint32_t PhaseCurrentAdc_GetPhaseC()
{
    return HAL_ADC_GetValue( &phaseC_hadc );
}
