#include "SceneBuilder.h"

const std::string VShaderPath = "Shaders/TextureShaderVert.spv";
const std::string FShaderPath = "Shaders/TextureShaderFrag.spv";

const std::string OBJPATH = "OBJs/viking_room.obj";
const std::string OBJTEXPATH = "Textures/viking_room.png";

//--------------------------------------------- TESTING VALUES ----------------------------------------------------------------------------------
const std::vector<Vertex> vertices = {
	{{-40.5f, 0.f,40.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0,1,0}},
	{{40.5f, 0.f, 40.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0,1,0}},
	{{40.5f, 0.f, -40.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0,1,0}},
	{{-40.5f, 0.f, -40.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0,1,0}}
};

const std::vector<Vertex> vertices2 = {
	{{-40.5f, 5.f,40.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{40.5f, 5.f, 40.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{40.5f, 5.f, -40.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-40.5f, 5.f, -40.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

//--------------------------------------------- SCEEN QUAD VALUES ----------------------------------------------------------------------------------

const std::string ScreenVShaderPath = "Shaders/ScreenShaderVert.spv";
const std::string ScreenFShaderPath = "Shaders/ScreenShaderFrag.spv";

const std::vector<Vertex> screenQuadVertices = {
	{{-1, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{-1.0f, 1.f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{1.0f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> screenIndices = { 0, 1, 2, 1, 3, 2 };

//--------------------------------------------- SKYBOX VALUES ----------------------------------------------------------------------------------

std::string textureFile = "Textures/texture.jpg";


const std::string SkyboxVShaderPath = "Shaders/SkyboxShaderVert.spv";
const std::string SkyboxFShaderPath = "Shaders/SkyboxShaderFrag.spv";

const std::vector<Vertex> skyboxVerts = {
	{{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
};


const std::vector<uint16_t> skyboxIndices = { 0,1,2,	2,3,0,
											 4,1,0,		0,5,4,
											 2,6,7,		7,3,2,
											 4,5,7,		7,6,4,
											 0,3,7,		7,5,0,
											 1,4,2,		2,4,6 };

std::string skyboxFront = "Textures/sh_ft.png";
std::string skyboxBack = "Textures/sh_bk.png";
std::string skyboxUp = "Textures/sh_up.png";
std::string skyboxDown = "Textures/sh_dn.png";
std::string skyboxRight = "Textures/sh_rt.png";
std::string skyboxLeft = "Textures/sh_lf.png";

// BLINN PHONG VALUES

const std::string BlinnPhongVShaderPath = "Shaders/BlinnPhongShaderVert.spv";
const std::string BlinnPhongFShaderPath = "Shaders/BlinnPhongShaderFrag.spv";

void SceneBuilder::setupScene()
{
	// TEXTURED OBJECTS
	std::vector<std::shared_ptr<MeshTexture>> meshes;
	Texture tex = Texture(OBJTEXPATH);
	std::shared_ptr<MeshTexture> m(new MeshTexture(OBJPATH));
	m->setTexture(std::make_shared<Texture>(tex));
	m->rotate(glm::vec3(-1, 0, 0));
	meshes.push_back(m);


	std::shared_ptr <VkShaderTexture> shaderTemp(new VkShaderTexture(VShaderPath, FShaderPath, meshes));
	shaderTex = shaderTemp;

	// SCREEN QUAD
	std::vector<std::shared_ptr<ScreenQuadMesh>> screenQuad;
	std::shared_ptr<ScreenQuadMesh> quad(new ScreenQuadMesh(screenQuadVertices));
	quad->setIndices(screenIndices);
	screenQuad.push_back(quad);

	std::shared_ptr<ScreenQuadShader> screenShaderTemp(new ScreenQuadShader(ScreenVShaderPath, ScreenFShaderPath, screenQuad));
	shaderScreenQuad = screenShaderTemp;

	//SKYBOX
	std::vector<std::string> paths;
	paths.push_back(skyboxFront);
	paths.push_back(skyboxBack);
	paths.push_back(skyboxUp);
	paths.push_back(skyboxDown);
	paths.push_back(skyboxRight);
	paths.push_back(skyboxLeft);

	SkyBox skytex = SkyBox(paths);

	std::vector<std::shared_ptr<MeshSkybox>> skymeshes;
	std::shared_ptr<MeshSkybox> skymesh(new MeshSkybox(skyboxVerts));
	skymesh->setIndices(skyboxIndices);

	skymesh->setSkybox(std::make_shared<SkyBox>(skytex));
	skymesh->scale(glm::vec3(500.f));
	skymeshes.push_back(skymesh);

	std::shared_ptr<VkShaderSkybox> skyboxShadertemp(new VkShaderSkybox(SkyboxVShaderPath, SkyboxFShaderPath, skymeshes));
	skyboxShader = skyboxShadertemp;

	// LIGHTS
	//directional light
	std::shared_ptr<DirectionalLight> dLightTemp(new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 1.0f , glm::vec3(-1.0f, -1.0f,1.0f)));
	directionalLight = dLightTemp;
	//point lights
	std::shared_ptr<PointLight> pLightTemp(new PointLight(glm::vec3(5.0f), glm::vec3(1.0f), 0.2f, 0.1f, 0.1f, 0.2f, 0.9f));
	pointLightList.push_back(pLightTemp);

	if (pointLightList.size() != PointLight::POINT_LIGHT_COUNT)
	{
		throw std::runtime_error("Incomplatable number of point lights loaded");
	}
	//spot lights
	std::shared_ptr<SpotLight> sLightTemp(new SpotLight(glm::vec3(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0), 0.2f, 0.8f, 0.1f, 0.1f, 0.2f, 30.0f));
	spotLightList.push_back(sLightTemp);

	if (spotLightList.size() != SpotLight::SPOT_LIGHT_COUNT)
	{
		throw std::runtime_error("Incomplatable number of spot lights loaded");
	}
	//MATERIALS
	std::shared_ptr<Material> matTemp(new Material(0.6, 32.f));
	materialList.push_back(matTemp);


	// BlinnPHONG OBJECTS
	std::vector<std::shared_ptr<BlinnPhongMesh>> bpMeshes;
	std::shared_ptr<BlinnPhongMesh> bpMeshTemp(new BlinnPhongMesh(OBJPATH));
	bpMeshTemp->translate(glm::vec3(5.0f, 0.0f, 0.0f));
	bpMeshTemp->rotate(glm::vec3(-1, 0, 0));
	bpMeshTemp->setTexture(std::make_shared<Texture>(tex));
	bpMeshes.push_back(bpMeshTemp);

	std::shared_ptr<BlinnPhongMesh> bpMeshTemp2 = std::make_shared<BlinnPhongMesh>(BlinnPhongMesh(vertices));
	bpMeshTemp2->setTexture(std::make_shared<Texture>(tex));
	bpMeshTemp2->setIndices(indices);
	bpMeshes.push_back(bpMeshTemp2);

	std::shared_ptr <VkBlinPhongShader> bpShaderTemp(new VkBlinPhongShader(BlinnPhongVShaderPath, BlinnPhongFShaderPath, bpMeshes));
	shaderBlinPhong = bpShaderTemp;
}

void SceneBuilder::cleanUpShaders(VkDevice device, std::vector<VkImage> SwapChainImages)
{
	vkDestroyPipeline(device, shaderTex->getPipeline(), nullptr);
	vkDestroyPipelineLayout(device, shaderTex->getPipelineLayout(), nullptr);

	vkDestroyPipeline(device, shaderScreenQuad->getPipeline(), nullptr);
	vkDestroyPipelineLayout(device, shaderScreenQuad->getPipelineLayout(), nullptr);

	vkDestroyPipeline(device, skyboxShader->getPipeline(), nullptr);
	vkDestroyPipelineLayout(device, skyboxShader->getPipelineLayout(), nullptr);

	vkDestroyPipeline(device, shaderBlinPhong->getPipeline(), nullptr);
	vkDestroyPipelineLayout(device, shaderBlinPhong->getPipelineLayout(), nullptr);


	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		for (auto& mesh : shaderTex->getMeshes())
		{
			vkDestroyBuffer(device, mesh->getUniformBuffers()[i], nullptr);
			vkFreeMemory(device, mesh->getUniformBufferMemory()[i], nullptr);
		}
	}
	vkDestroyDescriptorPool(device, shaderTex->getDescriptorPool(), nullptr);

	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		for (auto& mesh : shaderScreenQuad->getMeshes())
		{
			vkDestroyBuffer(device, mesh->getUniformBuffers()[i], nullptr);
			vkFreeMemory(device, mesh->getUniformBufferMemory()[i], nullptr);
		}
	}
	vkDestroyDescriptorPool(device, shaderScreenQuad->getDescriptorPool(), nullptr);

	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		for (auto& mesh : skyboxShader->getMeshes())
		{
			vkDestroyBuffer(device, mesh->getUniformBuffers()[i], nullptr);
			vkFreeMemory(device, mesh->getUniformBufferMemory()[i], nullptr);
		}
	}
	vkDestroyDescriptorPool(device, skyboxShader->getDescriptorPool(), nullptr);

	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		for (auto& mesh : shaderBlinPhong->getMeshes())
		{
			vkDestroyBuffer(device, mesh->getUniformBuffers()[i], nullptr);
			vkFreeMemory(device, mesh->getUniformBufferMemory()[i], nullptr);
		}
	}
	vkDestroyDescriptorPool(device, shaderBlinPhong->getDescriptorPool(), nullptr);
}

void SceneBuilder::cleanUpMeshes(VkDevice device)
{
	vkDestroyDescriptorSetLayout(device, shaderTex->getDescriptorSetLayout(), nullptr);
	for (auto& mesh : shaderTex->getMeshes())
	{
		vkDestroyBuffer(device, mesh->getIndexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getIndexBufferMemory(), nullptr);

		vkDestroyBuffer(device, mesh->getVertexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getVertexBufferMemory(), nullptr);
	}

	vkDestroyDescriptorSetLayout(device, shaderScreenQuad->getDescriptorSetLayout(), nullptr);
	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		vkDestroyBuffer(device, mesh->getIndexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getIndexBufferMemory(), nullptr);

		vkDestroyBuffer(device, mesh->getVertexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getVertexBufferMemory(), nullptr);
	}

	vkDestroyDescriptorSetLayout(device, skyboxShader->getDescriptorSetLayout(), nullptr);
	for (auto& mesh : skyboxShader->getMeshes())
	{
		vkDestroyBuffer(device, mesh->getIndexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getIndexBufferMemory(), nullptr);

		vkDestroyBuffer(device, mesh->getVertexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getVertexBufferMemory(), nullptr);
	}

	vkDestroyDescriptorSetLayout(device, shaderBlinPhong->getDescriptorSetLayout(), nullptr);
	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		vkDestroyBuffer(device, mesh->getIndexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getIndexBufferMemory(), nullptr);

		vkDestroyBuffer(device, mesh->getVertexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getVertexBufferMemory(), nullptr);
	}
}

void SceneBuilder::initShaderPipelines(float WIDTH, float HEIGHT, VkExtent2D SwapChainExtent, VkRenderPass renderPass, VkDevice device)
{
	shaderTex->initShaderPipeline(WIDTH, HEIGHT, SwapChainExtent, renderPass, device);

	skyboxShader->initShaderPipeline(WIDTH, HEIGHT, SwapChainExtent, renderPass, device);

	shaderScreenQuad->initShaderPipeline(WIDTH, HEIGHT, SwapChainExtent, renderPass, device);

	shaderBlinPhong->initShaderPipeline(WIDTH, HEIGHT, SwapChainExtent, renderPass, device);

}

void SceneBuilder::createDescriptorSetLayouts(VkDevice device)
{
	shaderTex->createDescriptorSetLayout(device);

	skyboxShader->createDescriptorSetLayout(device);

	shaderScreenQuad->createDescriptorSetLayout(device);

	shaderBlinPhong->createDescriptorSetLayout(device);

}

void SceneBuilder::createUniformBuffers(std::vector<VkImage> swapChainImages, VkDevice device, VkPhysicalDevice physicalDevice)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}

	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}

	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
		mesh->createLightingUBOBuffers(swapChainImages, device, physicalDevice);
	}
}

void SceneBuilder::updateUniformBuffers(uint32_t currentImage, Camera& cam, VkExtent2D swapChainExtent, VkDevice device)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, cam, swapChainExtent, device);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, cam, swapChainExtent, device);
	}

	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, cam, swapChainExtent, device);
	}

	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, cam, swapChainExtent, device);
		mesh->setLightingUBOBuffers(currentImage, device, directionalLight, pointLightList, spotLightList, materialList[0]);
		mesh->updateCampPosBuffer(currentImage, cam, device);

	}
}

void SceneBuilder::setLightingUBOBuffers(uint32_t currentImage, VkDevice device)
{
	for (auto mesh : shaderBlinPhong->getMeshes())
	{
		mesh->setLightingUBOBuffers(currentImage, device, directionalLight, pointLightList, spotLightList, materialList[0]);
	}
}

void SceneBuilder::createDescriptorPool(VkDevice device, int swapChainSize)
{
	shaderTex->createDescritorPool(device, swapChainSize);
	skyboxShader->createDescritorPool(device, swapChainSize);
	shaderScreenQuad->createDescritorPool(device, swapChainSize);
	shaderBlinPhong->createDescritorPool(device, swapChainSize);

}

void SceneBuilder::createDescriptorSets(std::vector<VkImage> swapChainImages, VkDevice device)
{
	shaderTex->createDescriptorSets(swapChainImages, device);
	skyboxShader->createDescriptorSets(swapChainImages, device);
	shaderScreenQuad->createDescriptorSets(swapChainImages, device);
	shaderBlinPhong->createDescriptorSets(swapChainImages, device);
}

void SceneBuilder::createVertexBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
}

void SceneBuilder::createIndexBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}

}

void SceneBuilder::createTextures(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->getTexture()->createTexture(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->createSkybox(device, physicalDevice, commandPool, graphicsQueue);
	}
	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->getTexture()->createTexture(device, physicalDevice, commandPool, graphicsQueue);
	}
}

void SceneBuilder::createTextureImageView(VkDevice device)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->getTexture()->getTextureImageView() = VulkanHelperFunctions::createImageView(mesh->getTexture()->getTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->getSkyboxImageView() = VulkanHelperFunctions::createSkyboxView(mesh->getSkybox()->getSkyboxImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}

	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->getTexture()->getTextureImageView() = VulkanHelperFunctions::createImageView(mesh->getTexture()->getTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}
}

void SceneBuilder::createTextureImageSampler(VkDevice device)
{
	for (auto& mesh : shaderTex->getMeshes())
	{
		mesh->getTexture()->createTextureSampler(device);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->createSkyboxSampler(device);
	}

	for (auto& mesh : shaderBlinPhong->getMeshes())
	{
		mesh->getTexture()->createTextureSampler(device);
	}
}
