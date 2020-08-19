#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>


struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

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

	//instance
	void createInstance();

	//debug
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtentions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	//device
	void pickPhysicalDevice();
	void createLogicalDevice();

	//Surface
	void createSurface();
	
	//queue fams
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	

	//Vulkan Checks
	void checkExtentionSupport();
	bool checkValidationLayerSupport();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	bool isPhysicalDeviceSuitable(VkPhysicalDevice device);

	//Cleanup
	void cleanup();

	//GLFW vars
	GLFWwindow* window;

	//Vulkan Vars
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
};

