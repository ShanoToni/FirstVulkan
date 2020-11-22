#pragma once
#include "VkShaderBase.h"
#include "BlinnPhongMesh.h"

class VkBlinPhongShader :
    public VkShaderBase
{
public:
    VkBlinPhongShader(std::string vert, std::string frag, std::vector<std::shared_ptr<BlinnPhongMesh>> meshesToAdd);

    // OVERRIDES
    void initShaderPipeline(float WIDTH, float HEIGHT,
        VkExtent2D SwapChainExtent,
        VkRenderPass renderPass,
        VkDevice device);
    void createDescriptorSetLayout(VkDevice device);
    void createDescritorPool(VkDevice device, int swapChainSize);
    void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);


    void addMesh(std::shared_ptr<BlinnPhongMesh> meshToAdd);
    inline std::vector<std::shared_ptr<BlinnPhongMesh>> getMeshes() { return meshes; }

    // ADDITIONS

protected:
    std::vector<std::shared_ptr<BlinnPhongMesh>> meshes;

};

