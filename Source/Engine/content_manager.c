#include "content_manager.h"
#include <string.h>
#include "engine.h"

void content_manager_load_content_to_file(const char* path, File* file){
    
}
int content_manager_get_content_type_from_binary(const char* path){
	
    File new_file;
    load_file(path,&new_file);

    u32 file_type;
    memcpy(&file_type,new_file.data,4);

    if(file_type != PVN_BINARY_FILE_MAGIC){
        LOG("File not reconized\n");
        close_file(&new_file);
        return -1;
    }
    LOG("Pavon Binary loaded\n");
	u32 binary_version;
	memcpy(&binary_version,new_file.data+4,4);//TODO: manage binary_versions 

    u32 total_binary_size;
    memcpy(&total_binary_size,new_file.data+8,4);

	u32 content_GUID;
	memcpy(&content_GUID,new_file.data+12,4);
	LOG("Content GUID: %i\n",content_GUID);	
    
	u32 data_size;
    memcpy(&data_size,new_file.data+16,4);
	LOG("Binary data size %i\n",data_size);

	
    u32 content_type;
    memcpy(&content_type,new_file.data+20,4);
	close_file(&new_file);
	return  content_type;
}
int content_manager_load_content(const char* path){

    File new_file;
    if(load_file(path,&new_file) == -1)
		return -1;

    u32 file_type;
    memcpy(&file_type,new_file.data,4);

    if(file_type != PVN_BINARY_FILE_MAGIC){
        LOG("File not reconized\n");
        close_file(&new_file);
        return -1;
    }
    LOG("Pavon Binary loaded\n");
	u32 binary_version;
	memcpy(&binary_version,new_file.data+4,4);//TODO: manage binary_versions 

    u32 total_binary_size;
    memcpy(&total_binary_size,new_file.data+8,4);

	u32 content_GUID;
	memcpy(&content_GUID,new_file.data+12,4);
	LOG("Content GUID: %i\n",content_GUID);	
    
	u32 data_size;
    memcpy(&data_size,new_file.data+16,4);
	LOG("Binary data size %i\n",data_size);

	
    u32 content_type;
    memcpy(&content_type,new_file.data+20,4);
    

    switch (content_type)
    {
    case CONTENT_TYPE_STATIC_MESH:{
        Array* prev_array = actual_model_array;
        actual_model_array = &array_models_loaded;
        model_load_from_content(new_file.data+24,data_size);

        actual_model_array = prev_array;
        
        engine_add_element(2);
        
        break;
    }
   	case CONTENT_TYPE_TEXTURE:{
		engine_add_texture_from_memory_to_selected_element(new_file.data+24,data_size);	
		LOG("Texture loaded\n");	
		break;
							  }
	case CONTENT_TYPE_PROJECT:{
										
								  
								  break;							
							  }
    default:
        break;
    }
   

    close_file(&new_file);
	return content_type;
}

void content_manager_create_static_mesh(const char* path){
    Content new_content;
    memset(&new_content,0,sizeof(Content));
    //char new_path[strlen(pavon_the_game_project_folder)+40];
    //sprintf(new_path,"%s%s%s",pavon_the_game_project_folder,"Content/content",".pvnf");
    //serializer_serialize_data(new_path,content_manager_serialize_static_mesh);
    
}

void content_manager_create_engine_content_type(const char* path, ContentType type){
    switch (type)
    {
    case CONTENT_TYPE_STATIC_MESH:
        {
            content_manager_create_static_mesh(path);
        }
        break;
    
    default:
        break;
    }
}
