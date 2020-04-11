#ifndef ENGINE_RENDER_THREAD
#define ENGINE_RENDER_THREAD
#include "array.h"
#include "threads.h"

void(*engine_user_render_thread_draw)(void);
void(*engine_user_render_thread_init)(void);
void(*engine_user_render_thread_finish)(void);


void engine_init_render();


EngineThread thread_render;

Array array_render_thread_init_commmands;
Array array_render_thread_commands;

bool engine_client_render_thread_initialized;


#endif
