#include "Player.hpp"

Player::Player(glm::vec<3, float, (glm::qualifier)0> pos, VkExtent2D swapChainExtent, GLFWwindow* window) {
    this->position = pos;

    this->worldUp = glm::vec3(0, 1, 0);

	this->projection = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);

    GLdouble xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
    lastMousePos.x = xPos;
    lastMousePos.y = yPos;
}

Player::~Player() {

}

void Player::update(float deltaTime, GLFWwindow* window) {
    this->handleMovement(window, deltaTime);
	this->handleCamera(window);
}

UniformBufferObject Player::updateUBO() {
    glm::mat4 view = glm::lookAt(position, position + front, up);
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = view;
    ubo.proj = this->projection;
    ubo.proj[1][1] *= -1;
    return ubo;
}

void Player::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	tempFront.y = sin(glm::radians(rotation.x));
	tempFront.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front = glm::normalize(tempFront);
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

void Player::handleCamera(GLFWwindow* window) {
	GLdouble xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	float xOffset = xPos - lastMousePos.x;
	float yOffset = lastMousePos.y - yPos;
	lastMousePos.x = xPos;
	lastMousePos.y = yPos;
	rotation.y += xOffset * mouseSensitivity;
	rotation.x += yOffset * mouseSensitivity;
	rotation.x = std::min(maxVerticalRotation, std::max(-maxVerticalRotation, rotation.x));
	updateCameraVectors();
}

void Player::handleMovement(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->position += glm::normalize(glm::vec3(front.x, front.y, front.z)) * this->speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->position += -glm::normalize(glm::vec3(front.x, front.y, front.z)) * this->speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->position += -glm::normalize(glm::vec3(right.x, right.y, right.z)) * this->speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->position += glm::normalize(glm::vec3(right.x, right.y, right.z)) * this->speed * deltaTime;
	}
}