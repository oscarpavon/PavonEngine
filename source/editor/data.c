#include "data.h"
#include "editor.h"
#include "../engine/camera.h"
#include "../engine/gui.h"
#include "ProjectManager/project_manager.h"

#include "Serialization/json_writer.h"

Element* current_element = NULL;

int previous_id_saved = 0;
int previous_path_id = 0;
StaticMeshComponent* previous_component = NULL;

void save_models_id(void* component){
    StaticMeshComponent* mesh = component;
    int* path_id = array_get(&mesh->meshes,0);
    int count = 0;
    int offset = 0;
    if(previous_path_id != *path_id){
        previous_path_id = *path_id;
        if(previous_component){
            offset = previous_component->meshes.count-1;
            previous_id_saved += offset;
        }
    }

    fprintf(actual_file,"%i,",*path_id);
    
    
    for(int o = 0; o < mesh->meshes.count-1 ; o++){
        int id = previous_id_saved + o;
        fprintf(actual_file,"%i,",id);
        count++;
    }
    previous_component = mesh;

}
void save_textures_id(void* component){
    StaticMeshComponent* mesh = component; 
    if(mesh->textures.count == 0){
        fprintf(actual_file,"%i,",0);
    }  
   for(int i = 0; i<mesh->textures.count; i++){
       int* texture_id = array_get(&mesh->textures,i);
       fprintf(actual_file,"%i,",*texture_id);
   }
}

void save_element_component_data(int id){
    ComponentDefinition* component = array_get(&current_element->components,id);
    new_text_primitive_token("type",component->type);
    switch (component->type)
    {
    case TRASNFORM_COMPONENT:{
        TransformComponent* transform = &component->data[0];
        new_text_vec3_token("position",transform->position);
        new_text_vec4_token("rotation",transform->rotation);
        break;
    }
    case STATIC_MESH_COMPONENT:{
        StaticMeshComponent* mesh = &component->data[0];
        if(mesh->meshes.count >= 1){
            new_array_data_with_pointer("models",&save_models_id,mesh);
            new_array_data_with_pointer("textures",&save_textures_id,mesh);
            break;
        }

        break;
    }
    case CAMERA_COMPONENT:
        {
            CameraComponent* camera = component->data;
            new_text_vec3_token("position",camera->position);
            break;
        }
    case LEVEL_OF_DETAIL_COMPONENT:
    {
        LevelOfDetailComponent* detail = component->data;
        new_text_primitive_token("texture",detail->texture_id);
        break;
    } 
    default:
        break;
    }
}

void components_data(){
    SaveDataFunction component = &save_element_component_data;
    
    for(int i = 0; i < current_element->components.count ; i++){
        new_save_element(component,i);
    }
}

void save_level_element_data(int id){ 
    
    Element* element = array_get(&editor_elements,id);
    current_element = element;

    new_text_token("name",element->name); 
    new_array_data("components",&components_data);
}


void level_elements_data(){
    SaveDataFunction element_save_function =  &save_level_element_data;
    for(int i = 0; i < editor_elements.count ; i++){        
        
        new_save_element(element_save_function,i);

    }

}

void save_model_paths(){

    for(int i = 0; i< texts.count ; i++){       
        hirachical_tab();
        fprintf(actual_file,"\"%s\",\n",array_get(&texts,i));
    }
   
}
void save_textures_paths(){

    for(int i = 0; i< textures_paths.count ; i++){       
        hirachical_tab();
        fprintf(actual_file,"\"%s\",\n",array_get(&textures_paths,i));
    }
   
}

void level_data(){
    new_array_data("models",&save_model_paths);
    new_array_data("textures",&save_textures_paths);
}

typedef void(*function_with_function)( const char*text, void(*function2)(void));

void new_element_wiht_data( function_with_function t,const char* text, void(*function2)(void)){
    hirachical_tab(); fputs("{\n", actual_file);
    hirarchical_count++;
    t(text,function2);
    hirarchical_count--;
    hirachical_tab(); fputs("},\n",actual_file);
}

void leve_data_element_plus_data(){
   new_element_wiht_data(&new_array_data,"elements",&level_elements_data);
   new_element_wiht_data(&new_array_data,"data",&level_data);   
}

void save_level(int id){
    new_array_data("level",&leve_data_element_plus_data);
}

void save_level_data(const char* level_name){
    if(strcmp(level_name, "") == 0){
        if( strcmp(opened_file_name,"") == 0)
            return;
        level_name = opened_file_name;
    }
        
    char new_file_name_with_path[strlen(project_manager_current_path) + 30];
	memset(new_file_name_with_path,0,sizeof(new_file_name_with_path));
    sprintf(new_file_name_with_path,"%s%s%s.lvl",project_manager_current_path,"/Content/",level_name);
  
    FILE* new_file = fopen(new_file_name_with_path,"w");
    if(!new_file){
        LOG("File not created: %s\n",new_file_name_with_path);
        return;
    }
    actual_file = new_file;
     
 
    SaveDataFunction level =  &save_level;
    new_save_element(level,0);
    
    
    fclose(new_file);
    previous_id_saved = 0;
    LOG("Saved to %s\n",new_file_name_with_path);
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
