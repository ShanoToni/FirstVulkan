#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <stdexcept>
#include <vector>

#include "VulkanHelperFunctions.h"

//THE ORDER SHOULD BE FRONT BACK UP DOWN RIGHT LEFT

class SkyBox
{
public:
	SkyBox(std::vector<std::string> fNames);

	void createSkybox(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createSkyboxSampler(VkDevice device);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t heigth, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

	void cleanup(VkDevice device);

	//GET AND SET
	VkImageView& getSkyboxImageView() { return skyboxImageView; }
	VkSampler& getSkyboxSampler() { return skyboxSampler; }
	VkImage& getSkyboxImage() { return skyboxImage; }

	~SkyBox();

private:

	std::vector<std::string> fileNames;

	VkImage skyboxImage;
	VkDeviceMemory skyboxImageMemory;

	VkSampler skyboxSampler;

	VkImageView skyboxImageView;
};

