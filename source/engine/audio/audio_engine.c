#include "audio_engine.h"
#include "../../engine/engine.h"

#include "../audio.h"

/*Add audio type to audio thread sounds queue*/
void pe_audio_play(PEAudio* audio){


}

void audio_engine_main_thread(void* argument){

	#ifdef LINUX		
	audio_init();
	#endif
	while(1){
		pe_thread_control(&thread_audio_commads);

	}	
}

int pe_audio_init(){
	thread_new_detached(audio_engine_main_thread,NULL,"Audio",&pe_th_audio_id);
	
}

