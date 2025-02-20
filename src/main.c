#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "usart_print.h"

// Hardware Init
#include "RotaryEncoderSpi.h"
#include "GateDriverSpi.h"
#include "GateDriverGpio.h"
#include "SvmPwm.h"
#include "PhaseCurrentAdc.h"
#include "ClockInit.h"

// Drivers
#include "AS5047P.h"
#include "DRV8323.h"

// FOC
#include "Transform.h"
#include "SVM.h"


uint8_t scalePulseWidth( float width );
uint8_t FloatToPWMValue(float value);
void main()
{
    HAL_Init();
    ClockInit_SystemClockConfig();
    UsartPrint_Init();

    RotaryEncoderSpi_Config();
    GateDriverSpi_Config();
    GateDriverGpio_Config();
    SvmPwm_Config();
    PhaseCurrentAdc_Config();

    AS5047PInterface encoder =
    {
        .spiWrite = RotaryEncoderSpi_Write,
        .spiRead  = RotaryEncoderSpi_Read
    };
    AS5047P_SetInterface( &encoder );

    DRV8323Interface gateDriver =
    {
        .spiWrite         = GateDriverSpi_Write,
        .spiRead          = GateDriverSpi_Read,
        .adcReadRawPhaseA = PhaseCurrentAdc_GetRawPhaseA,
        .adcReadRawPhaseB = PhaseCurrentAdc_GetRawPhaseB,
        .adcReadRawPhaseC = PhaseCurrentAdc_GetRawPhaseC,
        .setEnablePin     = GateDriverGpio_SetEnablePin
    };
    DRV8323_SetInterface( &gateDriver );

    DRV8323_SetEnableState( ENABLE_ON );

    DRV8323CurrentSenseCfg iCfg =
    {
        .vRef = 3.3,
        .adcSteps = 4096,
        .rSense = 0.0015,
        .csaGain = 10
    };

    while (1)
    {
        HAL_Delay( 500 );
    }
}

