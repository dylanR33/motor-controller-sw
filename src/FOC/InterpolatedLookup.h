#ifndef __INTERPOLATED_LOOKUP_H__
#define __INTERPOLATED_LOOKUP_H__

#define PI_2 1.570796327


typedef struct
{
    float x;
    float x0;
    float x1;
    float y0;
    float y1;
} Interpolation;


float InterpolatedLookup_Interpolate( Interpolation* i);


#endif
