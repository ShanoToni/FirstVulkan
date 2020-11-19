#pragma once
#include "VkShaderBase.h"
#include "ScreenQuadMesh.h"
class ScreenQuadShader :
    public VkShaderBase
{
public:
    ScreenQuadShader(std::string vert, std::string frag, std::vector<std::shared_ptr<ScreenQuadMesh>> meshesToAdd);

    // OVERRIDES
    void initShaderPipeline(float WIDTH, float HEIGHT,
        VkExtent2D SwapChainExtent,
        VkRenderPass renderPass,
        VkDevice device);
    void createDescriptorSetLayout(VkDevice device);
    void createDescritorPool(VkDevice device, int swapChainSize);
    void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);


    void addMesh(std::shared_ptr<ScreenQuadMesh> meshToAdd);
    inline std::vector<std::shared_ptr<ScreenQuadMesh>> getMeshes() { return meshes; }

    // ADDITIONS

protected:

    std::vector<std::shared_ptr<ScreenQuadMesh>> meshes;

};

