#include "SVM.h"

#include "Transform.h"

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP( SVM );


TEST_SETUP( SVM )
{
}

TEST_TEAR_DOWN( SVM )
{
}

TEST( SVM, ModifiedClarkeSampleInput )
{
    AlphaBeta ab = 
    {
        .alpha = 1.3,
        .beta  = 0.8
    };

    IJK ijk = { 0 };

    SVM_ModifiedClarke( &ab, &ijk );

    TEST_ASSERT_FLOAT_WITHIN( 0.001, 0.7258,   ijk.i );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, 0.8,      ijk.j );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, -1.5258 , ijk.k );
}

TEST( SVM, CalculateSextantSampleInput )
{
    IJK ijk =
    {
        .i = 0.7258,
        .j = 0.8,
        .k = -1.5258
    };

    TEST_ASSERT_EQUAL( SEXTANT_1, SVM_CalculateSextant( &ijk ) );
}

TEST( SVM, DetermineOnTimeTSampleInput )
{
    IJK ijk =
    {
        .i = 0.7258,
        .j = 0.8,
        .k = -1.5258
    };

    Sextant n = SVM_CalculateSextant( &ijk );

    OnTimeT T = { 0 };

    SVM_DetermineOnTimeT( &T, &ijk, n );

    TEST_ASSERT_FLOAT_WITHIN( 0.01, ijk.i                , T.Tx );
    TEST_ASSERT_FLOAT_WITHIN( 0.01, ijk.j                , T.Ty );
    TEST_ASSERT_FLOAT_WITHIN( 0.01, 1 - ( ijk.i + ijk.j ), T.T0 );
}

TEST( SVM, DeterminePulseWidthSampleInput )
{
    IJK ijk =
    {
        .i = 0.7258,
        .j = 0.8,
        .k = -1.5258
    };

    Sextant n = SVM_CalculateSextant( &ijk );

    OnTimeT T = { 0 };

    PulseWidths uvw = { 0 };

    SVM_DetermineOnTimeT( &T, &ijk, n );

    SVM_DeterminePulseWidths( &uvw, &T, n );

    TEST_ASSERT_FLOAT_WITHIN( 0.01, 1.2629, uvw.u );
    TEST_ASSERT_FLOAT_WITHIN( 0.01, 0.5371, uvw.v );
    TEST_ASSERT_FLOAT_WITHIN( 0.01, -0.2629, uvw.w );
}
