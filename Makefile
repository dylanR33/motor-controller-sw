UTFS_MODULE_DIRS = test/fakes test/spys src/drivers/DRV8323
UTFS_TEST_DIR = test
UTFS_BUILD_DIR = build

CLISTM_SRC_DIRS = src src/drivers/DRV8323
CLISTM_BUILD_DIR = build
CLISTM_MODEL_NUM = STM32F411xE
CLISTM_HAL_MODULES = gpio

.PHONY: clean
clean:
	rm -r build


include tools/unity_test_fixture_setup/unity_test_fixture_setup.mk
include tools/command_line_stm32/command_line_stm32.mk

