#include "level.h"

#include "../model.h"

#include "../engine.h"

#ifdef EDITOR
#include "../level_editor/editor.h"
#endif // EDITOR

#include "../file_loader.h"


int load_level_to_elements_array(const char* name, Array* load_elements){    

    File level_file;
    if( load_file(name, &level_file) == -1){
        
        LOG("Level not found: %s\n",name);
        return -1;
    }

    load_level_elements_from_json(level_file.data , level_file.size_in_bytes);

    close_file(&level_file);
    return 0;
}