#pragma once
#include "MeshBase.h"
class ScreenQuadMesh :
    public MeshBase
{
public:
	ScreenQuadMesh(std::vector<Vertex> verts);
	ScreenQuadMesh(std::string modelPath);

	// OVERRIDES
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device);

	// ADDITIONS
	inline void setScreenTexture(VkImageView imageView, VkSampler sampler) { screenQuadImageView = imageView; screenQuadSampler = sampler; }

protected:
	VkImageView screenQuadImageView;
	VkSampler screenQuadSampler;
};

