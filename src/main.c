#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_flash_ex.h"

#include "usart_print.h"

#include "RotaryEncoderSpi.h"
#include "GateDriverSpi.h"
#include "SvmPwm.h"
#include "PhaseCurrentAdc.h"

#include "AS5047P.h"
#include "DRV8323.h"

#include "Transform.h"
#include "SVM.h"


static void SystemClock_Config(void);
uint8_t scalePulseWidth( float width );
uint8_t FloatToPWMValue(float value);
void main()
{
    HAL_Init();

    SystemClock_Config();

    // // Application code ...
    
    RotaryEncoderSpi_Config();
    GateDriverSpi_Config();
    SvmPwm_Config();
    PhaseCurrentAdc_Config();

    AS5047PInterface encoder =
    {
        .spiWrite = RotaryEncoderSpi_Write,
        .spiRead  = RotaryEncoderSpi_Read
    };
    AS5047P_SetInterface( &encoder );

    AS5047PZPOSL cfg =
    {
        .zposLSB = 17,
        .comp_l_error_en = 1,
        .comp_h_error_en = 0
    };

    DRV8323Interface gateDriver =
    {
        .spiWrite = GateDriverSpi_Write,
        .spiRead  = GateDriverSpi_Read,
        .adcReadRawPhaseA = PhaseCurrentAdc_GetRawPhaseA,
        .adcReadRawPhaseB = PhaseCurrentAdc_GetRawPhaseB,
        .adcReadRawPhaseC = PhaseCurrentAdc_GetRawPhaseC,
    };
    DRV8323_SetInterface( &gateDriver );

    DRV8323GateDriveHS hs = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };

    UsartPrint_Init();

    DRV8323CurrentSenseCfg iCfg =
    {
        .vRef = 3.3,
        .adcSteps = 4096,
        .rSense = 0.0015,
        .csaGain = 10
    };


    GPIO_InitTypeDef gateEnable =
    {
        .Pin = GPIO_PIN_11,
        .Mode = GPIO_MODE_OUTPUT_PP,
    };

    HAL_GPIO_Init( GPIOA, &gateEnable );

    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_11, GPIO_PIN_SET );
   
    //SvmPwm_SetPulseU( 0x09FF );

    //SvmPwm_SetPulseV( 0x05FF );

    ////SvmPwm_SetPulseW( 0x01FF );
    //SvmPwm_SetPulseW( 0x0EFF );
    //#define VS_SIZE 7
    //AlphaBeta vs[VS_SIZE] = 
    //{
    //    {0.577, 0.577},
    //    {-0.577, -0.577},
    //    {0.577, -0.577},
    //    {-0.577, 0.577},
    //    {0,0},
    //    {1.0,0},
    //    {0.0,1.0}
    //};

    //AlphaBeta v =
    //{
    //    .alpha = -0.577,
    //    .beta = -0.577,
    //};

    //IJK ijk = { 0 };
    //OnTimeT T = { 0 };
    //PulseWidths uvw = { 0 };
    //Sextant n;
    //SVM_ModifiedClarke( &v, &ijk );

    //printf("i = %f\n", ijk.i);
    //printf("j = %f\n", ijk.j);
    //printf("k = %f\n", ijk.k);

    //n = SVM_CalculateSextant( &ijk );

    //printf("n = %d\n", n);

    //SVM_DetermineOnTimeT( &T, &ijk, n );
    //
    //printf("Tx = %f\n", T.Tx);
    //printf("Ty = %f\n", T.Ty);
    //printf("T0 = %f\n", T.T0);

    //SVM_DeterminePulseWidths( &uvw, &T, n );

    //printf("u = %f\n", uvw.u);
    //printf("v = %f\n", uvw.v);
    //printf("w = %f\n", uvw.w);

    //SvmPwm_SetPulseU( 1.0f - uvw.u );

    //SvmPwm_SetPulseV( 1.0f - uvw.v );

    //SvmPwm_SetPulseW( 1.0f - uvw.w );

    //IJK ijk = { 0 };
    //OnTimeT T = { 0 };
    //PulseWidths uvw = { 0 };
    //Sextant n;

    //uint8_t i = 0;
    // Super loop
    //

    AS5047PANGLEUNC anglecom = { 0 };

    DRV8323FaultStatus1 fs = { 0 };

    DRV8323GateDriveHS high = { 0 };
    while (1)
    {
        //i = i % VS_SIZE;
        ////AS5047P_SetZPOSL( &cfg );
        ////DRV8323_SetGateDriveHS( &hs );
        //SVM_ModifiedClarke( &vs[i], &ijk );

        //printf("i = %f\n", ijk.i);
        //printf("j = %f\n", ijk.j);
        //printf("k = %f\n", ijk.k);

        //n = SVM_CalculateSextant( &ijk );

        //printf("n = %d\n", n);

        //SVM_DetermineOnTimeT( &T, &ijk, n );
        //
        //printf("Tx = %f\n", T.Tx);
        //printf("Ty = %f\n", T.Ty);
        //printf("T0 = %f\n", T.T0);

        //SVM_DeterminePulseWidths( &uvw, &T, n );

        //printf("u = %f\n", uvw.u);
        //printf("v = %f\n", uvw.v);
        //printf("w = %f\n", uvw.w);

        //SvmPwm_SetPulseU( 1.0f - uvw.u );

        //SvmPwm_SetPulseV( 1.0f - uvw.v );

        //SvmPwm_SetPulseW( 1.0f - uvw.w );

        //i++;

        //AS5047P_GetANGLEUNC( &anglecom );

        //printf( "Angle %d\n", anglecom.angleunc );

        DRV8323_GetFaultStatus1( &fs );

        printf("fault: %d\n", fs.fault );

        DRV8323_GetGateDriveHS( &high );

        printf("idrivephs: %d\n", high.idrivep_hs);

        HAL_Delay( 500 );
    }
}

uint8_t FloatToPWMValue(float value) {
    if (value < 0.0f) {
        return 0;  // Ensure the value isn't below 0
    } else if (value > 1.0f) {
        return 4096;  // Ensure the value isn't above 1
    } else {
        return (uint16_t)(value * 4096.0f);  // Scale the value to an 8-bit range
    }
}

#define STEP 0.0052
uint8_t scalePulseWidth( float width )
{
    //width = ( width > (4/3))? (4/3) : width;
    //width = ( width < 0 )? 0 : width;

    uint8_t scaled = width / STEP;

    printf( "0x%X\n", scaled);

    return ( width / STEP );
}


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  //__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  
}


void Systick_Handler()
{
    HAL_IncTick();
}
