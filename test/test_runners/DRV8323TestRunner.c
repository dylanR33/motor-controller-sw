#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(DRV8323)
{
    RUN_TEST_CASE(DRV8323, SetInterface_RequireBothInterfaces);
    RUN_TEST_CASE( DRV8323, WriteMSBLow );
    RUN_TEST_CASE( DRV8323, ReadMSBHigh );
    RUN_TEST_CASE( DRV8323, PreventWriteToOutOfBoundsAddresses );
    RUN_TEST_CASE( DRV8323, PreventReadToOutOfBoundsAddresses );
    RUN_TEST_CASE( DRV8323, PreventWriteToReadOnlyAddresses );
    RUN_TEST_CASE( DRV8323, WritePlaceAddressInBits14Through11 );
    RUN_TEST_CASE( DRV8323, RestrictDataToBits10Through0 );
    RUN_TEST_CASE( DRV8323, ReadDataBitsCleared );
    RUN_TEST_CASE( DRV8323, ReadPlaceAddressInBits14Through11 );
    RUN_TEST_CASE( DRV8323, SetDriverCtrlMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, SetGateDriveHSMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, SetGateDriveLSMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, SetOCPCtrlMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, SetCSACtrlMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, SetDriverCtrlFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( DRV8323, SetGateDriveHSFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( DRV8323, SetGateDriveLSFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( DRV8323, SetOCPCtrlFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( DRV8323, SetCSACtrlFailIfMembersBitsExceedExpected );
    RUN_TEST_CASE( DRV8323, GetFaultStatus1MemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetFaultStatus2MemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetDriverCtrlMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetGateDriveHSMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetGateDriveLSMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetOCPCtrlMemberBitsExtractedCorrectly );
    RUN_TEST_CASE( DRV8323, GetCSACtrlMemberBitsExtractedCorrectly );
}
