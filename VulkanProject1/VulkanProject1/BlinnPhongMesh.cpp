#include "BlinnPhongMesh.h"

BlinnPhongMesh::BlinnPhongMesh(std::vector<Vertex> verts) : MeshBase(verts)
{
	scale(glm::vec3(100, 100, 100));
	rotate(glm::vec3(-1, 0, 0));
}

BlinnPhongMesh::BlinnPhongMesh(std::string modelPath) : MeshBase(modelPath)
{
	scale(glm::vec3(1, 1, 1));
	rotate(glm::vec3(-1, 0, 0));
}

void BlinnPhongMesh::createDescriptorSets(std::vector<VkImage> swapChainImages, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool, VkDevice device)
{
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();


	if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		//UBO
		VkDescriptorBufferInfo uboBufferInfo{};
		uboBufferInfo.buffer = uniformBuffers[i];
		uboBufferInfo.offset = 0;
		uboBufferInfo.range = sizeof(BasicUBO);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture->getTextureImageView();
		imageInfo.sampler = texture->getTextureSampler();

		// Directiona Light
		VkDescriptorBufferInfo directionBufferInfo{};
		directionBufferInfo.buffer = directionalLightBuffers[i];
		directionBufferInfo.offset = 0;
		directionBufferInfo.range = sizeof(DirectionalLightStruct);

		// Point Light
		VkDescriptorBufferInfo pointBufferInfo{};
		pointBufferInfo.buffer = pointLightBuffers[i];
		pointBufferInfo.offset = 0;
		pointBufferInfo.range = sizeof(PointLightStruct);
		
		// Spot Light
		VkDescriptorBufferInfo spotBufferInfo{};
		spotBufferInfo.buffer = spotLightBuffers[i];
		spotBufferInfo.offset = 0;
		spotBufferInfo.range = sizeof(SpotLightStruct);

		// Material	
		VkDescriptorBufferInfo materialBufferInfo{};
		materialBufferInfo.buffer = materialBuffer[i];
		materialBufferInfo.offset = 0;
		materialBufferInfo.range = sizeof(MaterialStruct);
		

		std::array<VkWriteDescriptorSet, 6> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &uboBufferInfo;
		descriptorWrites[0].pImageInfo = nullptr;
		descriptorWrites[0].pTexelBufferView = nullptr;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSet;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &uboBufferInfo;
		descriptorWrites[1].pImageInfo = &imageInfo;
		descriptorWrites[1].pTexelBufferView = nullptr;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSet;
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pBufferInfo = &directionBufferInfo;
		descriptorWrites[2].pImageInfo = nullptr;
		descriptorWrites[2].pTexelBufferView = nullptr;

		descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[3].dstSet = descriptorSet;
		descriptorWrites[3].dstBinding = 3;
		descriptorWrites[3].dstArrayElement = 0;
		descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[3].descriptorCount = PointLight::POINT_LIGHT_COUNT;
		descriptorWrites[3].pBufferInfo = &pointBufferInfo;
		descriptorWrites[3].pImageInfo = nullptr;
		descriptorWrites[3].pTexelBufferView = nullptr;

		descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[4].dstSet = descriptorSet;
		descriptorWrites[4].dstBinding = 4;
		descriptorWrites[4].dstArrayElement = 0;
		descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[4].descriptorCount = SpotLight::SPOT_LIGHT_COUNT;
		descriptorWrites[4].pBufferInfo = &spotBufferInfo;
		descriptorWrites[4].pImageInfo = nullptr;
		descriptorWrites[4].pTexelBufferView = nullptr;

		descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[5].dstSet = descriptorSet;
		descriptorWrites[5].dstBinding = 5;
		descriptorWrites[5].dstArrayElement = 0;
		descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[5].descriptorCount = 1;
		descriptorWrites[5].pBufferInfo = &materialBufferInfo;
		descriptorWrites[5].pImageInfo = nullptr;
		descriptorWrites[5].pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void BlinnPhongMesh::createLightingUBOBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice)
{
	// Create direction buffer
	VkDeviceSize directionalBufferSize = sizeof(DirectionalLightStruct);

	directionalLightBuffers.resize(swapChainImages.size());
	directionalLightBuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VulkanHelperFunctions::createBuffer(directionalBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			directionalLightBuffers[i], directionalLightBuffersMemory[i], device, physicalDevice);
	}

	// Create point buffer
	VkDeviceSize pointBufferSize = sizeof(PointLightStruct);

	pointLightBuffers.resize(swapChainImages.size());
	pointLightBuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VulkanHelperFunctions::createBuffer(pointBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			pointLightBuffers[i], pointLightBuffersMemory[i], device, physicalDevice);
	}

	// Create spot buffer
	VkDeviceSize spotBufferSize = sizeof(SpotLightStruct);

	spotLightBuffers.resize(swapChainImages.size());
	spotLightBuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VulkanHelperFunctions::createBuffer(spotBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			spotLightBuffers[i], spotLightBuffersMemory[i], device, physicalDevice);
	}
}

void BlinnPhongMesh::setLightingUBOBuffers(uint32_t currentImage, VkDevice device, DirectionalLight dirLight, std::vector<PointLight> pointLights, std::vector<SpotLight> spotLights, Material mat)
{
	directionalLightData.color = dirLight.getColor();
	directionalLightData.ambientIntensity = dirLight.getAmbientIntensity();
	directionalLightData.diffuseIntensity = dirLight.getDiffuseIntensity();
	directionalLightData.direction = dirLight.getDirection();

	if (PointLight::POINT_LIGHT_COUNT != pointLights.size())
	{
		throw std::runtime_error("Point light count does not match!");
	}

	for (int i = 0; i < PointLight::POINT_LIGHT_COUNT; i++)
	{
		pointLightData[i].color = pointLights[i].getColor();
		pointLightData[i].position = pointLights[i].getPosition();

		pointLightData[i].ambientIntensity = pointLights[i].getAmbientIntensity();
		pointLightData[i].diffuseIntensity = pointLights[i].getDiffuseIntensity();

		pointLightData[i].constant = pointLights[i].getConstant();
		pointLightData[i].linear = pointLights[i].getLinear();
		pointLightData[i].exponent = pointLights[i].getExponent();
	}

	if (SpotLight::SPOT_LIGHT_COUNT != spotLights.size())
	{
		throw std::runtime_error("Spot light count does not match!");
	}

	for (int i = 0; i < SpotLight::SPOT_LIGHT_COUNT; i++)
	{
		spotLightData[i].base.color = spotLights[i].getColor();
		spotLightData[i].base.position = spotLights[i].getPosition();

		spotLightData[i].base.ambientIntensity = spotLights[i].getAmbientIntensity();
		spotLightData[i].base.diffuseIntensity = spotLights[i].getDiffuseIntensity();

		spotLightData[i].base.constant = spotLights[i].getConstant();
		spotLightData[i].base.linear = spotLights[i].getLinear();
		spotLightData[i].base.exponent = spotLights[i].getExponent();

		spotLightData[i].direction = spotLights[i].getDirection();
		spotLightData[i].edge = spotLights[i].getEdge();
	}

	materialData.shininess = mat.getShininess();
	materialData.specularIntensity = mat.getSpecularIntensity();

	//send directional light data
	void* directionalData;
	vkMapMemory(device, directionalLightBuffersMemory[currentImage], 0, sizeof(directionalLightData), 0, &directionalData);
		memcpy(directionalData, &directionalLightData, sizeof(directionalLightData));
	vkUnmapMemory(device, directionalLightBuffersMemory[currentImage]);

	//send point light data
	void* pointData;
	VkDeviceSize pointLightArraySize = sizeof(pointLightBuffers);
	VkDeviceSize pointLightSize = sizeof(pointLightBuffers)/ PointLight::POINT_LIGHT_COUNT;

	vkMapMemory(device, pointLightBuffersMemory[currentImage], 0, pointLightArraySize, 0, &pointData);
		for (int i = 0; i < PointLight::POINT_LIGHT_COUNT; i++) 
		{
			memcpy((static_cast<char*>(pointData) + pointLightSize * i), &pointLightData, pointLightSize);
		}
	vkUnmapMemory(device, pointLightBuffersMemory[currentImage]);

	//send spot light data
	void* spotData;
	VkDeviceSize spotLightArraySize = sizeof(spotLightBuffers);
	VkDeviceSize spotLightSize = sizeof(spotLightBuffers) / PointLight::POINT_LIGHT_COUNT;

	vkMapMemory(device, spotLightBuffersMemory[currentImage], 0, spotLightArraySize, 0, &spotData);
		for (int i = 0; i < SpotLight::SPOT_LIGHT_COUNT; i++)
		{
			memcpy((static_cast<char*>(spotData) + spotLightSize * i), &spotLightData, spotLightSize);
		}
	vkUnmapMemory(device, spotLightBuffersMemory[currentImage]);

	//send directional light data
	void* matlData;
	vkMapMemory(device, materialBufferMemory[currentImage], 0, sizeof(materialData), 0, &matlData);
		memcpy(matlData, &materialData, sizeof(materialData));
	vkUnmapMemory(device, materialBufferMemory[currentImage]);
}
