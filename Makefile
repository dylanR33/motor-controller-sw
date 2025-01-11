UTFS_MODULE_DIRS = src/drivers/DRV8323
UTFS_TEST_DIR = test
UTFS_BUILD_DIR = build

.PHONY: clean
clean:
	rm -r build


include tools/unity_test_fixture_setup/unity_test_fixture_setup.mk

