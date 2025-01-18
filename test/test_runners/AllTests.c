#include "unity_fixture.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP( DRV8323  );
    RUN_TEST_GROUP( FakeRead );
    RUN_TEST_GROUP( WriteSpy );
    RUN_TEST_GROUP( AS5047P  );
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
