
#include <android_native_app_glue.h>
void pe_android_handle_cmd(struct android_app *app, int32_t cmd);
int32_t pe_android_input_handle(struct android_app* app, AInputEvent* event);
void pe_android_poll_envents();
