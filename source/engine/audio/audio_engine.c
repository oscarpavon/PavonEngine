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

void audio_engine_main_thread(void* argument){

    LOG("Audio Engine [OK]\n");

    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };

	#ifdef LINUX		
	audio_init();
	#endif


    pa_simple *stream = NULL;

    int error;
    stream = pa_simple_new(NULL,"test audio",PA_STREAM_PLAYBACK,NULL,"playback",&ss,NULL,NULL,&error);
    if(stream == NULL) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }

    File file;
	load_file("/home/pavon/audio.wav", &file);

    char *buff;
    int buff_size;
    int fd;
    
    if ((fd = open("/home/pavon/test.wav", O_RDONLY)) < 0) {
            fprintf(stderr, __FILE__": open() failed: %s\n", strerror(errno));
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
            fprintf(stderr, __FILE__": dup2() failed: %s\n", strerror(errno));
        }

        close(fd);

for (;;) {
        uint8_t buf[1024];
        ssize_t r;

        /* Read some data ... */
        if ((r = read(STDIN_FILENO, buf, sizeof(buf))) <= 0) {
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
        }

        /* ... and play it */
        if (pa_simple_write(stream, buf, (size_t) r, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        }
    }

	while(1){
		pe_thread_control(&thread_audio_commads);

	}	
}

int pe_audio_init(){
	thread_new_detached(&audio_engine_main_thread,NULL,"Audio",&pe_th_audio_id);
	
}

