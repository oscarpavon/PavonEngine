#include "vulkan.h"
#include <vulkan/vulkan.h>
#include <engine/log.h>

VkInstance vk_instance;
VkPhysicalDevice vk_physical_device;

#include "debug.h"

int pe_vk_init(){
	int instance_layer_properties_count = 0;
	vkEnumerateInstanceLayerProperties(&instance_layer_properties_count,NULL);
	LOG("VK instance layer count: %i\n",instance_layer_properties_count);	
	
	VkLayerProperties layers_properties[instance_layer_properties_count];
	vkEnumerateInstanceLayerProperties(&instance_layer_properties_count,layers_properties);
	for(int i = 0; i < instance_layer_properties_count; i++){
	//	LOG("%s\n",layers_properties[i].layerName);
	}
	

	const char* validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
	const char* instance_extension[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};


	VkApplicationInfo app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;	
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
	app_info.engineVersion = VK_MAKE_VERSION(1,0,0);
	app_info.pApplicationName = "PavonVKRender";
	app_info.pNext = NULL;
	app_info.pEngineName = "PavonEngine";
	

	VkInstanceCreateInfo instance_info;
	memset(&instance_info,0,sizeof(VkInstanceCreateInfo));	
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.enabledLayerCount = 1;
	instance_info.ppEnabledLayerNames = validation_layers;

	pe_vk_populate_messeger_debug_info(g_messenger_info);
	instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &g_messenger_info;	

	instance_info.enabledExtensionCount = 1;
	instance_info.ppEnabledExtensionNames = instance_extension;
	

	VkResult r = vkCreateInstance(&instance_info,NULL,&vk_instance);	
	if(r != VK_SUCCESS)
		LOG("Can't create vk instance\n");
	pe_vk_setup_debug_messenger();

	//****************
	//Physical devices
	//****************
	int devices_count = 0;
	vkEnumeratePhysicalDevices(vk_instance,&devices_count,NULL);
	if(devices_count == 0)
		LOG("Not devices compatibles\n");
	VkPhysicalDevice phy_devices[devices_count];
	vkEnumeratePhysicalDevices(vk_instance,&devices_count,phy_devices);
	



	return 0;
}
void pe_vk_end(){
	vkDestroyInstance(vk_instance,NULL);		
}
