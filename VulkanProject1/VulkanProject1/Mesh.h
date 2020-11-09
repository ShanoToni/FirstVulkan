#pragma once

#include <vector>
#include <array>

#include "Camera.h"
#include "Texture.h"



struct BasicUBO {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct BasicVertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(BasicVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(BasicVertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(BasicVertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(BasicVertex, texCoord);

		return attributeDescriptions;
	}
};

class Mesh
{
public:
	Mesh(std::vector<BasicVertex> verts);
	Mesh(std::string modelPath);

	void setIndices(std::vector<uint16_t> indicesToSet);
	inline std::vector<uint16_t> getIndices() { return indices; }

	inline std::vector<BasicVertex> getVertices() { return vertices; }

	inline glm::mat4 getModel() { return model; }

	void translate(glm::vec3 translationVector);
	void rotate(glm::vec3 rotationVector);
	void scale(glm::vec3 scaleVector);

	//static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

	void createVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device);

	void createUniformBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice);

	void updateUniformBufferModel( VkDevice device, uint32_t currentImage);

	void updateUniformBufferViewProj(uint32_t currentImage, Camera& cam, VkExtent2D swapChainExtent, VkDevice device);

	inline VkDescriptorSet& getDescriptorSets() { return descriptorSet; }
	inline std::vector<VkBuffer>& getUniformBuffers() { return uniformBuffers; }
	inline std::vector<VkDeviceMemory>& getUniformBufferMemory() { return uniformBuffersMemory; }

	inline VkBuffer& getVertexBuffer() { return vertexBuffer; }
	inline VkDeviceMemory& getVertexBufferMemory() { return vertexBufferMemory; }
	inline VkBuffer& getIndexBuffer() { return indexBuffer; }
	inline VkDeviceMemory& getIndexBufferMemory() { return indexBufferMemory; }

	void setTexture(std::shared_ptr<Texture> tex) { texture = tex; }
	inline std::shared_ptr<Texture> getTexture() { return texture; }

	~Mesh();
private:
	std::vector<BasicVertex> vertices;
	std::vector<uint16_t> indices;

	std::shared_ptr<Texture> texture;

	VkDescriptorSet descriptorSet;

	BasicUBO ubo;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	glm::mat4 model;
};

