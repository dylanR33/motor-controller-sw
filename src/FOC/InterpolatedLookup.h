#ifndef __INTERPOLATED_LOOKUP_H__
#define __INTERPOLATED_LOOKUP_H__


#include <stdint.h>


typedef struct
{
    float x;
    float x0;
    float x1;
    float y0;
    float y1;
} Interpolation;


typedef struct
{
    uint8_t length;
    const float*  x_values;
    const float*  y_values;
} LookupTable;


float InterpolatedLookup_Interpolate( Interpolation* i);


float InterpolatedLookup_InterpolateLUT( float x, const LookupTable* lut );


#endif
