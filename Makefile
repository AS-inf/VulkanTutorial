VULKAN_SDK_PATH = ~/Desktop/Vulkan/1.2.162.1/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -g
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan


compilation: src/main.cpp
	g++ $(CFLAGS) -o Triangle src/main.cpp src/vulkanApp.h src/vulkanApp.cpp src/libs.h $(LDFLAGS)

.PHONY: clean

Triangle: compilation
	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib
		@VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d
		@echo
		./Triangle


clean: RUN
	rm -f Triangle


