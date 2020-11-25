#pragma once
#include "MeshBase.h"

#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct DirectionalLightStruct
{
	alignas(16) glm::vec3 color;
	alignas(4) float ambientIntensity;
	alignas(4) float diffuseIntensity;
	alignas(16) glm::vec3 direction;
};

struct PointLightStruct
{
	alignas(16) glm::vec3 color;
	alignas(4) float ambientIntensity;
	alignas(4) float diffuseIntensity;
	alignas(16) glm::vec3 position;
	alignas(4) float constant;
	alignas(4) float linear;
	alignas(4) float exponent;
};

struct SpotLightStruct
{
	alignas(16) glm::vec3 color;
	alignas(4) float ambientIntensity;
	alignas(4) float diffuseIntensity;
	alignas(16) glm::vec3 position;
	alignas(4) float constant;
	alignas(4) float linear;
	alignas(4) float exponent;
	alignas(16) glm::vec3 direction;
	alignas(4) float edge;
};

struct MaterialStruct
{
	alignas(4) float specularIntensity;
	alignas(4) float shininess;
};

class BlinnPhongMesh :
    public MeshBase
{
public:
	BlinnPhongMesh(std::vector<Vertex> verts);
	BlinnPhongMesh(std::string modelPath);

	// OVERRIDES
	void createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device);

	// ADDITIONS
	void createLightingUBOBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice);
	void setLightingUBOBuffers(uint32_t currentImage, VkDevice device, std::shared_ptr<DirectionalLight> dirLight, std::vector< std::shared_ptr<PointLight>> pointLights, std::vector< std::shared_ptr<SpotLight>> spotLights, std::shared_ptr < Material> mat);
	void updateCampPosBuffer(uint32_t currentImage, Camera& cam, VkDevice device);


	void setTexture(std::shared_ptr<Texture> tex) { texture = tex; }
	inline std::shared_ptr<Texture> getTexture() { return texture; }


protected:
	std::shared_ptr<Texture> texture;

	DirectionalLightStruct directionalLightData;
	std::vector<VkBuffer> directionalLightBuffers;
	std::vector<VkDeviceMemory> directionalLightBuffersMemory;

	PointLightStruct pointLightData[PointLight::POINT_LIGHT_COUNT];
	std::vector<VkBuffer> pointLightBuffers;
	std::vector<VkDeviceMemory> pointLightBuffersMemory;

	SpotLightStruct spotLightData[SpotLight::SPOT_LIGHT_COUNT];
	std::vector<VkBuffer> spotLightBuffers;
	std::vector<VkDeviceMemory> spotLightBuffersMemory;

	MaterialStruct materialData;
	std::vector<VkBuffer> materialBuffer;
	std::vector<VkDeviceMemory> materialBufferMemory;

	std::vector<VkBuffer> camPosBuffer;
	std::vector<VkDeviceMemory> camPosBufferMemory;
};

