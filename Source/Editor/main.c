#include "editor.h"

int main(int argc , char* argv[]){
		
		command_parse_from_command_line(argc, argv);

    init_engine_memory();

    windows_manager_init();    

    engine_init();

    editor_init();

		editor_main_loop();

    editor_finish();
    
    
}
