#include "editor.h"
#include "../camera.h"

FILE* actual_file;
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

void add_element_to_save_data(FILE* new_file, Element* selected_element, int index){
    actual_file = new_file;
    if(index !=0){
        fputs(",\n", new_file);
    }
    fputs("{\n", new_file);
    
    new_text_primitive_token("id",selected_element->id);

    switch (selected_element->type)
    {
    case ELEMENT_TYPE_PLAYER_START:
        new_text_primitive_token("type",ELEMENT_TYPE_PLAYER_START);
        break;
    case ELEMENT_TYPE_CAMERA:
        new_text_primitive_token("type",ELEMENT_TYPE_CAMERA);
        break;
    default:
        break;
    }
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",selected_element->position[0] , selected_element->position[1] , selected_element->position[2]);
    fputs("\t\"rot\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f , %f],\n",selected_element->rotation[0] , selected_element->rotation[1] , selected_element->rotation[2], selected_element->rotation[3]);
    
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
void save_data(const char* name){   
    char* level_folder = "../Game/levels/";
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, level_folder);
    strcat(save_name,name);
    strcat(save_name,".lvl");
    printf("%s\n",save_name);

    create_save_data_backup();
    FILE* new_file = fopen(save_name,"w+");
    save_level_info(new_file);   
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        add_element_to_save_data(new_file,element,i);
    }
    save_camera_editor_camera_transform(new_file);
    fclose(new_file);

}