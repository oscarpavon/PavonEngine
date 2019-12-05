#include "vulkan.h"
#include "string.h"
int vulkan_renderer_init(){

	VkApplicationInfo application_info;	
	memset(&application_info,0,sizeof(application_info));
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	
	VkInstanceCreateInfo instance_create_info;
	memset(&instance_create_info,0,sizeof(instance_create_info));
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &application_info;
	
	vkCreateInstance(&instance_create_info,NULL,&vk_instance);


}
