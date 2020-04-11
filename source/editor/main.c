#include "editor.h"

int main(int argc , char* argv[]){

  command_parse_from_command_line(argc, argv);

  editor_init();

  pe_program_main_loop(&editor_main_loop, window_editor_main);

  editor_finish();
}
