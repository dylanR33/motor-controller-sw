#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( Transform )
{
    RUN_TEST_CASE( Transform, ClarkeSampleInput );
    RUN_TEST_CASE( Transform, InverseClarkeSampleInput );
}
