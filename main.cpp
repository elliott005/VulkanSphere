//#include "src/Application.hpp"
#include "src/Player.hpp"
#include "src/Planet.hpp"

int main() {
    /* const std::vector<Vertex> vertices = {
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
    }; */

    Application app;
    app.startup("compiledShaders/vert.spv", "compiledShaders/frag.spv");

    float planetStart = static_cast<float>(glfwGetTime());
    Planet planet(glm::vec3(0.0f, 0.0, -10.0), 5.0f);
    float planetEnd = static_cast<float>(glfwGetTime());
    printf("planet startup time: %f\n", planetEnd - planetStart);

    std::vector<Image*> images;

    IMGUI_CHECKVERSION();
    app.init_imgui();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    
    try {
        /* Image img0;
        app.createTextureImage("assets/sprite_000.png", &img0);
        images.push_back(&img0); */
        VkBuffer vertexBuffer; VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer; VkDeviceMemory indexBufferMemory;
        app.createVertexBuffer(planet.vertices, vertexBuffer, vertexBufferMemory);
        app.createIndexBuffer(planet.indices, indexBuffer, indexBufferMemory);
        app.createUniformBuffers();
        app.initImages(images);
        
        Player player{glm::vec3(0.0f, 0.0f, 0.0f), app.swapChainExtent, app.window};
        
        //glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        float lastFrame = 0.0;
        
        bool toolWindowOpen = true;

        bool leftPressed = false;
        bool rightPressed = false;
        
        
        while (!glfwWindowShouldClose(app.window)) {
            if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(app.window, true);
		    }

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("planet tool", &toolWindowOpen, ImGuiWindowFlags_MenuBar);
                int v = planet.num_samples;
                if (glfwGetKey(app.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    if (!leftPressed)
                        v = std::max(0, v - 1);
                    leftPressed = true;
                } else {
                    leftPressed = false;
                }
                if (glfwGetKey(app.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    if (!rightPressed)
                        v = std::min(100, v + 1);
                    rightPressed = true;
                } else {
                    rightPressed = false;
                }
                ImGui::SliderInt("num samples", &v, 0, 100);
                if (v != planet.num_samples) {
                    planet.num_samples = v;
                    planet.icosahedron();
                    app.updateVertexBuffer(vertexBuffer, planet.vertices);
                    app.updateIndexBuffer(indexBuffer, planet.indices);
                }
            ImGui::End();
            ImGui::Render();
            
            
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //printf("FPS: %f\n", 1.0f / deltaTime);

            player.update(deltaTime, app.window);
            UniformBufferObject ubo = player.updateUBO();

            app.updateScreen(vertexBuffer, indexBuffer, planet.indices, ubo);
            glfwPollEvents();
        }

        
        vkDeviceWaitIdle(app.device);
        
        app.destroy_imgui();
        
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