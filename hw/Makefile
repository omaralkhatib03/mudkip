BUILD_DIR := build
CURRENT := basic_sync_fifo
TARGETS := basic_sync_fifo spmv_network_op_tb spmv_network_tb row_decoder_tb fproduct 
JOBS := 12

.PHONY: all
all: $(CURRENT)

.PHONY: default
default: $(TARGETS)

.PHONY: configure
configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR); \
	fi

$(TARGETS): configure
	cmake --build $(BUILD_DIR) --target $@ -- -j$(JOBS)
	ln -sf lib/libgmp.so.11 libgmp.so.11
	ln -sf lib/libmpfr.so.4 libmpfr.so.4

.PHONY: clean
clean:
	@echo "Cleaning build artifacts (keeping build directory)..."
	rm -rf waves
	rm -rf libgmp.so.11
	rm -rf libmpfr.so.4
	@cmake --build $(BUILD_DIR) --target clean

.PHONY: deep_clean
deep_clean:
	@echo "Removing build directory..."
	@rm -rf $(BUILD_DIR)


.PHONY: test
test: $(TARGETS)
	@echo "Running tests for all binaries..."
	@for binary in $(TARGETS); do \
		if [ -f "$(BUILD_DIR)/bin/$$binary" ]; then \
			echo "Running $$binary..."; \
			./$(BUILD_DIR)/bin/$$binary --gtest_output=xml:./test_results/$$binary.xml; \
			if [ $$? -ne 0 ]; then \
				echo "Test failed for $$binary"; \
				exit 1; \
			fi; \
		fi; \
	done
	@echo "All tests have been executed."

