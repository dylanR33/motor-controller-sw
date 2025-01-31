#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_flash_ex.h"

#include "RotaryEncoderSpi.h"
#include "GateDriverSpi.h"
#include "SvmPwm.h"
#include "PhaseCurrentAdc.h"

#include "AS5047P.h"
#include "DRV8323.h"

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
        .spiRead  = GateDriverSpi_Read
    };
    DRV8323_SetInterface( &gateDriver );

    DRV8323GateDriveHS hs = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };
   
    HAL_Delay(1);

    // Super loop
    while (1)
    {
        AS5047P_SetZPOSL( &cfg );
        DRV8323_SetGateDriveHS( &hs );

        SvmPwm_SetPulseA( 0x07FF );
        HAL_Delay( 150 );

        SvmPwm_SetPulseA( 0x05FF );
        HAL_Delay( 150 );

        SvmPwm_SetPulseA( 0x04FF );
        HAL_Delay( 150 );
   }
}

void Systick_Handler()
{
    HAL_IncTick();
}
