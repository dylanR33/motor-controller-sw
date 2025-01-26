#include "Wave.h"

#include "unity.h"
#include "unity_fixture.h"


float pi_4 = 0.785398163;
float sqrt2_2 = 0.707106781;
float fivePi_6 = 2.617993878;
float sevenPi_6 = 3.665191429;
float fivePi_3 = 5.235987756;
float sqrt3_2 = 0.866025404;

TEST_GROUP( Wave );


TEST_SETUP( Wave )
{
}

TEST_TEAR_DOWN( Wave )
{
}


TEST( Wave, SinSampleInputQuadrant1 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.001, sqrt2_2, Wave_Sin( pi_4 ) );
}

TEST( Wave, SinSampleInputQuadrant2 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, 0.5, Wave_Sin( fivePi_6 ) );
}

TEST( Wave, SinSampleInputQuadrant3 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, -0.5, Wave_Sin( sevenPi_6 ) );
}

TEST( Wave, SinSampleInputQuadrant4 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, -sqrt3_2, Wave_Sin( fivePi_3 ) );
}

TEST( Wave, CosSampleInputQuadrant1 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.001, sqrt2_2, Wave_Cos( pi_4 ) );
}

TEST( Wave, CosSampleInputQuadrant2 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, -sqrt3_2, Wave_Cos( fivePi_6 ) );
}

TEST( Wave, CosSampleInputQuadrant3 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, -sqrt3_2, Wave_Cos( sevenPi_6 ) );
}

TEST( Wave, CosSampleInputQuadrant4 )
{
    TEST_ASSERT_FLOAT_WITHIN( 0.01, 0.5, Wave_Cos( fivePi_3 ) );
}
