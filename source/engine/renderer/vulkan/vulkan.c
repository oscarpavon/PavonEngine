#include "vulkan.h"
#include <vulkan/vulkan.h>
#include <engine/log.h>


VkInstance vk_instance;
VkDebugUtilsMessengerEXT debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT g_messenger_info;

static VKAPI_ATTR VkBool32 VKAPI_CALL pe_vk_debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data) {
	
	printf("PEvk_validation: %s\n",callback_data->pMessage);

	return VK_FALSE;	
}

VkResult pe_vk_create_debug_messeger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* info,
		const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messeger){

		PFN_vkCreateDebugUtilsMessengerEXT func = 
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkCreateDebugUtilsMessengerEXT");

		if(func != NULL){
			return func(instance,info,allocator,debug_messeger);
		}else{
			LOG("Cant't create debug messenger\n");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

}
void pe_vk_populate_messeger_debug_info(VkDebugUtilsMessengerCreateInfoEXT info_messeger){

	info_messeger.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info_messeger.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
													VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
													VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info_messeger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info_messeger.pfnUserCallback = pe_vk_debug_callback;
	info_messeger.pUserData = NULL;
}

void pe_vk_setup_debug_messenger(){

	pe_vk_create_debug_messeger(vk_instance,&g_messenger_info,NULL,&debug_messenger);	

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
	return 0;
}
void pe_vk_end(){
	vkDestroyInstance(vk_instance,NULL);		
}
