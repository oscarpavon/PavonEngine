#include "threads.h"

#include "pthread.h"
#include "log.h"

void thread_new_function(void*(*function)(void*), void* argument){
    pthread_t new_thread_id;
    pthread_create(&new_thread_id,NULL,function,argument);
    /* int result = pthread_detach(new_thread_id);
    if(result != 0){
        LOG("ERROR detaching thread\n");
    } */
}

void thread_new_detached(void*(*function)(void*), void* argument){
    pthread_t new_thread_id;
    pthread_create(&new_thread_id,NULL,function,argument);
    int result = pthread_detach(new_thread_id);
    if(result != 0){
        LOG("ERROR detaching thread\n");
    }
}