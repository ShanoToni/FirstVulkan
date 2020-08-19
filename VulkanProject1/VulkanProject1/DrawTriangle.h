#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

class DrawTriangle
{
public:
	void run();

private:
	//Initialization
	void initWindow();

	void initVulkan();

	//Main Functions
	void mainLoop();

	//VULKAN
	void createInstance();
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtentions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	//Vulkan Checks
	void checkExtentionSupport();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	//Cleanup
	void cleanup();

	//GLFW vars
	GLFWwindow* window;

	//Vulkan Vars
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;


};

