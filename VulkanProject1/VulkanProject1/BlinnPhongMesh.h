#pragma once
#include "MeshBase.h"

#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct DirectionalLightStruct
{
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	glm::vec3 direction;
};

struct PointLightStruct
{
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	glm::vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLightStruct
{
	PointLightStruct base;
	glm::vec3 direction;
	float edge;
};

struct MaterialStruct
{
	float specularIntensity;
	float shininess;
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
	void setLightingUBOBuffers(uint32_t currentImage, VkDevice device, DirectionalLight dirLight, std::vector<PointLight> pointLights, std::vector<SpotLight> spotLights, Material mat);

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
};

