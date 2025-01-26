#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( InterpolatedLookup )
{
    RUN_TEST_CASE( InterpolatedLookup, InterpolateSaturateLow );
    RUN_TEST_CASE( InterpolatedLookup, InterpolateSaturateHigh );
    RUN_TEST_CASE( InterpolatedLookup, InterpolateSampleInput );
    RUN_TEST_CASE( InterpolatedLookup, InterpolateLUTOutOfLowerBound );
    RUN_TEST_CASE( InterpolatedLookup, InterpolateLUTOutOfUpperBound );
    RUN_TEST_CASE( InterpolatedLookup, InterpolateLUTSampleInput );
}
