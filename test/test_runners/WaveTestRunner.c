#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( Wave )
{
    RUN_TEST_CASE( Wave, SinSampleInputQuadrant1 );
    RUN_TEST_CASE( Wave, SinSampleInputQuadrant2 );
    RUN_TEST_CASE( Wave, SinSampleInputQuadrant3 );
    RUN_TEST_CASE( Wave, SinSampleInputQuadrant4 );
    RUN_TEST_CASE( Wave, CosSampleInputQuadrant1 );
    RUN_TEST_CASE( Wave, CosSampleInputQuadrant2 );
    RUN_TEST_CASE( Wave, CosSampleInputQuadrant3 );
    RUN_TEST_CASE( Wave, CosSampleInputQuadrant4 );
}
