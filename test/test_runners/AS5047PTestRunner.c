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
    RUN_TEST_CASE( AS5047P, SetPROGMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( AS5047P, SetZPOSMMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( AS5047P, SetZPOSLMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( AS5047P, SetSETTINGS1MemberBitsPlacedCorrectly );
    RUN_TEST_CASE( AS5047P, SetSETTINGS2MemberBitsPlacedCorrectly );
    RUN_TEST_CASE( AS5047P, SetPROGFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( AS5047P, SetZPOSLFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( AS5047P, SetSETTINGS1FailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( AS5047P, SetSETTINGS2FailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( AS5047P, GetERRFLMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetPROGMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetDIAAGCMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetMAGMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetANGLEUNCMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetANGLECOMMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetZPOSMMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetZPOSLMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetSETTINGS1MemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, GetSETTINGS2MemberBitsExtractedCorrectly );
    RUN_TEST_CASE( AS5047P, SetZPOSLFullFrameCheck );
}
