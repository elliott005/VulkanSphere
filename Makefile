CFLAGS = -std=c++17# -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

compile: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp src/*.cpp $(LDFLAGS)

.PHONY: test clean

test: compile
	./VulkanTest

clean:
	rm -f VulkanTest

debug:
	g++ $(CFLAGS) -g -o VulkanTest main.cpp src/*.cpp $(LDFLAGS)
	gdb VulkanTest