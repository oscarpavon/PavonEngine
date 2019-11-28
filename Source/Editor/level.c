#include "level.h"
#include "content_manager.h"
#include "path.h"

void editor_level_new(const char * name){


}

void editor_level_save(const char* path){
	content_manager_create_engine_binary(path,CONTENT_TYPE_LEVEL);
}
