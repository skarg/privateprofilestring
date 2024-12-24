.PHONY: all
all: test

BUILD_DIR=build

CTEST_OPTIONS := --quiet --output-on-failure
CTEST_OPTIONS += --output-junit test-results.xml

.PHONY: test
test:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake .. && cd ..
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake --build . --clean-first && cd ..
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && ctest $(CTEST_OPTIONS)
	[ -d $(BUILD_DIR) ] && $(MAKE) -C $(BUILD_DIR) lcov

.PHONY: retest
retest:
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake --build . && cd ..
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && ctest $(CTEST_OPTIONS)
	[ -d $(BUILD_DIR) ] && $(MAKE) -C $(BUILD_DIR) lcov

.PHONY: report
report:
	[ -d $(BUILD_DIR) ] && cat $(BUILD_DIR)/Testing/Temporary/LastTest*.log

.PHONY: rebuild
rebuild:
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake --build . --clean-first && cd ..

.PHONY: env
env:
	@echo "Makefile environment variables"
	@echo "MAKEFLAGS=$(MAKEFLAGS)"
	@echo "CTEST_OPTIONS=$(CTEST_OPTIONS)"
	@echo "BUILD_DIR=$(BUILD_DIR)"

.PHONY: clean
clean:
	-rm -rf $(BUILD_DIR)
