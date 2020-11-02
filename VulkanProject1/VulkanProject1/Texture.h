#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <stdexcept>


class Texture
{

public:
	Texture(std::string fName);

	void createTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
	void createImage(uint32_t widht, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice device, VkPhysicalDevice physicalDevice);

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);

	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);
	static VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
	static void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t heigth, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

	void cleanup(VkDevice device);

	~Texture();

private:

	std::string fileName;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
};

