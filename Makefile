VULKAN_SDK_PATH = ~/Desktop/Vulkan/1.2.162.1/x86_64
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -g
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan -Wl,-M


compilation: src clean
	@mkdir build
	g++ $(CFLAGS) -o build/Triangle src/main.cpp src/vulkanApp.h src/vulkanApp.cpp src/libs.h src/globals.h src/globals.cpp $(LDFLAGS)  > compiler.log

.PHONY: clean

Triangle: compilation
	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib
		@VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d
		@echo
		./build/Triangle


clean:
	rm -rf build



#VULKAN_SDK_PATH = ~/Desktop/Vulkan/1.2.162.1/x86_64
#CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -g
#LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
#
#
#Triangle: compilation
#	@LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib
#		@VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d
#		@echo
#		./build/Triangle
#
#
#compilation: src clean build build/vulkan.a
#	g++ $(CFLAGS) -o build/Triangle src/main.cpp src/libs.h src/globals.h src/globals.cpp build/vulkan.a $(LDFLAGS)
#
#
#
#build/vulkan.a: src/vulkanApp.cpp src/vulkanApp.h build
#	g++ $(CFLAGS) -c -o build/vulkan.o src/vulkanApp.cpp src/vulkanApp.h src/libs.h
#	ar -rcs build/vulkan.a build/vulkan.o
#	ranlib build/vulkan.a
#
#build:
#	mkdir build
#
#
#.PHONY: clean
#clean:
#	rm -rf build


