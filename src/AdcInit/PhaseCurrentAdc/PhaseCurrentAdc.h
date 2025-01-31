#ifndef __PHASE_CURRENT_ADC_H__
#define __PHASE_CURRENT_ADC_H__

#include <stdint.h>

void PhaseCurrentAdc_Config();

uint32_t PhaseCurrentAdc_GetPhaseA();

uint32_t PhaseCurrnetAdc_GetPhaseB();

uint32_t PhaseCurrentAdc_GetPhaseC();

#endif
