#pragma once
#include "MeshBase.h"
#include "SkyBox.h"

class MeshSkybox :
    public MeshBase
{
public:
	MeshSkybox(std::vector<Vertex> verts);
	MeshSkybox(std::string modelPath);

	// OVERRIDES
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device);

	// ADDITIONS
	void setSkybox(std::shared_ptr<SkyBox> tex) { skybox = tex; }
	inline std::shared_ptr<SkyBox> getSkybox() { return skybox; }

protected:
	std::shared_ptr<SkyBox> skybox;
};

