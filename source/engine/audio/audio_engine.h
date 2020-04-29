#ifndef PAVON_AUDIO_ENGINE
#define PAVON_AUDIO_ENGINE

#include "engine/threads.h"
typedef struct PEAudio{
	int duration;
	bool playing;
}PEAudio;

int pe_audio_init();

void pe_audio_play(PEAudio* audio);

PEThread thread_audio;
Array thread_audio_commads;
#endif
