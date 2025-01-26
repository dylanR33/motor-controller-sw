#include "InterpolatedLookup.h"

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP( InterpolatedLookup );


TEST_SETUP( InterpolatedLookup )
{
}

TEST_TEAR_DOWN( InterpolatedLookup )
{
}


TEST( InterpolatedLookup, InterpolateSaturateLow )
{

    Interpolation i =
    {
        .x  = 1.0,
        .x0 = 2.0,
        .x1 = 4.0,
        .y0 = 1.0,
        .y1 = 3.0
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, i.y0 , InterpolatedLookup_Interpolate( &i ) );
}

TEST( InterpolatedLookup, InterpolateSaturateHigh )
{

    Interpolation i =
    {
        .x  = 5.0,
        .x0 = 2.0,
        .x1 = 4.0,
        .y0 = 1.0,
        .y1 = 3.0
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, i.y1 , InterpolatedLookup_Interpolate( &i ) );
}

TEST( InterpolatedLookup, InterpolateSampleInput )
{

    Interpolation i =
    {
        .x  = 3.0,
        .x0 = 2.0,
        .x1 = 4.0,
        .y0 = 1.0,
        .y1 = 3.0
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, 2.0, InterpolatedLookup_Interpolate( &i ) );
}

TEST( InterpolatedLookup, InterpolateLUTOutOfLowerBound )
{
    float x = 0.5;

    // y = 2x ( 1.0 <= x <= 5.0 )
    float y_vals[] = {2.0, 4.0, 6.0, 10.0};
    float x_vals[] = {1.0, 2.0, 3.0, 5.0 };

    LookupTable lut = 
    {
        .length = 4,
        .x_values = x_vals,
        .y_values = y_vals
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, y_vals[0], InterpolatedLookup_InterpolateLUT( x, &lut ) );
}

TEST( InterpolatedLookup, InterpolateLUTOutOfUpperBound )
{
    float x = 5.5;

    // y = 2x ( 1.0 <= x <= 5.0 )
    float y_vals[] = {2.0, 4.0, 6.0, 10.0};
    float x_vals[] = {1.0, 2.0, 3.0, 5.0 };

    LookupTable lut = 
    {
        .length = 4,
        .x_values = x_vals,
        .y_values = y_vals
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, y_vals[ lut.length - 1 ], InterpolatedLookup_InterpolateLUT( x, &lut ) );
}

TEST( InterpolatedLookup, InterpolateLUTSampleInput )
{
    float x = 2.5;

    // y = 2x ( 1.0 <= x <= 5.0 )
    float y_vals[] = {2.0, 4.0, 6.0, 10.0};
    float x_vals[] = {1.0, 2.0, 3.0, 5.0 };

    LookupTable lut = 
    {
        .length = 4,
        .x_values = x_vals,
        .y_values = y_vals
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.1, 5.0, InterpolatedLookup_InterpolateLUT( x, &lut ) );
}

