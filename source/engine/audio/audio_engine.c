#include "audio_engine.h"
#include "../../engine/engine.h"

#include "../audio.h"


#include <pulse/simple.h>
#include <pulse/error.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


/*Add audio type to audio thread sounds queue*/
void pe_audio_play(PEAudio* audio){


}
void pe_audio_pulse(){

    /* The Sample format to use */
    static const pa_sample_spec sample_specs = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    
    pa_simple *stream = NULL;

    int error;
    stream = pa_simple_new(NULL,"test audio",PA_STREAM_PLAYBACK,NULL,"playback",&sample_specs,NULL,NULL,&error);
    if(stream == NULL) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }

    File effect;
	load_file("/home/pavon/cat.wav", &effect);

    File music;
	load_file("/home/pavon/test.wav", &music);

    for (;;) {
        uint8_t buffer[1024];
        ssize_t bytes_readed;

		if ((bytes_readed = file_read(&effect, buffer, sizeof(buffer))) == 0) {

            if (bytes_readed == 0) /* EOF */
                break;
        }
        if (pa_simple_write(stream, buffer, (size_t) bytes_readed, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        }
        //LOG("playing\n");
    }
}

void audio_engine_main_thread(void* argument){

    LOG("Audio Engine [OK]\n");


    pe_audio_alsa_init();

    audio_play("/home/pavon/cat.wav");

	while(1){
		pe_thread_control(&thread_audio_commads);

	}	
}

int pe_audio_init(){
	thread_new_detached(&audio_engine_main_thread,NULL,"Audio",&pe_th_audio_id);
	
}

