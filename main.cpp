//#include "src/Application.hpp"
#include "src/Player.hpp"

int main() {
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 7, 6, 6, 5, 4,
    };

    Application app;

    std::vector<Image*> images;
    
    try {
        app.startup("shaders/defaultShader.vert", "shaders/defaultShader.frag");
        /* Image img0;
        app.createTextureImage("assets/sprite_000.png", &img0);
        images.push_back(&img0); */
        VkBuffer vertexBuffer; VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer; VkDeviceMemory indexBufferMemory;
        app.createVertexBuffer(vertices, vertexBuffer, vertexBufferMemory);
        app.createIndexBuffer(indices, indexBuffer, indexBufferMemory);
        app.createUniformBuffers();
        app.initImages(images);
        
        Player player{glm::vec3(0.0f, 0.0f, 0.0f), app.swapChainExtent, app.window};
        
        glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        float lastFrame = 0.0;

        while (!glfwWindowShouldClose(app.window)) {
            if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(app.window, true);
		    }
            
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            player.update(deltaTime, app.window);
            UniformBufferObject ubo = player.updateUBO();

            app.updateScreen(vertexBuffer, indexBuffer, indices, ubo);
            glfwPollEvents();
        }
        
        vkDeviceWaitIdle(app.device);
        
        for (Image* image : images) {
            app.destroyImage(image);
        }
        app.destroyBuffers(vertexBuffer, vertexBufferMemory, indexBuffer, indexBufferMemory);
        app.close();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}