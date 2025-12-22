BUILD_DIR := build
TARGET := ogl_engine_test

.PHONY: all configure build run clean rebuild

all: build

configure:
	cmake -S . -B$(BUILD_DIR)
	
build: configure
	cmake --build $(BUILD_DIR) -j
	
run: build
	./$(BUILD_DIR)/$(TARGET)
	
clean:
	rm -rf $(BUILD_DIR)
	
	
rebuild: clean build
	
