#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( AS5047P )
{
    RUN_TEST_CASE( AS5047P, SetInterface_RequireBothInterfaces );
    RUN_TEST_CASE( AS5047P, WriteBit30Low );
    RUN_TEST_CASE( AS5047P, ReadBit30High );
    RUN_TEST_CASE( AS5047P, PreventWriteToReadOnlyAddresses );
    RUN_TEST_CASE( AS5047P, WriteCmdOutParityAndEFBitsLow );
    RUN_TEST_CASE( AS5047P, ReadCmdOutParityAndEFBitsLow );
    RUN_TEST_CASE( AS5047P, ReadPlaceAddressBitsInBits29Through16 );
    RUN_TEST_CASE( AS5047P, WritePlaceAddressBitsInBits29Through16 );
    RUN_TEST_CASE( AS5047P, RestrictDataBitsToBits13Through0 );
    RUN_TEST_CASE( AS5047P, ReadDataBitsCleared );
    RUN_TEST_CASE( AS5047P, PreventReadFromOutOfBoundsAddress );
    RUN_TEST_CASE( AS5047P, ReadEnsuresParityBitsBackAreSet );
    RUN_TEST_CASE( AS5047P, ReadEnsuresEFBackIsLow );
    RUN_TEST_CASE( AS5047P, SetZPOSLFullFrameCheck );
}
