#include "data.h"
#include "editor.h"
#include "../camera.h"
#include "../gui.h"

FILE* actual_file;
DataType actual_data_type;

int add_array_of_numbers(int count){
    
    return 0;
}
int token_count = 0;
void new_text_token(const char* type, const char* value){
    fprintf(actual_file,"\t\"%s\" : \"%s\",\n",type,value);
    token_count += 2;
}
void new_text_primitive_token(const char* type, int value){
    fprintf(actual_file,"\t\"%s\" : %i,\n",type,value);
    token_count += 2;
}

void new_text_vec4_token(const char* text,vec4 vec){
    fprintf(actual_file,"\t\"%s\" : [%f , %f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2], vec[3]);
    token_count += 6;
}

void new_text_vec3_token(const char* text,vec3 vec){
    fprintf(actual_file,"\t\"%s\" : [%f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2]);
    token_count += 5;
}

void new_text_vec2_token(const char* text,vec2 vec){
    fprintf(actual_file,"\t\"%s\" : [%f , %f ],\n", text ,vec[0] , vec[1]);
    token_count += 4;
}

void new_save_element(SaveDataFunction function){
    fputs("{\n", actual_file);
    function();
    fputs("},\n",actual_file);
    token_count++;
}

void add_element_to_save_data(FILE* new_file, Element* selected_element, int index){
    actual_file = new_file;
    if(index !=0){
        fputs(",\n", new_file);
    }
    fputs("{\n", new_file);
    
    new_text_primitive_token("id",selected_element->id);

    switch (selected_element->type)
    {
    case ELEMENT_TYPE_MODEL:
        break;
    case ELEMENT_TYPE_PLAYER_START:
        new_text_primitive_token("type",ELEMENT_TYPE_PLAYER_START);
        break;
    case ELEMENT_TYPE_CAMERA:
        new_text_primitive_token("type",ELEMENT_TYPE_CAMERA);
        break;
    case ELEMENT_TYPE_PLAYER_CONTROLLER:
        new_text_primitive_token("type",ELEMENT_TYPE_PLAYER_CONTROLLER);
        new_text_primitive_token("model_id",player1->model->id);
        break;
    default:
        break;
    }

    new_text_vec3_token("pos",selected_element->position);
    new_text_vec4_token("rot",selected_element->rotation);
    
    if(selected_element->has_HLOD){
        fputs("\t\"elements\" : ", new_file);

    }
    
    if(selected_element->duplicated_of_id > -1){
        new_text_primitive_token("copy",selected_element->duplicated_of_id);
    }else  {
        new_text_token("path",selected_element->model_path);
        new_text_token("texture",selected_element->texture_path);
    }
     
    fputs("}",new_file);
}

void save_level_info(FILE* new_file){
    fputs("{\n\t\"model_count\" : ", new_file);
    fprintf(new_file,"%i",element_id_count+1);
    fputs("\n},\n",new_file);
}

void header_info(){
    new_text_primitive_token("elements",element_id_count+1);
    new_text_primitive_token("type",actual_data_type);
    new_text_primitive_token("tokens",token_count);
    
}
void save_header_info(){
    SaveDataFunction save = &header_info;
    new_save_element(save);
}

void save_camera_editor_camera_transform(FILE* new_file){     
    fputs(",\n", new_file);
    fputs("{\n\t\"type\" : ", new_file);
    fprintf(new_file,"\"%s\",\n","editor_camera");  
    fputs("\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",300);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",camera_position[0] , camera_position[1] , camera_position[2]);
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
    save_header_info();

    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        add_element_to_save_data(new_file,element,i);
    }
    save_camera_editor_camera_transform(new_file);
    fclose(new_file);

    printf("Saved to %s\n",save_name);
}

void save_buttons_data(){
    Button* button = get_element_from_array(actual_buttons_array,0);
    if(button != NULL){
        new_text_vec2_token("pos",button->position);
        new_text_vec2_token("size",button->size);
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
    
    save_header_info();
    
    SaveDataFunction save = &save_buttons_data;
    new_save_element(save);

    fclose(new_file);

    printf("Saved to %s\n",save_name);
}