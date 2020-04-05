#include "audio_engine.h"
#include "../../Engine/engine.h"

#include "../audio.h"

void audio_engine_main_thread(void* argument){


	//audio_init();
	while(1){

	}	
}

int audio_engine_init(){
	thread_new_detached(audio_engine_main_thread,NULL,"Audio");
	
}

