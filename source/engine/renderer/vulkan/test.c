#include "vulkan.h"
#include <engine/engine.h>
int main(){
	pe_init();

	int extention_count = 0;
	char ** extension;
	extension = glfwGetRequiredInstanceExtensions(&extention_count);
	LOG("Extension required count: %i\n",extention_count);
				
	LOG("%s\n",extension[0]);
	LOG("%s\n",extension[1]);
	pe_vk_init();
	pe_vk_end();
	return 0;
}
