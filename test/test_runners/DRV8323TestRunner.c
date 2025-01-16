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
    RUN_TEST_CASE( DRV8323, DriverCtrlMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, GateDriveHSMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, GateDriveLSMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, OCPCtrlMemberBitsPlacedCorrectly );
    RUN_TEST_CASE( DRV8323, CSACtrlMemberBitsPlacedCorrectly );
}
