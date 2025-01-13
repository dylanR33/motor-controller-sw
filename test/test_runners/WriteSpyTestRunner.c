#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( WriteSpy )
{
    RUN_TEST_CASE( WriteSpy, GetLastWrite );
    RUN_TEST_CASE( WriteSpy, Create );
}
