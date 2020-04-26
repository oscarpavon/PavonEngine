#include "data.h"
#include "editor.h"
#include "../engine/camera.h"
#include "../engine/gui.h"
#include "ProjectManager/project_manager.h"

#include "Serialization/json_writer.h"

#include "ThirdParty/parson.h"

Element* current_element = NULL;

int previous_id_saved = 0;
u8 previous_path_id = 0;
StaticMeshComponent* previous_component = NULL;

void pe_serialize_textures_ids(StaticMeshComponent* mesh, JSON_Array* array){
    if(mesh->textures.count == 0){
				json_array_append_number(array,0);
    }  
   for(int i = 0; i<mesh->textures.count; i++){
       int* texture_id = array_get(&mesh->textures,i);
				json_array_append_number(array,*texture_id);
   }
}
void pe_serialize_models_ids(StaticMeshComponent* mesh,JSON_Array* array){
   
    u8* path_id = array_get(&mesh->meshes,0);
    int count = 0;
    int offset = 0;
    if(previous_path_id != *path_id){
        previous_path_id = *path_id;
        if(previous_component){
            offset = previous_component->meshes.count-1;
            previous_id_saved += offset;
        }
    }
		
		json_array_append_number(array,*path_id);
    
    for(int o = 0; o < mesh->meshes.count-1 ; o++){
        u8 id = previous_id_saved + o;
				json_array_append_number(array,id);
        count++;
    }

}

void pe_serialize_components(Element *element, JSON_Array *array) {
  for (int i = 0; i < element->components.count; i++) {
    ComponentDefinition *component = array_get(&element->components, i);
    JSON_Value *element_obj_val = json_value_init_object();
    JSON_Object *element_obj = json_value_get_object(element_obj_val);

    json_object_set_number(element_obj, "type", component->type);
    json_array_append_value(array, element_obj_val);
    switch (component->type) {
    case TRASNFORM_COMPONENT: {
      TransformComponent *transform = component->data;
      JSON_Value *array_component_val = json_value_init_array();
      JSON_Array *array_component = json_value_get_array(array_component_val);

      json_array_append_number(array_component, transform->position[0]);
      json_array_append_number(array_component, transform->position[1]);
      json_array_append_number(array_component, transform->position[2]);

      JSON_Value *array_rot_val = json_value_init_array();
      JSON_Array *array_rot = json_value_get_array(array_rot_val);

      json_array_append_number(array_rot, transform->rotation[0]);
      json_array_append_number(array_rot, transform->rotation[1]);
      json_array_append_number(array_rot, transform->rotation[2]);
      json_array_append_number(array_rot, transform->rotation[3]);

      JSON_Value *array_scale_val = json_value_init_array();
      JSON_Array *array_scale = json_value_get_array(array_scale_val);

      json_array_append_number(array_scale, transform->scale[0]);
      json_array_append_number(array_scale, transform->scale[1]);
      json_array_append_number(array_scale, transform->scale[2]);

      json_object_set_value(element_obj, "position", array_component_val);
      json_object_set_value(element_obj, "rotation", array_rot_val);
      json_object_set_value(element_obj, "scale", array_scale_val);

      break;
    }
    case STATIC_MESH_COMPONENT: {
      StaticMeshComponent *mesh = component->data;
      if (mesh->meshes.count >= 1) {
        JSON_Value *array_component_val = json_value_init_array();
        JSON_Array *array_component = json_value_get_array(array_component_val);

        JSON_Value *array_rot_val = json_value_init_array();
        JSON_Array *array_rot = json_value_get_array(array_rot_val);

        pe_serialize_models_ids(mesh, array_component);
        pe_serialize_textures_ids(mesh, array_rot);

        json_object_set_value(element_obj, "models", array_component_val);
        json_object_set_value(element_obj, "textures", array_rot_val);

        break;
      }

      break;
    }
    case CAMERA_COMPONENT:
        {
            CameraComponent* camera = component->data;
     //       new_text_vec3_token("position",camera->position);
            break;
        }
    case LEVEL_OF_DETAIL_COMPONENT:
    {
        LevelOfDetailComponent* detail = component->data;
    //    new_text_primitive_token("texture",detail->texture_id);
        break;
    } 
    }
  }
}

void pe_serialize_elements(JSON_Array* array){
	for(int i = 0; i < editor_elements.count; i++){
			Element* element = array_get(&editor_elements,i);
			JSON_Value *element_obj_val = json_value_init_object();
			JSON_Object *element_obj = json_value_get_object(element_obj_val);
			json_object_set_string(element_obj,"name",element->name);	


			JSON_Value* array_component_val = json_value_init_array();
			JSON_Array* array_component = json_value_get_array(array_component_val);
			pe_serialize_components(element,array_component);

			json_object_set_value(element_obj,"components",array_component_val);	
				
			json_array_append_value(array,element_obj_val);	
	}

}

void pe_serialize_level() {
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  char *serialized_string = NULL;
	
  JSON_Value *value2 = json_value_init_object();
  JSON_Object *value2_obj = json_value_get_object(value2);

  JSON_Value *elements_obj_val = json_value_init_object();
  JSON_Object *elements_obj = json_value_get_object(elements_obj_val);

  JSON_Value *data_obj_val = json_value_init_object();
  JSON_Object *data_obj = json_value_get_object(data_obj_val);

	JSON_Value * elements_array_val = json_value_init_array();

	JSON_Array * element_array = json_value_get_array(elements_array_val);	
	
	JSON_Value * data_array_val = json_value_init_array();

	JSON_Array * data_array = json_value_get_array(data_array_val);	

	JSON_Value * models_paths_val= json_value_init_array();

	JSON_Array * models_paths_arr= json_value_get_array(models_paths_val);	

	JSON_Value * textures_paths_val= json_value_init_array();

	JSON_Array * textures_paths_arr= json_value_get_array(textures_paths_val);	

	JSON_Value * level = json_value_init_array();

	JSON_Array * level_arr	= json_value_get_array(level);

	json_object_set_number(value2_obj, "type", 3);
	json_array_append_value(level_arr,elements_obj_val);
	json_array_append_value(level_arr,data_obj_val);
	
		
  JSON_Value *model_obj_val= json_value_init_object();
  JSON_Object *model_obj= json_value_get_object(model_obj_val);

  JSON_Value *texture_obj_val= json_value_init_object();
  JSON_Object *texture_obj= json_value_get_object(texture_obj_val);
	
	json_object_set_value(model_obj,"models",models_paths_val);
	json_object_set_value(texture_obj,"textures",textures_paths_val);

	json_array_append_value(data_array,model_obj_val);
	json_array_append_value(data_array,texture_obj_val);
	
	json_object_set_value(root_object,"level",level);
	json_object_set_value(elements_obj,"elements",elements_array_val);
	json_object_set_value(data_obj,"data",data_array_val);

	pe_serialize_elements(element_array);

	for(int i = 0; i < pe_arr_models_paths.count ; i++){
			char* path = (char*)array_get(&pe_arr_models_paths,i);
			json_array_append_string(models_paths_arr,path);	
	}
	
	for(int i = 0; i < textures_paths.count ; i++){
			char* path = (char*)array_get(&textures_paths,i);
			json_array_append_string(textures_paths_arr,path);	
	}
	
	serialized_string = json_serialize_to_string_pretty(root_value);
	 
	fputs(serialized_string,actual_file);	

	json_free_serialized_string(serialized_string);
  json_value_free(root_value);
}

void save_level_data(const char* level_name){
    if(strcmp(level_name, "") == 0){
        if( strcmp(opened_file_name,"") == 0)
            return;
        level_name = opened_file_name;
    }
        
    FILE* new_file = fopen(level_name,"w");
    if(!new_file){
        LOG("File not created: %s\n",level_name);
        return;
    }
    actual_file = new_file;
    
		pe_serialize_level();	
    
    fclose(new_file);
    previous_id_saved = 0;
    LOG("Saved to %s\n",level_name);
}

void serializer_serialize_data(const char* path, void(*function)(void)){
    if(strcmp(path, "") == 0){
        if( strcmp(opened_file_name,"") == 0)
            return;
        path = opened_file_name;
    }      
  
    FILE* new_file = fopen(path,"w");
    if(!new_file){
        LOG("File not created: %s\n",path);
        return;
    }
    actual_file = new_file;     
 
    function();    
    
    fclose(new_file);
    previous_id_saved = 0;
    LOG("Saved to %s\n",path);
}




//**********************
//GUI save data
//**********************
void save_buttons_data(int id){    
    Button* button = array_get(actual_buttons_array,id);
    if(button != NULL){
        new_text_token("name",button->name);
        new_text_vec2_token("position",button->position);
        new_text_vec2_token("size",button->size);
        //new_text_primitive_token("relative",button->relative_to);
        new_text_primitive_token("function",button->action_function_id);
    }   
    
}


void ui_elements_data(){
    SaveDataFunction save = &save_buttons_data;
    for(int i = 0; i < actual_buttons_array->count ; i++){
         new_save_element(save,i);
    }
}

void save_gui_data(const char* gui_name){
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, gui_folder);
    strcat(save_name,gui_name);
    strcat(save_name,".gui");   

    FILE* new_file = fopen(save_name,"w");
    actual_file = new_file;    
    element_id_count = actual_buttons_array->count;  
    
    new_element(&ui_elements_data);

    fclose(new_file);

    LOG("Saved to %s\n",save_name);
}

void save_commnad_history(const char* entered_commnad){
    FILE* new_file = fopen("command_history.txt","a");
    fprintf(new_file,"%s",&entered_commnad[1]);
    fputc('\n',new_file);
    fclose(new_file);
}
