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
							VkDescriptorSetLayout descriptorSetLayout,
							VkRenderPass renderPass,
							VkDevice device);



	std::vector<char> readFile(std::string filepath);

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);

	void addMesh(std::shared_ptr<Mesh> meshToAdd);

	inline std::vector<std::shared_ptr<Mesh>> getMeshes() { return meshes; }

	inline VkPipeline getPipeline() { return ShaderPipeline; }
	inline VkPipelineLayout getPipelineLayout() { return pipelineLayout; }

	


	~VulkanShader();

private:
	VkPipeline ShaderPipeline;
	VkPipelineLayout pipelineLayout;
	

	std::string vertPath;
	std::string fragPath;
	std::vector<std::shared_ptr<Mesh>> meshes;
};

