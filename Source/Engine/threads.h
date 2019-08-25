#ifndef ENGINE_THREADS
#define ENGINE_THREADS

void thread_new_function(void*(*function)(void*),void* argument);
void thread_new_detached(void*(*function)(void*),void* argument,const char* name);
#endif // !ENGINE_THREADS