#ifndef ENGINE_THREADS
#define ENGINE_THREADS

void thread_new_function(void*(*function)(void*),void* argument);

#endif // !ENGINE_THREADS