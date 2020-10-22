#pragma once

#include "Camera.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>
#include <fstream>
#include <array>
#include <chrono>
#include <map>
#include "VulkanShaders.h"

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

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanRenderer
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

	void recreateSwapChain();

	void cleanupSwapChain();

	// Image Views
	void createImageViews();

	//Graphics pipeline
	void createRenderPass();

	void createGraphicsPipelines();

	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createFramebuffers();

	//Drawing Commands
	void createCommandPool();

	void createCommandBuffers();

	//Buffer
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	//Uniform Buffers
	void createDescriptorSetLayout();

	void createUniformBuffers();
	
	void updateUniformBuffer(uint32_t currentImage);

	void createDescritorPool();
	void createDescriptorSet();

	//VERTEX
	void createVertexBuffer();

	void createIndexBuffer();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

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

	//CAMERA
	std::unique_ptr<Camera> camera;

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

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<std::shared_ptr<VulkanShaders>> shaders;
	
public:
	bool framebufferResized = false;
};

