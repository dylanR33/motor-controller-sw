#include "PIController.h"

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP( PI );


TEST_SETUP( PI )
{
}

TEST_TEAR_DOWN( PI )
{
}

TEST( PI, PISampleInput )
{
    PIController pi =
    {
        .processVar = 0.0f,
        .pGain = 1.0f,
        .iGain = 1.0f,
        .integral = 0.0f,
        .T = 0.2f,
        .awGain = 1.0f,
        .min = 0,
        .max = 100,
        .prevCmdUnsat = 0,
        .prevCmdSat = 0
    };
    float setPoint = 70;
    TEST_ASSERT_FLOAT_WITHIN( 0.01, 84.0, PIController_Control( setPoint, &pi ) );
}

