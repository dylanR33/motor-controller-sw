#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER( SVM )
{
    RUN_TEST_CASE( SVM, ModifiedClarkeSampleInput );
    RUN_TEST_CASE( SVM, CalculateSextantSampleInput );
    RUN_TEST_CASE( SVM, DetermineOnTimeTSampleInput );
    RUN_TEST_CASE( SVM, DeterminePulseWidthSampleInput );
}
