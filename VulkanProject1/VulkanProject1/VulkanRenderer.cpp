#include "VulkanRenderer.h"


// ------------------------------------------------ GLOBAL VARS ---------------------------------------------------------------
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string VShaderPath = "Shaders/TextureShaderVert.spv";
const std::string FShaderPath = "Shaders/TextureShaderFrag.spv";

const std::string OBJPATH = "OBJs/viking_room.obj";
const std::string OBJTEXPATH = "Textures/viking_room.png";

const int MAX_FRAMES_IN_FLIGHT = 2;


//--------------------------------------------- TESTING VALUES ----------------------------------------------------------------------------------
const std::vector<Vertex> vertices = {
	{{-40.5f, 0.f,40.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{40.5f, 0.f, 40.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{40.5f, 0.f, -40.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-40.5f, 0.f, -40.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<Vertex> vertices2 = {
	{{-40.5f, 5.f,40.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{40.5f, 5.f, 40.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{40.5f, 5.f, -40.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-40.5f, 5.f, -40.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {	0, 1, 2, 2, 3, 0 };


const std::string ScreenVShaderPath = "Shaders/ScreenShaderVert.spv";
const std::string ScreenFShaderPath = "Shaders/ScreenShaderFrag.spv";

const std::vector<Vertex> screenQuadVertices = {
	{{-1, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{-1.0f, 1.f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{1.0f, 1.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> screenIndices = { 0, 1, 2, 1, 3, 2 };

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


const std::vector<uint16_t> skyboxIndices = {0,1,2,		2,3,0,
											 4,1,0,		0,5,4,
											 2,6,7,		7,3,2,
											 4,5,7,		7,6,4,
											 0,3,7,		7,5,0,
											 1,4,2,		2,4,6};

std::string skyboxFront = "Textures/sh_ft.png";
std::string skyboxBack = "Textures/sh_bk.png";
std::string skyboxUp = "Textures/sh_up.png";
std::string skyboxDown = "Textures/sh_dn.png";
std::string skyboxRight = "Textures/sh_rt.png";
std::string skyboxLeft = "Textures/sh_lf.png";

//--------------------------------------------ADDITIONAL PROXY FUNCTION FOR VALIDATION LAYERS-------------------------------------------------

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
										const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
										const VkAllocationCallbacks* pAllocator,
										VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
									VkDebugUtilsMessengerEXT debugMessenger,
									const VkAllocationCallbacks* pAllocator) 
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

//--------------------------------------------STATIC FUNCTIONS-------------------------------------------------

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));

	if (cam->getFirstMouse())
	{
		cam->getLastX() = xpos;
		cam->getLastY() = ypos;
		cam->getFirstMouse() = false;
	}

	float xoffset = xpos - cam->getLastX();
	float yoffset = cam->getLastY() - ypos;
	cam->getLastX() = xpos;
	cam->getLastY() = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cam->getYaw() += xoffset;
	cam->getPitch() += yoffset;

	if (cam->getPitch() > 89.0f)
		cam->getPitch() = 89.0f;
	if (cam->getPitch() < -89.0f)
		cam->getPitch() = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
	direction.y = sin(glm::radians(cam->getPitch()));
	direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
	cam->getCamFront() = glm::normalize(direction);

	cam->update();
}


// ---------------------------------------------------------MEMBER FUNCTIONS --------------------------------------------------------------------------


// --------------------------------------------------------- INITILIZATION  -------------------------------------------------------------------------------
void VulkanRenderer::run()
{
	try
	{
		initWindow();
		initVulkan();
		std::unique_ptr<Camera> cam(new Camera(window));
		camera = std::move(cam);
		glfwSetCursorPosCallback(window, mouse_callback);
		//checkExtentionSupport();
		//checkValidationLayerSupport();
		mainLoop();
		cleanup();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

void VulkanRenderer::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void VulkanRenderer::initVulkan()
{
	setupScene();
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createOffscreen();
	createDescriptorSetLayout();
	createGraphicsPipelines();
	createCommandPool();
	createDepthResource();
	createFramebuffers();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescritorPool();
	createDescriptorSet();
	createCommandBuffers();
	createSyncObjects();
}

void VulkanRenderer::setupScene()
{

	// TEXTURED OBJECTS
	std::vector<std::shared_ptr<MeshTexture>> meshes;
	Texture tex = Texture(OBJTEXPATH);
	std::shared_ptr<MeshTexture> m(new MeshTexture(OBJPATH));
	m->setTexture(std::make_shared<Texture>(tex));
	meshes.push_back(m);


	std::shared_ptr <VkShaderTexture> shaderTemp(new VkShaderTexture(VShaderPath, FShaderPath, meshes));
	shader = shaderTemp;

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
}

void VulkanRenderer::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		camera->processInput(window);
		drawFrame();
	}

	vkDeviceWaitIdle(device);
}

// --------------------------------------------------------- VULKAN ------------------------------------------------------------------------------------------

// --------------------------------------------------------- Instance

void VulkanRenderer::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	//App create info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	//Vulkan Instance create info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Extentions
	auto extensions = getRequiredExtentions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//Validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	//check if it worked
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}

}

// --------------------------------------------------------- VALIDATION

void VulkanRenderer::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

std::vector<const char*> VulkanRenderer::getRequiredExtentions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
}

// -------------------------------------------------------- DEVICES

void VulkanRenderer::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, & deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isPhysicalDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find suitable GPU");
	}
}

void VulkanRenderer::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.f;

	for(uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	//TODO: Query support for features 
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &presentQueue);
}

// -------------------------------------------------------- SURFACE

void VulkanRenderer::createSurface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}
}

// -------------------------------------------------------- QUEUES

QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


	int i = 0;

	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

// -------------------------------------------------------- SWAPCHAIN

SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	//check surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	//Supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// Device Presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
			&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

void VulkanRenderer::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void VulkanRenderer::recreateSwapChain()
{

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipelines();
	createFramebuffers();
	createUniformBuffers();
	createDescritorPool();
	createDescriptorSet();
	createCommandBuffers();
}

void VulkanRenderer::cleanupSwapChain()
{
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
	}

	vkDestroyFramebuffer(device, offScreen.getOffscreenFramebuffer(), nullptr);
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	
	
	vkDestroyPipeline(device, shader->getPipeline(), nullptr);
	vkDestroyPipelineLayout(device, shader->getPipelineLayout(), nullptr);
	
	vkDestroyRenderPass(device, offScreen.getOffscreenRenderPass(), nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		vkDestroyImageView(device, swapChainImageViews[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		for (auto& mesh : shader->getMeshes())
		{
			vkDestroyBuffer(device, mesh->getUniformBuffers()[i], nullptr);
			vkFreeMemory(device, mesh->getUniformBufferMemory()[i], nullptr);
		}
	}
	
	vkDestroyDescriptorPool(device, shader->getDescriptorPool(), nullptr);

	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void VulkanRenderer::createImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		swapChainImageViews[i] = VulkanHelperFunctions::createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}
}

// --------------------------------------------------------- GRAPHICS PIPELINE ------------------------------------------------------------------------------------------

void VulkanRenderer::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create render pass!");
	}
}

//------------------------------------------------------ !!! ADD NEW SHADER HERE !!! -------------------------------------------------------------------------------------------------------------------------

void VulkanRenderer::createGraphicsPipelines()
{
	
	shader->initShaderPipeline(WIDTH,HEIGHT, swapChainExtent, renderPass, device);

	shaderScreenQuad->initShaderPipeline(WIDTH, HEIGHT, swapChainExtent, renderPass, device);

	skyboxShader->initShaderPipeline(WIDTH, HEIGHT, swapChainExtent, renderPass, device);
}


void VulkanRenderer::createFramebuffers()
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++)
	{

		std::array<VkImageView, 2> attachments = {
			swapChainImageViews[i],
			depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}

// --------------------------------------------------------- DRAWING COMMANDS ------------------------------------------------------------------------------------------

void VulkanRenderer::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0;

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command pool!");
	}
}

void VulkanRenderer::createCommandBuffers()
{
	commandBuffers.resize(swapChainFramebuffers.size());

	

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording to command buffer!");
		}


		// ------------------------------------------------------------ OFFSCREEN RENDER FIRST-----------------------------------------------

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = offScreen.getOffscreenRenderPass() ;
		renderPassInfo.framebuffer = offScreen.getOffscreenFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


		//-------------------------------------------------- SKYBOX SHADER ---------------------------------------------------
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxShader->getPipeline());
		for (auto& mesh : skyboxShader->getMeshes())
		{
			VkBuffer vertexBuffers[] = { mesh->getVertexBuffer() };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffers[i], mesh->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxShader->getPipelineLayout(), 0, 1, &mesh->getDescriptorSets(), 0, nullptr);

			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(mesh->getIndices().size()), 1, 0, 0, 0);
		}


		//--------------------------------------------------Texture SHADER ---------------------------------------------------
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipeline());
		for (auto& mesh : shader->getMeshes())
		{
			VkBuffer vertexBuffers[] = { mesh->getVertexBuffer() };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffers[i], mesh->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->getPipelineLayout(), 0, 1, &mesh->getDescriptorSets(), 0, nullptr);

			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(mesh->getIndices().size()), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(commandBuffers[i]);


		//-------------------------------------------------- SCREEN QUAD  ---------------------------------------------------

		//VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		//std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
		shaderScreenQuad->getMeshes()[0]->setScreenTexture(offScreen.getOffscreenColorImageView(), offScreen.getOffscreenSampler());
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//--------------------------------------------------SCREEN SHADER ---------------------------------------------------

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderScreenQuad->getPipeline());
		for (auto& mesh : shaderScreenQuad->getMeshes())
		{
			VkBuffer vertexBuffers[] = { mesh->getVertexBuffer() };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffers[i], mesh->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shaderScreenQuad->getPipelineLayout(), 0, 1, &mesh->getDescriptorSets(), 0, nullptr);

			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(mesh->getIndices().size()), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}

}



void VulkanRenderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

// --------------------------------------------------------- UNIFORM BUFFER  !!! ADD NEW SHADER HERE !!! ------------------------------------------------------------------------------------------

void VulkanRenderer::createDescriptorSetLayout()
{
	shader->createDescriptorSetLayout(device);
	
	shaderScreenQuad->createDescriptorSetLayout(device);

	skyboxShader->createDescriptorSetLayout(device);
}

//Create uniform buffer for new shaders here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  !!! ADD NEW SHADER HERE !!!  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void VulkanRenderer::createUniformBuffers()
{
	for (auto & mesh : shader->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}

	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}
	
	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createUniformBuffers(swapChainImages, device, physicalDevice);
	}
}

void VulkanRenderer::updateUniformBuffer(uint32_t currentImage)
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, *camera, swapChainExtent, device);
	}
	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->updateUniformBuffer(currentImage, *camera, swapChainExtent, device);
	}

}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ADD SHADER MESHE TO DESCRIPTOR POOL SIZE  !!! ADD NEW SHADER HERE !!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void VulkanRenderer::createDescritorPool()
{
	shader->createDescritorPool(device, swapChainImages.size());
	
	shaderScreenQuad->createDescritorPool(device, swapChainImages.size());

	skyboxShader->createDescritorPool(device, swapChainImages.size());
}

//Create descriptor set for new shaders here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!! ADD NEW SHADER HERE !!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void VulkanRenderer::createDescriptorSet()
{
	shader->createDescriptorSets(swapChainImages, device);

	shaderScreenQuad->createDescriptorSets(swapChainImages, device);

	skyboxShader->createDescriptorSets(swapChainImages, device);
}

// --------------------------------------------------------- BasicVertex BUFFER -----------------------  !!! ADD NEW SHADER HERE !!!-------------------------------------------------------------------

void VulkanRenderer::createVertexBuffer()
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}

	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
}

//!!!ADD NEW SHADER HERE !!!

void VulkanRenderer::createIndexBuffer()
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}

	for (auto& mesh : shaderScreenQuad->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
	}
}

// --------------------------------------------------------- TEXTURE ------------------------- !!! ADD NEW SHADER HERE IF TEXTURED!!!-----------------------------------------------------------------

void VulkanRenderer::createTextureImage()
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->getTexture()->createTexture(device, physicalDevice, commandPool, graphicsQueue);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->createSkybox(device, physicalDevice, commandPool, graphicsQueue);
	}
}

//!!!ADD NEW SHADER HERE IF TEXTURED!!!

void VulkanRenderer::createTextureImageView()
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->getTexture()->getTextureImageView() = VulkanHelperFunctions::createImageView(mesh->getTexture()->getTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->getSkyboxImageView() = VulkanHelperFunctions::createSkyboxView(mesh->getSkybox()->getSkyboxImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);
	}
}

void VulkanRenderer::createTextureSampler()
{
	for (auto& mesh : shader->getMeshes())
	{
		mesh->getTexture()->createTextureSampler(device);
	}

	for (auto& mesh : skyboxShader->getMeshes())
	{
		mesh->getSkybox()->createSkyboxSampler(device);
	}
}

// --------------------------------------------------------- DEPTH RESOURCES ------------------------------------------------------------------------------------------

void VulkanRenderer::createDepthResource()
{
	VkFormat depthFormat = findDepthFormat();

	VulkanHelperFunctions::createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory,device, physicalDevice);

	depthImageView = VulkanHelperFunctions::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, device);

	
}

VkFormat VulkanRenderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format!");
}

VkFormat VulkanRenderer::findDepthFormat()
{
	return findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool VulkanRenderer::hasStencilComponet(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}


// --------------------------------------------------------- DRAWING ------------------------------------------------------------------------------------------

void VulkanRenderer::createSyncObjects()
{
	imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create sync objects for a frame!");
		}
	}
}

void VulkanRenderer::drawFrame()
{
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	} 
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphore[] = { imageAvailableSemaphore[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR 
		|| result == VK_SUBOPTIMAL_KHR
		|| framebufferResized)
	{
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) & MAX_FRAMES_IN_FLIGHT;
}

// --------------------------------------------------------- DRAWING OFFSCREEN ------------------------------------------------------------------------------------------


void VulkanRenderer::createOffscreen()
{
	offScreen = OffscreenBuffer();

	offScreen.createOffscreen(findDepthFormat(), swapChainExtent, device, physicalDevice, swapChainImageFormat, *shaderScreenQuad->getMeshes()[0]);

}

// --------------------------------------------------------- CHECKS ------------------------------------------------------------------------------------------

void VulkanRenderer::checkExtentionSupport()
{
	uint32_t extentionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extentionCount, nullptr);

	std::vector<VkExtensionProperties> extentions(extentionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extentionCount, extentions.data());

	std::cout << " available extensions: \n";
	for (const auto& extention : extentions)
	{
		std::cout << "\t" << extention.extensionName << "\n";
	}
}

bool VulkanRenderer::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			std::cout << "Validation Layer support not found!" << std::endl;
			return false;
		}
	}

	std::cout << "Validation Layer support all good!" << std::endl;
	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

bool VulkanRenderer::isPhysicalDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainGood = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainGood = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainGood;
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extentionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extentionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extentionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extentionCount, availableExtensions.data());


	for (const auto& deviceExtension : deviceExtensions)
	{
		bool hasRequiredExtensions = false;
		for (const auto& extension : availableExtensions)
		{
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasRequiredExtensions = true;
				break;
			}
		}

		if (!hasRequiredExtensions)
		{
			std::cout << "Required extensions not found!" << std::endl;
			return false;
		}
	
	}

	std::cout << "Required extensions all good!" << std::endl;
	return true;

}

// --------------------------------------------------------- CLEANUP -------------------------------------------------------------------------------------------------

void VulkanRenderer::cleanup()
{
	cleanupSwapChain();

	vkDestroyDescriptorSetLayout(device, shader->getDescriptorSetLayout(), nullptr);
	for (auto& mesh : shader->getMeshes())
	{
		vkDestroyBuffer(device, mesh->getIndexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getIndexBufferMemory(), nullptr);

		vkDestroyBuffer(device, mesh->getVertexBuffer(), nullptr);
		vkFreeMemory(device, mesh->getVertexBufferMemory(), nullptr);
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, imageAvailableSemaphore[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphore[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);
	
	vkDestroyDevice(device, nullptr);

	if (enableValidationLayers) 
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}
