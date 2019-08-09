#include "data.h"
#include "editor.h"
#include "../camera.h"
#include "../gui.h"

FILE* actual_file;

Element* current_element = NULL;
int hirarchical_count = 0;
int previous_id_saved = 0;
int previous_path_id = 0;
StaticMeshComponent* previous_component = NULL;

void hirachical_tab(){
    for(int i = 0; i< hirarchical_count ; i++){
        fputs("\t", actual_file);
    }
}
int add_array_of_numbers(int count){
    
    return 0;
}
int token_count = 0;
void new_text_token(const char* type, const char* value){
    hirachical_tab(); 
    fprintf(actual_file,"\"%s\" : \"%s\",\n",type,value);
    token_count += 2;
}
void new_text_primitive_token(const char* type, int value){
    hirachical_tab(); 
    fprintf(actual_file,"\"%s\" : %i,\n",type,value);
    token_count += 2;
}

void new_text_vec4_token(const char* text,vec4 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2], vec[3]);
    token_count += 6;
}

void new_text_vec3_token(const char* text,vec3 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2]);
    token_count += 5;
}

void new_text_vec2_token(const char* text,vec2 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f],\n", text ,vec[0] , vec[1]);
    token_count += 4;
}

void new_array_data_with_pointer(const char* text,void(*function)(void*),void* data){
    hirachical_tab(); fprintf(actual_file,"\"%s\" : [",text);
    hirarchical_count++;
    function(data);
    hirarchical_count--;    
    fputs("],\n",actual_file);
    
}
void new_array_data(const char* text,void(*function)(void)){
    hirachical_tab(); fprintf(actual_file,"\"%s\" : [\n",text);
    hirarchical_count++;
    function();
    hirarchical_count--;    
    hirachical_tab(); fputs("],\n",actual_file);
    
}

void new_element(void(*function)(void)){
    hirachical_tab(); fputs("{\n", actual_file);
    hirarchical_count++;
    function();
    hirarchical_count--;
    hirachical_tab(); fputs("},\n",actual_file);
}

void new_save_element(SaveDataFunction function, int data_id){
    hirachical_tab(); fputs("{\n", actual_file);
    hirarchical_count++;
    function(data_id);
    hirarchical_count--;
    hirachical_tab(); fputs("},\n",actual_file);   
   
    token_count++;
}

void save_models_id(void* component){
    StaticMeshComponent* mesh = component;
    int* path_id = get_from_array(&mesh->meshes,0);
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
       int* texture_id = get_from_array(&mesh->textures,i);
       fprintf(actual_file,"%i,",*texture_id);
   }
}

void save_element_component_data(int id){
    ComponentDefinition* component = get_from_array(&current_element->components,id);
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
    
    Element* element = get_from_array(&editor_elements,id);
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
        fprintf(actual_file,"\"%s\",\n",get_from_array(&texts,i));
    }
   
}
void save_textures_paths(){

    for(int i = 0; i< textures_paths.count ; i++){       
        hirachical_tab();
        fprintf(actual_file,"\"%s\",\n",get_from_array(&textures_paths,i));
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
        

    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, level_folder);
    strcat(save_name,level_name);
    strcat(save_name,".lvl");    
  
    FILE* new_file = fopen(save_name,"w");
    if(!new_file){
        LOG("File not created: %s\n",save_name);
        return;
    }
    actual_file = new_file;
     
 
    SaveDataFunction level =  &save_level;
    new_save_element(level,0);
    
    
    fclose(new_file);
    previous_id_saved = 0;
    LOG("Saved to %s\n",save_name);
}

void save_buttons_data(int id){    
    Button* button = get_from_array(actual_buttons_array,id);
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