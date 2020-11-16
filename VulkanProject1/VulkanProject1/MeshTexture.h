#pragma once
#include "MeshBase.h"



class MeshTexture :
    public MeshBase
{
public:
	MeshTexture(std::vector<Vertex> verts);
	MeshTexture(std::string modelPath);

	// OVERRIDES
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device);

	// ADDITIONS
	void setTexture(std::shared_ptr<Texture> tex) { texture = tex; }
	inline std::shared_ptr<Texture> getTexture() { return texture; }

protected:
	std::shared_ptr<Texture> texture;
};

