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
#include <fstream>


struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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

	// Swapchain
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createSwapChain();

	// Image Views
	void createImageViews();

	//Graphics pipeline
	void createRenderPass();

	void createGraphicsPipeline();

	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createFramebuffers();

	//Drawing Commands

	void createCommandPool();
	void createCommandBuffers();

	//DRAWING
	void createSyncObjects();

	void drawFrame();

	//Vulkan Checks
	void checkExtentionSupport();
	bool checkValidationLayerSupport();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	bool isPhysicalDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

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

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkCommandPool commandPool;

	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
};

