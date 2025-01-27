#include "Transform.h"

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP( Transform );


TEST_SETUP( Transform )
{
}

TEST_TEAR_DOWN( Transform )
{
}


TEST( Transform, ClarkeSampleInput )
{
    PhaseVariables abc =
    {
        .a = 3,
        .b = 5,
        .c = 6
    };

    AlphaBeta alphaBeta = { 0 };

    Transform_Clarke( &abc, &alphaBeta );

    TEST_ASSERT_EQUAL( abc.a, alphaBeta.alpha );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, 7.506, alphaBeta.beta  );
}

TEST( Transform, InverseClarkeSampleInput )
{
    AlphaBeta alphaBeta =
    {
        .alpha = 5,
        .beta = 2
    };

    PhaseVariables abc = { 0 };

    Transform_InverseClarke( &alphaBeta, &abc );

    TEST_ASSERT_EQUAL( alphaBeta.alpha, abc.a      );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, -0.7679, abc.b  );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, -4.2321, abc.c  );
}

TEST( Transform, ParkSampleInput )
{
    AlphaBeta alphaBeta =
    {
        .alpha = 3.0,
        .beta = 5.3
    };

    float theta = 3.259;

    DirectQuadrature dq = { 0 };

    Transform_Park( &alphaBeta, &dq , theta);

    TEST_ASSERT_FLOAT_WITHIN( 0.015, -3.6002, dq.d );
    TEST_ASSERT_FLOAT_WITHIN( 0.015, -4.9120, dq.q );
}

TEST( Transform, InverseParkSampleInput )
{
    DirectQuadrature dq =
    {
        .d = 2.59,
        .q = 3.6
    };

    float theta = 2.52;

    AlphaBeta alphaBeta = { 0 };

    Transform_InversePark( &dq, &alphaBeta, theta );
    
    TEST_ASSERT_FLOAT_WITHIN( 0.015, -4.2019, alphaBeta.alpha );
    TEST_ASSERT_FLOAT_WITHIN( 0.015, -1.4184, alphaBeta.beta  );
}

