#ifndef ENGINE_MEMORY
#define ENGINE_MEMORY

#define INIT_MEMORY 250000

void init_engine_memory();
void* allocate_memory(int size);
void clear_engine_memory();
void free_to_marker();
void* engine_memory;

int memory_used;
int memory_marker;

int actual_free_memory;

#endif