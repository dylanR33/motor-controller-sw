#include "WriteSpy.h"

#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


TEST_GROUP( WriteSpy );


void (*write)(uint16_t cmd);


TEST_SETUP( WriteSpy )
{
}

TEST_TEAR_DOWN( WriteSpy )
{
}

TEST( WriteSpy, Create )
{
    WriteSpy_Create();
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite() );
}


TEST( WriteSpy, GetLastWrite )
{
    WriteSpy_Create();

    write = WriteSpy_Write;

    write( 4 );

    TEST_ASSERT_EQUAL( 4, WriteSpy_GetLastWrite() );
} 
