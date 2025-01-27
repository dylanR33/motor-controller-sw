sources = src/drivers/DRV8323 src/drivers/AS5047P src/FOC/Transform \
		  src/FOC/Wave src/FOC/InterpolatedLookup

build = build

UTFS_MODULE_DIRS = test/fakes test/spys $(sources)
UTFS_TEST_DIR = test
UTFS_BUILD_DIR = $(build)

CLISTM_SRC_DIRS = src $(sources)
CLISTM_BUILD_DIR = $(build)
CLISTM_MODEL_NUM = STM32F446xx
CLISTM_HAL_MODULES = gpio

.PHONY: clean
clean:
	rm -r build


include tools/unity-test-fixture-setup/unity_test_fixture_setup.mk
include tools/command-line-stm32f4/command_line_stm32f4.mk

