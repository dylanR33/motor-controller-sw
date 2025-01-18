#include "WriteSpy.h"

#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


TEST_GROUP( WriteSpy );


void (*write16)(uint16_t cmd);
void (*write32)(uint32_t cmd);


TEST_SETUP( WriteSpy )
{
    WriteSpy_Create();
    write16 = WriteSpy_Write16;
    write32 = WriteSpy_Write32;
}

TEST_TEAR_DOWN( WriteSpy )
{
}

TEST( WriteSpy, Create )
{
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}


TEST( WriteSpy, GetLastWrite )
{
    write16( 4 );
    TEST_ASSERT_EQUAL( 4, WriteSpy_GetLastWrite16() );

    write32( 5 );
    TEST_ASSERT_EQUAL( 5, WriteSpy_GetLastWrite32() );
} 
