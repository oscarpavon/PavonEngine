#ifndef ENGINE_THREADS
#define ENGINE_THREADS

#include "array.h"
#include "pthread.h"

typedef struct{
    pthread_t id;
    Array commands;
    char name[20];

}EngineThread;

typedef struct PEThreadCommand{
    bool done;
    void* data;
    char command_text[1000];
    void (*command)(void*);
}PEThreadCommand;

void thread_new_function(void*(*function)(void*),void* argument);
void thread_new_detached(void*(*function)(void*),void* argument,const char* name);

void thread_engine_thread_system_init();

Array array_engine_threads;

void pe_thread_control(Array* thread_commads);
#endif // !ENGINE_THREADS
