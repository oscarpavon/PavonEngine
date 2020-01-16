#include "audio_alsa.h"
#include <alsa/asoundlib.h>
#include "../../Engine/log.h"
#include "../../Engine/file_loader.h"
snd_pcm_t *pcm_handle;
snd_pcm_hw_params_t *hardware_parameters;
int dir;
snd_pcm_uframes_t frames = 32;
unsigned int val = 44100;
long loops;
void audio_init(){
	LOG("ALSA library version: %s\n",SND_LIB_VERSION_STR);
	snd_pcm_stream_name(0);
	int result_open_pcm = snd_pcm_open(&pcm_handle,"default",SND_PCM_STREAM_PLAYBACK,0);
	if(result_open_pcm < 0) {
		LOG("Unable to open PCM device: %s\n",snd_strerror(result_open_pcm));
		return;
	}
	
	snd_pcm_hw_params_alloca(&hardware_parameters);

	snd_pcm_hw_params_any(pcm_handle, hardware_parameters);
	
	snd_pcm_hw_params_set_access(pcm_handle,hardware_parameters,SND_PCM_ACCESS_RW_INTERLEAVED);

	snd_pcm_hw_params_set_format(pcm_handle,hardware_parameters,SND_PCM_FORMAT_S16_LE);

	snd_pcm_hw_params_set_channels(pcm_handle,hardware_parameters,2);

	snd_pcm_hw_params_set_rate_near(pcm_handle,hardware_parameters,&val,&dir);
	
	snd_pcm_hw_params_set_period_size_near(pcm_handle,hardware_parameters,&frames,&dir);

	result_open_pcm = snd_pcm_hw_params(pcm_handle,hardware_parameters);
	
	if(result_open_pcm < 0) {
		LOG("Unable to set hardware parameteres: %s\n",snd_strerror(result_open_pcm));
		return;
	}

}

void audio_play(const char* file_path){
	File audio_file;
	load_file(file_path,&audio_file);	

	LOG("Play audio file\n");
	snd_pcm_hw_params_get_period_size(hardware_parameters,&frames,&dir);
	int size = frames * 4; // 2 bytes/sample, 2 channels	 
	char* buffer = (char*) malloc(size);
	memset(buffer,0,size);
//memcpy(buffer,&audio_file.data[44],size);

	snd_pcm_hw_params_get_period_time(hardware_parameters,&val,&dir);

//5 seconds in microseconds divided by period time
	loops = 25000000 / 44100;
	int pcm;
	while( loops > 0)
	{
		loops --;
		int rc = read(audio_file.resource_descriptor.descriptor,buffer,size);
		if(rc == 0){
			LOG("End of file audio\n");
			break;
		}else if ( rc != size) {
			LOG("Short read: read %d bytes \n", rc);
		}
		rc = snd_pcm_writei(pcm_handle,buffer,frames);
		if( rc  == -EPIPE) {
			LOG("EPIPE ERROR\n");
			snd_pcm_prepare(pcm_handle);
		} else if ( rc < 0 ) {
			
			LOG("Error writing to PCM device: %s\n",snd_strerror(pcm));
		} else if( rc != (int) frames) {
			LOG("Short write, write %d fragmes\n",rc);
		}
		LOG("Playing \n");
	}
	
	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buffer);
	close_file(&audio_file);	
	LOG("END PLAY\n");
}


