#pragma once
#include "MeshBase.h"

#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct DirectionalLightStruct
{
	float ambientIntensity;
	float diffuseIntensity;
	glm::vec4 color;
	glm::vec4 direction;
};

struct PointLightStruct
{
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
	glm::vec4 color;
	glm::vec4 position;
};

struct SpotLightStruct
{
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
	float edge;
	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;
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
	void setLightingUBOBuffers(uint32_t currentImage, VkDevice device, std::shared_ptr<DirectionalLight> dirLight, std::vector< std::shared_ptr<PointLight>> pointLights, std::vector< std::shared_ptr<SpotLight>> spotLights, std::shared_ptr < Material> mat, Camera& cam);
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

