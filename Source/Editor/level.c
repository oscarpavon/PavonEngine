#include "level.h"
#include "editor.h"
void editor_level_new(const char * name){
	LOG("New level\n");

}

void editor_level_save(const char* path){
	
	content_manager_create_engine_binary(path,CONTENT_TYPE_LEVEL);
}
