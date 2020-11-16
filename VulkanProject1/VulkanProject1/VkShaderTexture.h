#pragma once
#include "VkShaderBase.h"
#include "MeshTexture.h"

class VkShaderTexture :
    public VkShaderBase
{
public:
    VkShaderTexture(std::string vert, std::string frag, std::vector<std::shared_ptr<MeshTexture>> meshesToAdd);

    // OVERRIDES
    void initShaderPipeline(float WIDTH, float HEIGHT,
        VkExtent2D SwapChainExtent,
        VkRenderPass renderPass,
        VkDevice device);
    void createDescriptorSetLayout(VkDevice device);
    void createDescritorPool(VkDevice device, int swapChainSize);
    void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);


    void addMesh(std::shared_ptr<MeshTexture> meshToAdd);
    inline std::vector<std::shared_ptr<MeshTexture>> getMeshes() { return meshes; }

    // ADDITIONS

protected:

    std::vector<std::shared_ptr<MeshTexture>> meshes;

};

