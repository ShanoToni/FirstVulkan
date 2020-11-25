#pragma once

#include "VkShaderTexture.h"
#include "ScreenQuadShader.h"
#include "OffscreenBuffer.h"
#include "VkShaderSkybox.h"
#include "VkBlinPhongShader.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class SceneBuilder
{
public:

	void setupScene();

	void cleanUpShaders(VkDevice device, std::vector<VkImage> SwapChainImages);
	void cleanUpMeshes(VkDevice device);

	void initShaderPipelines(float WIDTH, float HEIGHT, VkExtent2D SwapChainExtent, VkRenderPass renderPass, VkDevice device);

	void createDescriptorSetLayouts(VkDevice device);

	void createUniformBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice);

	void updateUniformBuffers(uint32_t currentImage, Camera& cam, VkExtent2D swapChainExtent, VkDevice device);

	void setLightingUBOBuffers(uint32_t currentImage, VkDevice device);

	void createDescriptorPool(VkDevice device, int swapChainSize);

	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device);

	void createVertexBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createIndexBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createTextures(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

	void createTextureImageView(VkDevice device);

	void createTextureImageSampler(VkDevice device);


	std::shared_ptr<VkShaderTexture>& getTexShader() { return shaderTex; }
	std::shared_ptr<VkShaderSkybox>& getSkyboxShader() { return skyboxShader; }
	std::shared_ptr<ScreenQuadShader>& getScreenQuadShader() { return shaderScreenQuad; }
	std::shared_ptr<VkBlinPhongShader>& getBlinnPhongShader() { return shaderBlinPhong; }
private:
	std::shared_ptr<VkShaderTexture> shaderTex;
	std::shared_ptr<VkShaderSkybox> skyboxShader;
	std::shared_ptr<ScreenQuadShader> shaderScreenQuad;
	std::shared_ptr<VkBlinPhongShader> shaderBlinPhong;

	std::shared_ptr<DirectionalLight> directionalLight;
	std::vector<std::shared_ptr<PointLight>> pointLightList;
	std::vector<std::shared_ptr<SpotLight>> spotLightList;
	std::vector<std::shared_ptr<Material>> materialList;
};

