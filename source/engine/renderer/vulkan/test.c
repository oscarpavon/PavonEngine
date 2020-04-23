#include "vulkan.h"
#include <engine/engine.h>
int main() {
  pe_wm_renderer_type = PEWMVULKAN;

  pe_init();
	
  EngineWindow new_window;
  array_add(&engine_windows, &new_window);
  EngineWindow *main_window = array_pop(&engine_windows);
  window_create(main_window, NULL, "Vulkan");
		

  int extention_count = 0;
  char **extension;
  extension = glfwGetRequiredInstanceExtensions(&extention_count);
  LOG("Extension required count: %i\n", extention_count);

  LOG("%s\n", extension[0]);
  LOG("%s\n", extension[1]);
  pe_vk_init();
  pe_vk_end();

	LOGW("test warning");
  return 0;
}
