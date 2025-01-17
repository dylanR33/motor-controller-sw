#include "FakeRead.h"

#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


TEST_GROUP( FakeRead );


uint16_t (*read16)(uint16_t cmd);
uint32_t (*read32)(uint32_t cmd);


TEST_SETUP( FakeRead )
{
    FakeRead_Create();
    read16 = FakeRead_Read16;
    read32 = FakeRead_Read32;
}

TEST_TEAR_DOWN( FakeRead )
{
}

TEST( FakeRead, Create )
{
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd16() );
    TEST_ASSERT_EQUAL( 0, read16( 0 ) );

    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd32() );
    TEST_ASSERT_EQUAL( 0, read32( 0 ) );
}


TEST( FakeRead, SetNextReading )
{
    FakeRead_SetNextReading16( 2 );
    TEST_ASSERT_EQUAL( 2, read16( 0 ) );

    FakeRead_SetNextReading32( 5 );
    TEST_ASSERT_EQUAL( 5, read32( 0 ) );
}

TEST( FakeRead, GetLastReading )
{
    read16( 3 );
    TEST_ASSERT_EQUAL( 3, FakeRead_GetLastCmd16() );

    read32( 5 );
    TEST_ASSERT_EQUAL( 5, FakeRead_GetLastCmd32() );
}




