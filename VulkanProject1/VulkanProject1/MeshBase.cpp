#include "MeshBase.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

MeshBase::MeshBase()
{
	model = glm::mat4(1.0f);
}

MeshBase::MeshBase(std::vector<Vertex> meshVertices)
{
	for (auto Vertex : meshVertices)
	{
		vertices.push_back(Vertex);
	}
	model = glm::mat4(1.0f);
}

MeshBase::MeshBase(std::string filePath)
{
	tinyobj::attrib_t attribute;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &err, filePath.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			vertex.pos = {
				attribute.vertices[3 * index.vertex_index + 0],
				attribute.vertices[3 * index.vertex_index + 1],
				attribute.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attribute.texcoords[2 * index.texcoord_index + 0],
				1.0f - attribute.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.normal = {
				attribute.normals[3 * index.normal_index + 0],
				attribute.normals[3 * index.normal_index + 1],
				attribute.normals[3 * index.normal_index + 2]
			};
			vertex.color = glm::vec3(1.0f);

			vertices.push_back(vertex);
			indices.push_back(indices.size());

		}
	}
	model = glm::mat4(1.0f);

}

void MeshBase::translate(glm::vec3 translationVector)
{
	model = glm::translate(model, translationVector);
	ubo.model = model;
}

void MeshBase::rotate(glm::vec3 rotationVector)
{
	model = glm::rotate(model, glm::radians(90.f), rotationVector);
	ubo.model = model;
}

void MeshBase::scale(glm::vec3 scaleVector)
{
	model = glm::scale(model, scaleVector);
	ubo.model = model;
}

void MeshBase::createVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanHelperFunctions::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory, device, physicalDevice);

	//copy data to the GPU
	void* data;

	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);

	memcpy(data, vertices.data(), (size_t)bufferSize);

	vkUnmapMemory(device, stagingBufferMemory);

	VulkanHelperFunctions::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, device, physicalDevice);

	VulkanHelperFunctions::copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, device, graphicsQueue);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void MeshBase::createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanHelperFunctions::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	VulkanHelperFunctions::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, device, physicalDevice);

	VulkanHelperFunctions::copyBuffer(stagingBuffer, indexBuffer, bufferSize, commandPool, device, graphicsQueue);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void MeshBase::createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device)
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

		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr;
		descriptorWrites[0].pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void MeshBase::createUniformBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice)
{
	VkDeviceSize bufferSize = sizeof(BasicUBO);

	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VulkanHelperFunctions::createBuffer(bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i], device, physicalDevice);
	}
}

void MeshBase::updateUniformBuffer(uint32_t currentImage, Camera& cam, VkExtent2D swapChainExtent, VkDevice device)
{
	ubo.model = model;

	ubo.view = cam.getView();
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 999999.f);
	ubo.proj[1][1] *= -1;
	ubo.model = model;

	void* data;
	vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void MeshBase::setIndices(std::vector<uint16_t> indicesToSet)
{
	for (auto index : indicesToSet)
	{
		indices.push_back(index);
	}
}
