#include "unity_fixture.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP( FakeRead           );
    RUN_TEST_GROUP( WriteSpy           );
    RUN_TEST_GROUP( DRV8323            );
    RUN_TEST_GROUP( AS5047P            );
    RUN_TEST_GROUP( Transform          );
    RUN_TEST_GROUP( InterpolatedLookup );
    RUN_TEST_GROUP( Wave               );
    RUN_TEST_GROUP( PI                 );
    RUN_TEST_GROUP( SVM                );
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
