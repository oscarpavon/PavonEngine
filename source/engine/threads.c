#include "threads.h"


#include "log.h"

void thread_new_function(void*(*function)(void*), void* argument){
    pthread_t new_thread_id;
    pthread_create(&new_thread_id,NULL,function,argument);
    /* int result = pthread_detach(new_thread_id);
    if(result != 0){
        LOG("ERROR detaching thread\n");
    } */
}

void thread_new_detached(void*(*function)(void*), void* argument, const char* name){
    pthread_t new_thread_id;
    pthread_create(&new_thread_id,NULL,function,argument);
#ifdef LINUX
    pthread_setname_np(new_thread_id,name);
#endif

    int result = pthread_detach(new_thread_id);
    if(result != 0){
        LOG("ERROR detaching thread\n");
    }
}
void thread_init_engine_thread(const char* name, EngineThread* thead, void*(*function)(void*)){
	

}
void thread_new_job(EngineThread* thread){
	

}

void pe_thread_control(Array* thread_commads){
		
		if(thread_commads->block)
				return;

		for (u8 i = 0; i < thread_commads->count; i++)
		{
				PEThreadCommand* new_command = array_get(thread_commads,i);
				if(new_command && !new_command->done){
						new_command->command(new_command->data);//Execute command
						new_command->done = true;
				}
		}
		int undone_jobs = 0;
		for (u8 i = 0; i < thread_commads->count; i++)
		{
				PEThreadCommand* new_command = array_get(thread_commads,i);
				if(new_command && !new_command->done){
						undone_jobs ++;
				}
		}
		
		if(undone_jobs == 0)
				array_clean(thread_commads);
}

