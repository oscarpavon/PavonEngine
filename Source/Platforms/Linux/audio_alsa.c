#include "audio_alsa.h"
#include <alsa/asoundlib.h>
#include "../../Engine/log.h"
void audio_init(){
	LOG("ALSA library version: %s\n",SND_LIB_VERSION_STR);
	snd_pcm_stream_name(0);
}

void audio_play(const char* file_path){
	 LOG("Play audio file\n");
	
}


