#ifndef __SVM_PWM_H__
#define __SVM_PWM_H__

#include <stdint.h>

void SvmPwm_Config();

void SvmPwm_SetPulseU( uint16_t pulse );

void SvmPwm_SetPulseV( uint16_t pulse );

void SvmPwm_SetPulseW( uint16_t pulse );

#endif

