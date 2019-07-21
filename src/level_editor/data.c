#include "data.h"
#include "editor.h"
#include "../camera.h"
#include "../gui.h"

FILE* actual_file;
DataType actual_data_type;

Element* current_element = NULL;
int hirarchical_count = 0;


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
    fprintf(actual_file,"\t\"%s\" : [%f , %f ],\n", text ,vec[0] , vec[1]);
    token_count += 4;
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


void save_level_info(FILE* new_file){
    fputs("{\n\t\"model_count\" : ", new_file);
    fprintf(new_file,"%i",element_id_count+1);
    fputs("\n},\n",new_file);
}

void header_info(int i){
    new_text_primitive_token("elements",element_id_count);
    new_text_primitive_token("type",actual_data_type);
    new_text_primitive_token("tokens",token_count);
    
}
void save_header_info(){
    SaveDataFunction save = &header_info;
    new_save_element(save,0);
}

void save_camera_editor_camera_transform(FILE* new_file){     
    fputs(",\n", new_file);
    fputs("{\n\t\"type\" : ", new_file);
    fprintf(new_file,"\"%s\",\n","editor_camera");  
    fputs("\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",300);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",main_camera.position[0] , main_camera.position[1] , main_camera.position[2]);
    fputs("\t\"rot\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f , %f],\n",camera_rotation[0] , camera_rotation[1] , camera_rotation[2], camera_rotation[3]);
    fputs("\t\"texture\" : ", new_file);
    fprintf(new_file,"\"%s\"\n","no_texture");
    fputs("}",new_file);
}

void create_save_data_backup(){
    FILE* level_file = fopen("new_level.lvl","r");
    if(level_file == NULL)
        return;

    fseek(level_file, 0, SEEK_END);
    long file_size = ftell(level_file);
    rewind (level_file);

    char file_buffer[file_size];

    fread(file_buffer,sizeof(char), file_size, level_file);

    FILE* new_file = fopen("level.bkp1","w");
    fputs(file_buffer,new_file);
    fclose(new_file);

    fclose(level_file);
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
        new_text_primitive_token("path",mesh->model_id);
        break;
    }       
    default:
        break;
    }
}
void components_count(){
    new_text_primitive_token("count",current_element->components_count);
}
void components_data(){
    SaveDataFunction component = &save_element_component_data;
    
    for(int i = 0; i < current_element->components_count ; i++){
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

void level_paths_data(){

    for(int i = 0; i< texts.count ; i++){       
        hirachical_tab();
        fprintf(actual_file,"\"%s\",\n",get_from_array(&texts,i));
    }
   
}

void level_data(){
    new_array_data("paths",&level_paths_data);
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
    //new_array_data("elements",&level_elements_data);
    //new_array_data("data",&level_data);
   
}
void save_level(int id){
    new_array_data("level",&leve_data_element_plus_data);
}

void save_level_data(const char* level_name){    
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, level_folder);
    strcat(save_name,level_name);
    strcat(save_name,".lvl");    

    create_save_data_backup();

    FILE* new_file = fopen(save_name,"w+");
    actual_file = new_file;
    actual_data_type = DATA_TYPE_LEVEL;  
 
    SaveDataFunction level =  &save_level;
    new_save_element(level,0);
    
    
    fclose(new_file);

    LOG("Saved to %s\n",save_name);
}

void save_buttons_data(int id){    
    Button* button = get_from_array(actual_buttons_array,id);
    if(button != NULL){
        new_text_token("name",button->name);
        new_text_vec2_token("pos",button->position);
        new_text_vec2_token("size",button->size);
        //new_text_primitive_token("relative",button->relative_to);
        new_text_primitive_token("function_id",button->action_function_id);
    }   
    
}



void save_gui_data(const char* gui_name){
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, gui_folder);
    strcat(save_name,gui_name);
    strcat(save_name,".gui");   

    FILE* new_file = fopen(save_name,"w+");
    actual_file = new_file;
    actual_data_type = DATA_TYPE_GUI;
    element_id_count = actual_buttons_array->count;

    save_header_info();
    fputs("{\n", new_file);
    SaveDataFunction save = &save_buttons_data;
    for(int i = 0; i < actual_buttons_array->count ; i++){
         new_save_element(save,i);
    }
    fputs("\n}", new_file);

    fclose(new_file);

    LOG("Saved to %s\n",save_name);
}