#include "FakeRead.h"

#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


TEST_GROUP( FakeRead );


uint16_t (*read)(uint16_t cmd);


TEST_SETUP( FakeRead )
{
    FakeRead_Create();
    read = FakeRead_Read;
}

TEST_TEAR_DOWN( FakeRead )
{
}

TEST( FakeRead, Create )
{
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd() );
    TEST_ASSERT_EQUAL( 0, read( 0 ) );
}


TEST( FakeRead, SetNextReading )
{
    FakeRead_SetNextReading( 2 );

    TEST_ASSERT_EQUAL( 2, read( 0 ) );
}

TEST( FakeRead, GetLastReading )
{
    read( 3 );

    TEST_ASSERT_EQUAL( 3, FakeRead_GetLastCmd() );
}




