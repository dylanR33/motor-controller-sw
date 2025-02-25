#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( AS5047P )
{
    RUN_TEST_CASE( AS5047P, SetInterface_RequireBothInterfaces );
    RUN_TEST_CASE( AS5047P, WriteBit30Low );
    RUN_TEST_CASE( AS5047P, ReadBit30and14High );
    RUN_TEST_CASE( AS5047P, PreventWriteToReadOnlyAddresses );
    RUN_TEST_CASE( AS5047P, WriteCmdOutParityAndEFCheck );
    RUN_TEST_CASE( AS5047P, ReadCmdOutParityCheck );
    RUN_TEST_CASE( AS5047P, PreventReadFromOutOfBoundsAddress );
    RUN_TEST_CASE( AS5047P, ReadEnsuresParityBitsMatch );
    RUN_TEST_CASE( AS5047P, ReadEnsuresEFBackIsLow );
    RUN_TEST_CASE( AS5047P, SetZPOSLFullFrameCheck );
}
