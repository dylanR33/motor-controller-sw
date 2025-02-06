#ifndef __PI_CONTROLLER_H__
#define __PI_CONTROLLER_H__

#include <stdint.h>

typedef struct
{
    float processVar;
    float pGain;            // Proportional gain
    float iGain;            // Integral gain
    float integral;         // Integral carry
    float T;                // Sampling period
    float awGain;           // Antiwindup gain
    float min;              // Process variable min
    float max;              // Process variable max
    float prevCmdUnsat;     // previous unsaturated command  (used for antiwindup)
    float prevCmdSat;       // previous saturated command    (used for antiwindup)
} PIController;

float PIController_Control( float setPoint, PIController* pi );

#endif
