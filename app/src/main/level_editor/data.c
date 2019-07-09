#include "editor.h"
#include "../camera.h"

int add_array_of_numbers(int count){
    
    return 0;
}

void add_element_to_save_data(FILE* new_file, Element* selected_element, int index){
    if(index !=0){
        fputs(",\n", new_file);
    }
    fputs("{\n\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",selected_element->id);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",selected_element->position[0] , selected_element->position[1] , selected_element->position[2]);
    fputs("\t\"rot\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f , %f],\n",selected_element->rotation[0] , selected_element->rotation[1] , selected_element->rotation[2], selected_element->rotation[3]);
    if(selected_element->has_HLOD){
        fputs("\t\"elements\" : ", new_file);

    }
    if(selected_element->duplicated_of_id > -1){
        fputs("\t\"copy\" : ", new_file);
        fprintf(new_file,"%i\n",selected_element->duplicated_of_id);
    }else{
        fputs("\t\"path\" : ", new_file);
        fprintf(new_file,"\"%s\",\n",selected_element->model_path);
        fputs("\t\"texture\" : ", new_file);
        fprintf(new_file,"\"%s\"\n",selected_element->texture_path);
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

    create_save_data_backup();
    FILE* new_file = fopen(name,"w+");
    save_level_info(new_file);   
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        add_element_to_save_data(new_file,element,i);
    }
    save_camera_editor_camera_transform(new_file);
    fclose(new_file);

}