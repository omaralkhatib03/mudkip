BUILD_DIR := build
CURRENT := spmv_network_tb
TARGETS := basic_sync_fifo spmv_network_op_tb spmv_network_tb row_decoder_tb fproduct vector_ram_tb 
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

