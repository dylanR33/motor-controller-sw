#ifndef __PHASE_CURRENT_ADC_H__
#define __PHASE_CURRENT_ADC_H__

#include <stdint.h>

void PhaseCurrentAdc_Config();

uint32_t PhaseCurrentAdc_GetRawPhaseA();

uint32_t PhaseCurrentAdc_GetRawPhaseB();

uint32_t PhaseCurrentAdc_GetRawPhaseC();

#endif
