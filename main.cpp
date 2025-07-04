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
    ImFont* defaultFont = io.Fonts->AddFontFromFileTTF("assets/DroidSans.ttf");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    static const char* enumNoiseType[] = { "OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value" };
    static const char* enumFractalType[] = { "None", "FBm", "Ridged", "Ping Pong" };

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
        
        glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        float lastFrame = 0.0;
        
        bool leftPressed = false;
        bool rightPressed = false;
        
        
        while (!glfwWindowShouldClose(app.window)) {
            if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(app.window, true);
		    }
            
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //printf("FPS: %f\n", 1.0f / deltaTime);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::PushFont(defaultFont, 20.0f);
            ImGui::Begin("planet tool", nullptr, ImGuiWindowFlags_MenuBar);
                int num_samples = planet.num_samples;
                if (glfwGetKey(app.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    if (!leftPressed)
                        num_samples = std::max(0, num_samples - 1);
                    leftPressed = true;
                } else {
                    leftPressed = false;
                }
                if (glfwGetKey(app.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    if (!rightPressed)
                        num_samples = std::min(100, num_samples + 1);
                    rightPressed = true;
                } else {
                    rightPressed = false;
                }
                ImGui::SliderInt("num samples", &num_samples, 0, 100);
                float size = planet.size;
                ImGui::SliderFloat("size", &size, 1.0f, 20.0f);
                bool use_random_colors = planet.use_random_colors;
                ImGui::Checkbox("use random colors", &use_random_colors);
                glm::vec3 base_color = planet.base_color;
                ImGui::ColorEdit3("base color", &base_color.x);
                bool use_noise = planet.use_noise;
                ImGui::Checkbox("use noise", &use_noise);
                float noise_frequency = planet.noise_frequency;
                ImGui::SliderFloat("noise frequency", &noise_frequency, 0.01f, 2.0f);
                float noise_strength = planet.noise_strength;
                ImGui::SliderFloat("noise strength", &noise_strength, 0.0, 10.0);
                int noise_seed = planet.noise_seed;
                ImGui::DragInt("noise seed", &noise_seed);
                int noise_type_int = (int)planet.noise_type;
                ImGui::Combo("noise type", &noise_type_int, enumNoiseType, IM_ARRAYSIZE(enumNoiseType));
                FastNoiseLite::NoiseType noise_type = (FastNoiseLite::NoiseType)noise_type_int;
                int noise_fractal_type_int = (int)planet.noise_fractal_type;
                ImGui::Combo("noise fractal type", &noise_fractal_type_int, enumFractalType, IM_ARRAYSIZE(enumFractalType));
                FastNoiseLite::FractalType noise_fractal_type = (FastNoiseLite::FractalType)noise_fractal_type_int;
                if (noise_seed != planet.noise_seed || noise_fractal_type != planet.noise_fractal_type || noise_strength != planet.noise_strength || noise_type != planet.noise_type || base_color != planet.base_color || num_samples != planet.num_samples || use_noise != planet.use_noise || size != planet.size || noise_frequency != planet.noise_frequency || use_random_colors != planet.use_random_colors) {
                    planet.size = size;
                    planet.num_samples = num_samples;
                    planet.base_color = base_color;
                    planet.use_noise = use_noise;
                    planet.noise_seed = noise_seed;
                    planet.noise_strength = noise_strength;
                    planet.noise_type = noise_type;
                    planet.noise_fractal_type = noise_fractal_type;
                    planet.noise_frequency = noise_frequency;
                    planet.use_random_colors = use_random_colors;
                    planet.icosahedron();
                    app.updateVertexBuffer(vertexBuffer, planet.vertices);
                    app.updateIndexBuffer(indexBuffer, planet.indices);
                }
            ImGui::End();
            ImGui::Begin("stats", nullptr);
                ImGui::Text("FPS: %i", int(std::round(1.0f / deltaTime)));
                ImGui::Text("num triangles: %i", planet.num_triangles);
                ImGui::Text("num vertices: %i", planet.num_vertices);
                ImGui::Text("num unique points: %i", planet.num_unique_points);
            ImGui::End();
            ImGui::PopFont();
            ImGui::Render();
            
            

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