#include "editor.h"

int main(int argc , char* argv[]){
		
		int command_line_result = command_parse_from_command_line(argc, argv);
		
    init_engine_memory();

    if (command_line_result != EDITOR_CONSOLE_MODE_NORMAL) {

      windows_manager_init();

      engine_init();

      editor_init();

      editor_main_loop();

      editor_finish();
    } else {
      audio_init();

      audio_play("/home/pavon/music.wav");
    }
}
