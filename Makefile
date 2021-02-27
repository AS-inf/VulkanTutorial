VULKAN_SDK_PATH = ~/Desktop/Vulkan/1.2.162.1/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -g
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan


compilation: src
	@mkdir build
	g++ $(CFLAGS) -o build/Triangle src/main.cpp src/vulkanApp.h src/vulkanApp.cpp src/libs.h $(LDFLAGS)

.PHONY: clean

build/Triangle: compilation
	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib
		@VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d
		@echo
		./build/Triangle


clean:
	rm -f build/Triangle


