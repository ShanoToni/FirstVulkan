#pragma once

#include <vector>
#include <fstream>

#include "Mesh.h"

class VulkanShaders
{
public:
	VulkanShaders(std::string vert, std::string frag);
	VulkanShaders(std::string vert, std::string frag, std::vector<Mesh> meshesToAdd);

	void initShaderPipeline(float WIDTH, float HEIGHT,
							VkExtent2D SwapChainExtent,
							VkDescriptorSetLayout descriptorSetLayout,
							VkRenderPass renderPass,
							VkDevice device);

	std::vector<char> readFile(std::string filepath);

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);

	void addMesh(Mesh meshToAdd);

	std::vector<Vertex> getVerticesFromMeshes();

	~VulkanShaders();

private:
	VkPipeline ShaderPipeline;
	VkPipelineLayout pipelineLayout;

	std::vector<char> vertShaderCode;
	std::vector<char> fragShaderCode;
	std::vector<Mesh> meshes;
};

