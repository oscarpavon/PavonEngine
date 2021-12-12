#ifndef PAVON_AUDIO_ENGINE
#define PAVON_AUDIO_ENGINE

#include "engine/threads.h"
#include <engine/file_loader.h>
typedef struct PAudio{
	int duration;
	bool playing;
    File file;
}PAudio;

int pe_audio_init();

void pe_audio_play(PAudio* audio);

PEThread thread_audio;
Array thread_audio_commads;
Array pe_audio_array_queue;
PAudio audio2;
#endif
