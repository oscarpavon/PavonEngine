#ifndef ENGINE_MEMORY
#define ENGINE_MEMORY

#define INIT_MEMORY 7500000

typedef struct StackMemory{
    int used;
    int marker;
    int available;
    int previous_marker;
    void* memory;
}StackMemory;

typedef struct PoolMemory{
    int used;
    int marker;
    int available;
    int previous_marker;
    void* memory;
}PoolMemory;

void init_engine_memory();
void* allocate_memory(int size);
void clear_engine_memory();
void free_to_marker();
void* engine_memory;

void* allocate_stack_memory(StackMemory* stack, int bytes_size);
void free_stack_to_market(StackMemory* stack);

void* allocate_stack_memory_alignmed(int bytes_size, int alignment);

int memory_used;
int memory_marker;
int previous_marker;

int actual_free_memory;

StackMemory vertex_memory;
StackMemory indices;
StackMemory engine_stack_memory;

StackMemory created_components;

PoolMemory arrays_memory;

#endif