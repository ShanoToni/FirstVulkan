#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


class Camera
{
public:
	Camera(GLFWwindow* window,
			glm::vec3 camPos = glm::vec3(0.0f, 5.0f, 5.0f),
			glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	void processInput(GLFWwindow* window);

	void update();

	~Camera();

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 up;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::mat4 view;
	glm::vec3 cameraFront;
	bool firstMouse;

	float yaw = 0.0f;
	float pitch = -90.0f;

	glm::vec3 direction;
	float lastX = 400, lastY = 300;
	float sensitivity = 0.1f;

public:
	glm::mat4& getView() { return view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp); }

	float& getYaw() { return yaw; }
	float& getPitch() { return pitch; }

	float getSensitivity() { return sensitivity; }

	float& getLastX() { return lastX; }
	float& getLastY() { return lastY; }

	glm::vec3& getCamFront() { return cameraFront; }
	glm::vec3& getCamPos() { return cameraPos; }
	glm::vec3& getCamUp() { return cameraUp; }
	glm::vec3 getCamDirection() { return glm::normalize(direction); }

	bool& getFirstMouse() { return firstMouse; }
};


