#ifndef __UVW_TIM_H__
#define __UVW_TIM_H__

#include <stdint.h>

void TimerConfig();

void PwmAdjustPhaseA( uint16_t pulse );

void PwmAdjustPhaseB( uint16_t pulse );

void PwmAdjustPhaseC( uint16_t pulse );

#endif

