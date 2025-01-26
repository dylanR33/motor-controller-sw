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


