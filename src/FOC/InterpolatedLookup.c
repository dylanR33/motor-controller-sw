#include "InterpolatedLookup.h"


float InterpolatedLookup_Interpolate( Interpolation* i)
{
    if ( i->x <= i->x0 )
        return i->y0;

    if ( i->x >= i->x1 )
        return i->y1;

    return i->y0 + ( ( i->x - i->x0 ) / ( i->x1 - i->x0 ) ) * ( i->y1 - i->y0 );
}


float InterpolatedLookup_InterpolateLUT( float x, const LookupTable* lut )
{
    // x is under lower bound -> saturate to lowest y value
    if ( x <= lut->x_values[0] )
    {
        return lut->y_values[0];
    } // x is over upper bound -> saturate to highest y value
    else if ( x >= lut->x_values[ lut->length - 1 ] )
    {
        return lut->y_values[ lut->length - 1 ];
    }

    // Search for x0 <= x <= x1
    for ( uint8_t point = 0; point < ( lut->length - 1 ); point++ )
    {
        if ( ( lut->x_values[ point ] <= x ) && ( lut->x_values[ point + 1 ] >= x ) )
        {
            Interpolation i = 
            {
                .x = x,
                .x0 = lut->x_values[ point ],
                .x1 = lut->x_values[ point + 1 ],
                .y0 = lut->y_values[ point ],
                .y1 = lut->y_values[ point + 1 ]
            };
            return InterpolatedLookup_Interpolate( &i );
        }
    }

    // If all checks fail return min
    return lut->y_values[ 0 ];
}

