#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <stdexcept>
#include "VulkanHelperFunctions.h"

class Texture
{

public:
	Texture(std::string fName);

	void createTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createTextureSampler(VkDevice device);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t heigth, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

	void cleanup(VkDevice device);

	//GET AND SET
	VkImageView& getTextureImageView() { return textureImageView; }
	VkSampler& getTextureSampler() {return textureSampler;}
	VkImage& getTextureImage() { return textureImage; }

	~Texture();

private:

	std::string fileName;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkSampler textureSampler;

	VkImageView textureImageView;
};

