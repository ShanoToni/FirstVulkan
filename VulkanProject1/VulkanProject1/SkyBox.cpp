#include "SkyBox.h"

SkyBox::SkyBox(std::vector<std::string> fNames)
{
	
	for (auto fileName : fNames)
	{
		fileNames.push_back(fileName);
	}
}

void SkyBox::createSkybox(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	int texWidth, texHeight, texChannels;
	unsigned char* pixels[6];

	for (int i = 0; i< 6; i++)
	{
		pixels[i] = VulkanHelperFunctions::loadTextureData(fileNames[i], texWidth, texHeight, texChannels);

		if (!pixels[i])
		{
			throw std::runtime_error("Failed to load texture image!");
		}
	}

	VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth * texHeight * 4 * 6);
	VkDeviceSize layerSize = imageSize / 6;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanHelperFunctions::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

	void* data;

	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		for (int i = 0; i < 6; i++)
		{
			memcpy(static_cast<char*>(data) + (layerSize * i), pixels[i], static_cast<size_t>(layerSize));
		}
	vkUnmapMemory(device, stagingBufferMemory);


	VulkanHelperFunctions::createSkybox(texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		skyboxImage, skyboxImageMemory,
		device, physicalDevice);

	VulkanHelperFunctions::transitionImageLayout(skyboxImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandPool, device, graphicsQueue, 6);

	copyBufferToImage(stagingBuffer, skyboxImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), commandPool, device, graphicsQueue);

	VulkanHelperFunctions::transitionImageLayout(skyboxImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandPool, device, graphicsQueue, 6);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void SkyBox::createSkyboxSampler(VkDevice device)
{
	VkSamplerCreateInfo samplerInfo{};

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.f;

	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &skyboxSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void SkyBox::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t heigth, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue)
{
	VkCommandBuffer commandBuffer = VulkanHelperFunctions::beginSingleTimeCommands(commandPool, device);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 6;

	region.imageOffset = { 0,0,0 };
	region.imageExtent = { width, heigth, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	VulkanHelperFunctions::endSingleTimeCommands(commandBuffer, commandPool, device, graphicsQueue);
}

void SkyBox::cleanup(VkDevice device)
{
	vkDestroySampler(device, skyboxSampler, nullptr);
	vkDestroyImageView(device, skyboxImageView, nullptr);

	vkDestroyImage(device, skyboxImage, nullptr);
	vkFreeMemory(device, skyboxImageMemory, nullptr);
}

SkyBox::~SkyBox()
{
	
}
