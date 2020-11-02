#include "Mesh.h"

Mesh::Mesh(std::vector<BasicVertex> verts)
{
	for (auto BasicVertex : verts)
	{
		vertices.push_back(BasicVertex);
	}
	model = glm::mat4(1.0f);
}

void Mesh::setIndices(std::vector<uint16_t> indicesToSet)
{
	for (auto index : indicesToSet)
	{
		indices.push_back(index);
	}
}

void Mesh::translate(glm::vec3 translationVector)
{
	model = glm::translate(model, translationVector);
	ubo.model = model;
}

void Mesh::rotate(glm::vec3 rotationVector)
{
	model = glm::rotate(model, glm::radians(90.f), rotationVector);
	ubo.model = model;
}

void Mesh::scale(glm::vec3 scaleVector)
{
	model = glm::scale(model, scaleVector);
	ubo.model = model;
}


void Mesh::createVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Texture::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory, device, physicalDevice);

	//copy data to the GPU
	void* data;

	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);

	memcpy(data, vertices.data(), (size_t)bufferSize);

	vkUnmapMemory(device, stagingBufferMemory);

	Texture::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, device, physicalDevice);

	Texture::copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, device, graphicsQueue);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Mesh::createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Texture::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	Texture::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, device, physicalDevice);

	Texture::copyBuffer(stagingBuffer, indexBuffer, bufferSize, commandPool, device, graphicsQueue);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Mesh::createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device)
{
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();



	if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(BasicUBO);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;

		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;

		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}
}

void Mesh::createUniformBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice)
{
	VkDeviceSize bufferSize = sizeof(BasicUBO);

	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		Texture::createBuffer(bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i], device, physicalDevice);
	}
}

void Mesh::updateUniformBufferModel(VkDevice device, uint32_t currentImage)
{
	ubo.model = model;

	void* data;
	vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void Mesh::updateUniformBufferViewProj(uint32_t currentImage, Camera& cam, VkExtent2D swapChainExtent, VkDevice device)
{
	ubo.view = cam.getView();
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 1000.f);
	ubo.proj[1][1] *= -1;
	ubo.model = model;

	void* data;
	vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

Mesh::~Mesh()
{
}
