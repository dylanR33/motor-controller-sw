#ifndef __SVM_PWM_H__
#define __SVM_PWM_H__

#include <stdint.h>

void SvmPwm_Config();

void SvmPwm_SetPulseA( uint16_t pulse );

void SvmPwm_SetPulseB( uint16_t pulse );

void SvmPwm_SetPulseC( uint16_t pulse );

#endif

