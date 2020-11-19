#pragma once

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

//#include "VulkanShader.h"
//#include "VkShaderBase.h"

#include "VkShaderTexture.h"
#include "ScreenQuadShader.h"

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


class VulkanRenderer
{
public:
	void run();

private:
	//Initialization
	void initWindow();

	void initVulkan();

	void setupScene();

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

	void createFramebuffers();

	//Drawing Commands
	void createCommandPool();

	void createCommandBuffers();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	//Uniform Buffers
	void createDescriptorSetLayout();

	void createUniformBuffers();
	
	void updateUniformBuffer(uint32_t currentImage);

	void createDescritorPool();
	void createDescriptorSet();

	//BasicVertex
	void createVertexBuffer();

	void createIndexBuffer();

	//TEXTURE
	void createTextureImage();

	void createTextureImageView();

	void createTextureSampler();

	//DEPTH DATA
	void createDepthResource();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponet(VkFormat format);

	//DRAWING
	void createSyncObjects();

	void drawFrame();

	// OFFSCREEN DRAWING
	void createOffscreen();

	//OFFSCREEN DRAWING VARS
	VkFramebuffer offFrameBuffer;

	VkImage offImageColor;
	VkDeviceMemory offImageColorMemory;
	VkImageView offImageColorView;

	VkImage offImageDepth;
	VkDeviceMemory offImageDepthMemory;
	VkImageView offImageDepthView;

	VkRenderPass offRenderPass;
	VkSampler offSampler;
	VkDescriptorImageInfo offDescriptor;

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

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkCommandPool commandPool;

	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	VkDescriptorSetLayout descriptorSetLayout;

	std::shared_ptr<VkShaderTexture> shader;
	std::shared_ptr<ScreenQuadShader> shaderScreenQuad;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	
public:
	bool framebufferResized = false;
};

