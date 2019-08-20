#include "commands.h"
#include "editor.h"


void system_command(const char* command, const char* argument){
    char final_command[strlen(command) + strlen(argument)];
    memset(final_command,0,strlen(final_command));
    strcat(final_command,command);
    strcat(final_command,argument);

    if(fork() == 0){
        system(final_command);
        exit(0);
    }
}



static size_t code_to_utf8(unsigned char *const buffer, const unsigned int code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}


bool activate_text_input_mode = false;


char first_char_command;
char* command_array_pointer;
char* argument_array_pointer;

void parse_command_with_len(const char* command, int len){
    
}

void parse_commmand_with_one_parameter(const char* command, float parameter){
    if(strcmp(command,"cluster") == 0){
        
        add_editor_native_element("HLOD Cluster");
        update_scale(VEC3(parameter,parameter,parameter));

        
        return;
    }

}

void command_while_not_space(const char* command){
    int command_len = strlen(command);
    for(int i = 0; i<command_len; i++){
        if(command[i] == ' '){
            char new_command_text[i];
            memcpy(new_command_text,command,i);
            LOG("%s\n",new_command_text);
            float parameter = atof(&command[i+1]);
            parse_commmand_with_one_parameter(new_command_text,parameter);
        }
    }
}

void parse_command(const char* command){
    first_char_command = command[1];

    if(strcmp(&command[1],"gui") == 0){
        change_to_editor_mode(EDITOR_MODE_GUI_EDITOR);
        return;
    }

    if(strcmp(&command[1],"add cube") == 0){
        editor_generate_and_add_cube_element();
        return;
    }
    command_while_not_space(&command[1]);

    
    if(strcmp(&command[1],"hlod") == 0){
        generate_HLODS(false);
    }
    if(strcmp(&command[1],"hlode") == 0){
        generate_HLODS(true);
        gizmos_can_draw_spheres = false;
        return;
    }

    switch (first_char_command)
    {
    case 'w':
    {
        switch (editor_mode)
        {
        case EDITOR_MODE_GUI_EDITOR:
            save_gui_data(&command[3]);
            LOG("GUI saved: %s\n",&command[3]);
            return;
        case EDITOR_DEFAULT_MODE:
            save_level_data(&command[3]);
            LOG("Level saved: %s\n",&command[3]);
            return;
        default:
            break;
        }
    }
    case 'o':
    {
        switch (editor_mode)
        {
        case EDITOR_MODE_GUI_EDITOR:
            load_gui(&command[3]);       
            LOG("GUI loaded\n");
            return;
            
        case EDITOR_DEFAULT_MODE:
            editor_load_level(&command[3]);
            LOG("Level loaded\n");
            return;
            
        default:
            break;
        }
    }
    case 'r':{
        reload_editor();
        LOG("reload\n");
        return;
    }
    break;
    case 'u':{
        isDrawUV = true;
        break;
    }
    case 'e':
    {
        data_export_select_element("/home/pavon/PavonTheGame/exports/export.gltf");
        //system("blender --python ../scripts/Blender/import.py");
        //reload_editor();
        //add_element_with_model_path("../assets/HLOD/out.gltf");
        return;
    }
    case 'a':
    {
        editor_add_element_with_model_path(&command[3]);
        return;
    }
    case 'q':{
        exit(0);
        return;
    }
    case 'g':{

        return;
    }
    case 's':
        {
            if(command[2] == 'z'){
                mat4 idenity;
                glm_mat4_identity(idenity);
                glm_mat4_copy(idenity,selected_element->transform->model_matrix);
                update_translation(VEC3(0,0,0));
                return;
            }

            if(command[3] == 'p'){//player
                player1 = selected_element;            
                add_editor_native_element("Player Controller");
                return;
            }

            if(command[2] == 'n'){
                switch (editor_mode)
                {
                case EDITOR_MODE_GUI_EDITOR:
                    strcpy(selected_button->name,&command[4]);
                    return;
                    
                case EDITOR_DEFAULT_MODE:
                    strcpy(selected_element->name,&command[4]);
                    return;
                    
                default:
                    break;
                }
                
            }
        }
    case 'd':
    {
        unsigned int duplicate_count = atoi(&command[3]); 
        int duplicate_offset = atoi(&command[5]);
        Element* original = selected_element;
        for(int i = 0; i< duplicate_count; i++){            
            duplicate_selected_element(i,original);      
            vec3 move = {duplicate_offset,0,0};
            update_translation(move);            
        }      
        LOG("duplicated %i\n",duplicate_count);
    }
    default:
        break;
    }
    

}

#include "Windows/content_browser.h"
void parse_characters(unsigned char character){
    if(character == ':'){
        LOG("Command mode\n");
        command_text_buffer[command_character_count] = character;
        command_character_count++;        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_TEXT_INPUT);
        return;
    }else if(character == '/'){
        if(editor_sub_mode == EDITOR_SUB_MODE_NULL)
        { 
            LOG("Search mode\n");
            command_text_buffer[command_character_count] = character;
            command_character_count++;
            editor_search_objects = true;
            change_to_editor_sub_mode(EDITOR_SUB_MODE_TEXT_INPUT);
            return;
        }
    }

    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT){
        command_text_buffer[command_character_count] = character;
        command_character_count++;
    }

    if(editor_window_content_browser_hint && window_content_browser.focus){
        command_text_buffer[command_character_count] = character;
        command_character_count++;
    }
}

int log_command_offset = 0;
void text_input_mode(){
    if(key_released(&input.ENTER)){
        parse_command(command_text_buffer);
        command_character_count = 0;
        save_commnad_history(command_text_buffer);
        memset(command_text_buffer,0,sizeof(command_text_buffer));
        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_NULL);
        return;
    }
    if(key_released(&input.BACKSPACE)){
        command_character_count--;
        command_text_buffer[command_character_count] = '\0';
    }
    if(key_released(&input.KEY_UP)){
       
        FILE* file = fopen("/home/pavon/sources/PavonEngineC/Binaries/command_history.txt","r");
        if(!file)
            return;

        fseek(file, 0, SEEK_END);
     
        static const long max_len = 55 + 1;

        char buf[max_len + 1];

        /* now read that many bytes from the end of the file */
        fseek(file, -max_len, SEEK_END);
        int len = fread(buf,1, 55, file);

        buf[len] = '\0';

        /* and find the last newline character (there must be one, right?) */
        char *last_newline = strrchr(buf, '\n');
        char *last_line = last_newline+1;

        LOG("Last line: %s\n",last_line);
        memcpy(&command_text_buffer[command_character_count],last_line,strlen(last_line));

    }

}

void character_callback(GLFWwindow* window, unsigned int codepoint){
    if(codepoint == 241)//equal "ñ"
            return;
    unsigned char character[1];
    code_to_utf8(character,codepoint);
    //LOG("Converted: %s\n",character);
    parse_characters(character[0]);
}

