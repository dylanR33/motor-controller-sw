#ifndef __SVM_H__
#define __SVM_H__

#include <stdint.h>

#include "Transform.h"


typedef struct
{
    float i;
    float j;
    float k;
} IJK;


typedef struct
{
    float Tx;
    float Ty;
    float T0;
} OnTimeT;


typedef struct
{
    float u;
    float v;
    float w;
} PulseWidths;


typedef enum
{
    SEXTANT_1 = 1,
    SEXTANT_2,
    SEXTANT_3,
    SEXTANT_4,
    SEXTANT_5,
    SEXTANT_6
}Sextant;


void SVM_ModifiedClarke( AlphaBeta* alphaBeta, IJK* ijk );

Sextant SVM_CalculateSextant( IJK* ijk );

void SVM_DetermineOnTimeT( OnTimeT* T, IJK* ijk, Sextant nSextant );

void SVM_DeterminePulseWidths( PulseWidths* uvw, OnTimeT* T, Sextant nSextant );

#endif
