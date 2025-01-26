#include "Wave.h"

#include "InterpolatedLookup.h"


#define PI 3.141592654
#define PI_2 (PI / 2)


static const float sin_x[] = 
{
    0.0,
    0.15707963267948966,
    0.3141592653589793,
    0.47123889803846897,
    0.6283185307179586,
    0.7853981633974483,
    0.9424777960769379,
    1.0995574287564276,
    1.2566370614359172,
    1.413716694115407,
    1.5707963267948966
};


static const float sin_y[] =
{
    0.0,
    0.15643446504023087,
    0.3090169943749474,
    0.45399049973954675,
    0.5877852522924731,
    0.7071067811865475,
    0.8090169943749475,
    0.8910065241883678,
    0.9510565162951535,
    0.9876883405951378,
    1.0
};


static const LookupTable sinLUT =
{
    .length = 11,
    .x_values = sin_x,
    .y_values = sin_y
};


static float fmodf( float x, float y )
{
    return ( x - ( (int32_t)( x / y ) ) * y );
}


static float normalize_radians( float rad )
{
    rad = fmodf( rad, 2*PI );

    if ( rad < 0 )
        return rad + ( 2*PI );

    return rad;
}


float Wave_Sin( float rad )
{
    rad = normalize_radians( rad );

    float y = 0.0f;
    
    if ( rad < PI_2 )
    {
        y = InterpolatedLookup_InterpolateLUT( rad, &sinLUT );
    }
    else if ( rad < PI )
    {
        y = InterpolatedLookup_InterpolateLUT( PI - rad, &sinLUT );
    }
    else if ( rad < ( PI + PI_2 ) )
    {
        y = -InterpolatedLookup_InterpolateLUT( rad - PI, &sinLUT );
    }
    else
    {
        y = -InterpolatedLookup_InterpolateLUT( (2*PI) - rad, &sinLUT );
    }

    return y;
}


float Wave_Cos( float rad )
{
    return Wave_Sin( PI_2 - rad );
}
