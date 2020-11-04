#pragma once

#include <vector>
#include <fstream>

#include "Mesh.h"

class VulkanShader
{
public:
	VulkanShader(std::string vert, std::string frag);
	VulkanShader(std::string vert, std::string frag, std::vector<std::shared_ptr<Mesh>> meshesToAdd);

	void initShaderPipeline(float WIDTH, float HEIGHT,
							VkExtent2D SwapChainExtent,
							VkRenderPass renderPass,
							VkDevice device);



	std::vector<char> readFile(std::string filepath);

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
	
	void createDescriptorSetLayout(VkDevice device);
	void createDescritorPool(VkDevice device, int swapChainSize);
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);

	void addMesh(std::shared_ptr<Mesh> meshToAdd);

	inline std::vector<std::shared_ptr<Mesh>> getMeshes() { return meshes; }

	inline VkPipeline getPipeline() { return ShaderPipeline; }
	inline VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
	
	inline VkDescriptorPool& getDescriptorPool() { return descriptorPool; }
	inline VkDescriptorSetLayout& getDescriptorSetLayout() { return descriptorSetLayout; }

	~VulkanShader();

private:
	VkPipeline ShaderPipeline;
	VkPipelineLayout pipelineLayout;

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;

	std::string vertPath;
	std::string fragPath;
	std::vector<std::shared_ptr<Mesh>> meshes;
};

