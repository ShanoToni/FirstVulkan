#pragma once

#include <vector>
#include <fstream>

#include "MeshBase.h"

class VkShaderBase
{
public:
	VkShaderBase(std::string vert, std::string frag);
	VkShaderBase(std::string vert, std::string frag, std::vector< std::shared_ptr<MeshBase>> meshesToAdd);

	void initShaderPipeline(float WIDTH, float HEIGHT,
							VkExtent2D SwapChainExtent,
							VkRenderPass renderPass,
							VkDevice device);

	std::vector<char> readFile(std::string filepath);

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);

	void createDescriptorSetLayout(VkDevice device);
	void createDescritorPool(VkDevice device, int swapChainSize);
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);

	void addMesh(std::shared_ptr<MeshBase> meshToAdd);

	inline std::vector<std::shared_ptr<MeshBase>> getMeshes() { return meshes; }

	inline VkPipeline getPipeline() { return ShaderPipeline; }
	inline VkPipelineLayout getPipelineLayout() { return pipelineLayout; }

	inline VkDescriptorPool& getDescriptorPool() { return descriptorPool; }
	inline VkDescriptorSetLayout& getDescriptorSetLayout() { return descriptorSetLayout; }

	inline bool inUse() { return meshes.size() > 0 ? true : false; }

	~VkShaderBase();

protected:

	VkPipeline ShaderPipeline;
	VkPipelineLayout pipelineLayout;

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;

	std::string vertPath;
	std::string fragPath;
	std::vector<std::shared_ptr<MeshBase>> meshes;
};

