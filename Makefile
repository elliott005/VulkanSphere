CFLAGS = -std=c++17 -Wall# -O2
LDFLAGS = -lvulkan-1 -lgdi32
LIBRARIES = -IC:/Users/ellio/Documents/coding/C++/glfw-3.4/include -IC:/Users/ellio/Documents/coding/C++/glm -IC:/VulkanSDK/1.4.304.1/Include -LC:/VulkanSDK/1.4.304.1/Lib

compile: main.cpp
	g++ $(CFLAGS) $(LIBRARIES) main.cpp src/*.cpp C:/Users/ellio/Documents/coding/C++/glfw-3.4/lib-mingw-w64/libglfw3dll.a $(LDFLAGS) -o VulkanTest.out

compile_shaders:
	C:/VulkanSDK/1.4.304.1/Bin/glslc.exe shaders/defaultShader.vert -o compiledShaders/vert.spv
	C:/VulkanSDK/1.4.304.1/Bin/glslc.exe shaders/defaultShader.frag -o compiledShaders/frag.spv

.PHONY: test clean

test: compile
	./VulkanTest.out

clean:
	rm -f VulkanTest.out

debug:
	g++ $(CFLAGS) -g -o VulkanTest main.cpp src/*.cpp $(LDFLAGS)
	gdb VulkanTest