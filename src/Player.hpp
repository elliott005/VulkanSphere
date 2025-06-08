#pragma once

#include "Application.hpp"


class Player
{
public:
    Player(glm::vec<3, float, (glm::qualifier)0> pos, VkExtent2D swapChainExtent, GLFWwindow* window);
    ~Player();

    void update(float deltaTime, GLFWwindow* window);
    UniformBufferObject updateUBO();

    glm::vec3 position;
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};
private:
    void updateCameraVectors();
    void handleCamera(GLFWwindow* window);
	void handleMovement(GLFWwindow* window, float deltaTime);
   
    glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

    glm::vec2 lastMousePos;

    float speed = 7.0;
    
	glm::mat4 projection = glm::mat4(1.0f);

    float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
	float mouseSensitivity = 0.1;
	float maxVerticalRotation = 70.0;
};
