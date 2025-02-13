#ifndef __SVM_PWM_H__
#define __SVM_PWM_H__

#include <stdint.h>

void SvmPwm_Config();

void SvmPwm_SetPulseU( float pulse );

void SvmPwm_SetPulseV( float pulse );

void SvmPwm_SetPulseW( float pulse );

#endif

