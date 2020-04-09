#ifndef ENGINE_THREADS
#define ENGINE_THREADS

#include "array.h"
#include "pthread.h"

typedef struct{
    pthread_t id;
    Array commands;
    char name[20];

}EngineThread;

void thread_new_function(void*(*function)(void*),void* argument);
void thread_new_detached(void*(*function)(void*),void* argument,const char* name);

void thread_engine_thread_system_init();

Array array_engine_threads;

#endif // !ENGINE_THREADS