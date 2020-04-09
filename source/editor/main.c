#include "editor.h"

int main(int argc , char* argv[]){
		
		int command_line_result = command_parse_from_command_line(argc, argv);

    switch (command_line_result) {
    case EDITOR_CONSOLE_MODE_NORMAL: {
    }
    default: {

      engine_init();

      editor_init();
			
			engine_program_main_loop(&editor_main_loop,window_editor_main);

      editor_finish();
    }
    }
}
