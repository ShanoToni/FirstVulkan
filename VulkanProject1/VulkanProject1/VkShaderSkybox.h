#pragma once
#include "VkShaderBase.h"
#include "MeshSkybox.h"

class VkShaderSkybox :
    public VkShaderBase
{
public:
    VkShaderSkybox(std::string vert, std::string frag, std::vector<std::shared_ptr<MeshSkybox>> meshesToAdd);

    // OVERRIDES
    void initShaderPipeline(float WIDTH, float HEIGHT,
        VkExtent2D SwapChainExtent,
        VkRenderPass renderPass,
        VkDevice device);
    void createDescriptorSetLayout(VkDevice device);
    void createDescritorPool(VkDevice device, int swapChainSize);
    void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);


    void addMesh(std::shared_ptr<MeshSkybox> meshToAdd);
    inline std::vector<std::shared_ptr<MeshSkybox>> getMeshes() { return meshes; }

    // ADDITIONS

protected:

    std::vector<std::shared_ptr<MeshSkybox>> meshes;

};

