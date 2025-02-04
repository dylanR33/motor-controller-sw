#include "PhaseCurrentAdc.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_gpio.h"


static ADC_HandleTypeDef phaseA_hadc;
static ADC_HandleTypeDef phaseB_hadc;
static ADC_HandleTypeDef phaseC_hadc;


static void AdcConfig( ADC_HandleTypeDef* hadc, ADC_TypeDef* instance, uint32_t channel )
{
    hadc->Instance = instance;
    hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc->Init.Resolution = ADC_RESOLUTION_12B;
    hadc->Init.ScanConvMode = DISABLE;
    hadc->Init.ContinuousConvMode = ENABLE;
    hadc->Init.DiscontinuousConvMode = DISABLE;
    hadc->Init.NbrOfDiscConversion = 0;
    hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc->Init.NbrOfConversion = 1;
    hadc->Init.DMAContinuousRequests = DISABLE;
    hadc->Init.EOCSelection = DISABLE;

    HAL_ADC_Init( hadc );

    ADC_ChannelConfTypeDef chan = 
    {
        .Channel = channel,
        .Rank = 1, // 0 - 16
        .SamplingTime = ADC_SAMPLETIME_3CYCLES
    };

    HAL_ADC_ConfigChannel( hadc, &chan );

    // Start ADC
    HAL_ADC_Start( hadc );
}

void PhaseCurrentAdc_Config()
{
    AdcConfig( &phaseA_hadc, ADC1, ADC_CHANNEL_10 );
    AdcConfig( &phaseB_hadc, ADC2, ADC_CHANNEL_11 );
    AdcConfig( &phaseC_hadc, ADC3, ADC_CHANNEL_12 );
}

uint32_t PhaseCurrentAdc_GetRawPhaseA()
{
    HAL_ADC_PollForConversion( &phaseA_hadc, 10000 );
    return HAL_ADC_GetValue( &phaseA_hadc );
}

uint32_t PhaseCurrentAdc_GetRawPhaseB()
{
    HAL_ADC_PollForConversion( &phaseB_hadc, 10000 );
    return HAL_ADC_GetValue( &phaseB_hadc );
}

uint32_t PhaseCurrentAdc_GetRawPhaseC()
{

    HAL_ADC_PollForConversion( &phaseC_hadc, 10000 );
    return HAL_ADC_GetValue( &phaseC_hadc );
}
