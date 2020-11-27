#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <stdexcept>
#include <array>

#include "VulkanHelperFunctions.h"
#include "ScreenQuadMesh.h"

class OffscreenBuffer
{
public:
	OffscreenBuffer();

	void createOffscreen(VkFormat depthFormat, VkExtent2D swapChainExtent, VkDevice device, VkPhysicalDevice physicalDevice, VkFormat swapChainImageFormat, ScreenQuadMesh& mesh);

	//gets
	inline VkFramebuffer getOffscreenFramebuffer() { return offFrameBuffer; }
	inline VkRenderPass getOffscreenRenderPass() { return offRenderPass; }
	inline VkImageView getOffscreenColorImageView() { return offImageColorView; }
	inline VkSampler getOffscreenSampler() { return offSampler; }

private:
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

};

