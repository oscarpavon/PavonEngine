#include "vulkan.h"
#include <vulkan/vulkan.h>
#include <engine/log.h>
#include <string.h>
#include <engine/macros.h>
#include "debug.h"

const char* validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
const char* instance_extension[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

#define VKVALID(f) if(f != VK_SUCCESS){LOG("Not succes\n");}

int pe_vk_new_log_divice(){

	const float queue_priority = 1.f;

	VkDeviceQueueCreateInfo qinfo;
	ZERO(qinfo);
	qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qinfo.pQueuePriorities = &queue_priority;
	qinfo.queueCount = 1;
	qinfo.queueFamilyIndex = q_graphic_family;	
	


	VkDeviceCreateInfo info;
	ZERO(info);
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	
	info.enabledLayerCount = 1;
	info.ppEnabledLayerNames = validation_layers;
	info.queueCreateInfoCount = 1;
	info.pQueueCreateInfos = &qinfo;
	
	VKVALID(vkCreateDevice(vk_physical_device,&info,NULL,&vk_device))
}
int pe_vk_init(){
	int instance_layer_properties_count = 0;
	vkEnumerateInstanceLayerProperties(&instance_layer_properties_count,NULL);
	LOG("VK instance layer count: %i\n",instance_layer_properties_count);	
	
	VkLayerProperties layers_properties[instance_layer_properties_count];
	vkEnumerateInstanceLayerProperties(&instance_layer_properties_count,layers_properties);
	for(int i = 0; i < instance_layer_properties_count; i++){
	//	LOG("%s\n",layers_properties[i].layerName);
	}


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

	ZERO(g_messenger_info);
	pe_vk_populate_messeger_debug_info(&g_messenger_info);
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
	vk_physical_device = phy_devices[0];		
	

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device,&queue_family_count,NULL);
	VkQueueFamilyProperties q_families[queue_family_count];
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device,&queue_family_count,q_families);

	for(int i = 0; i<queue_family_count; i++){
		VkQueueFamilyProperties property = q_families[i];
		if(property.queueFlags == VK_QUEUE_GRAPHICS_BIT){
			q_graphic_family = i;
			break;
		}
		
	}

	pe_vk_new_log_divice();	

	return 0;
}
void pe_vk_end(){
	pe_vk_debug_end();
	vkDestroyDevice(vk_device,NULL);	
	vkDestroyInstance(vk_instance,NULL);		
}
