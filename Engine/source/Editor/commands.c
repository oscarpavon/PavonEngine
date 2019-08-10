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
unsigned short int character_count = 0;

char first_char_command;
char* command_array_pointer;
char* argument_array_pointer;

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

    if(strcmp(&command[1],"hlod") == 0){
        editor_add_HLOD_element();
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
            load_level_in_editor(&command[3]);
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
    case 'e':
    {
        export_gltf("../assets/HLOD/out.gltf");
        system("blender --python ../scripts/Blender/import.py");
        reload_editor();
        add_element_with_model_path("../assets/HLOD/out.gltf");
        return;
    }
    case 'a':
    {
        add_element_with_model_path(&command[3]);
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
            if(command[3] == 'p'){//player
                player1 = selected_element;            
                add_editor_native_element("Player Controller");
            }
            return;

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
        for(int i = 0; i< duplicate_count; i++){
            duplicate_selected_element();        
            vec3 move = {duplicate_offset,0,0};
            update_translation(move);
        }      
        LOG("duplicated %i\n",duplicate_count);
    }
    default:
        break;
    }
    

}

void parse_characters(unsigned char character){
    if(character == ':'){
        LOG("Command mode\n");
        command_text_buffer[character_count] = character;
        character_count++;        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_TEXT_INPUT);
        return;
    }
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT){
        command_text_buffer[character_count] = character;
        character_count++;
    }
}

int log_command_offset = 0;
void text_input_mode(){
    if(key_released(&input.ENTER)){
        parse_command(command_text_buffer);
        character_count = 0;
        save_commnad_history(command_text_buffer);
        memset(command_text_buffer,0,sizeof(command_text_buffer));
        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_NULL);
        return;
    }
    if(key_released(&input.BACKSPACE)){
        character_count--;
        command_text_buffer[character_count] = '\0';
    }
    if(key_released(&input.KEY_UP)){
        File file;
        load_file("../binaries/command_history.txt",&file);
        const char* first = file.data;
        
        int log_character_count = 0;
        for(int i = 0; i<(file.size_in_bytes-log_command_offset+1) ; i++){
            const char* character = &first[i+log_command_offset];           

            if(*character == '\n' || (*character == '\0' ) ){
                log_character_count++;
                memcpy(&command_text_buffer[character_count],&first[log_command_offset],log_character_count-1);
                log_command_offset = log_character_count;
                break;
            }else{
                log_character_count++;                   
            }                
           
        
        }
       // strcpy(&command_text_buffer[character_count],file.data);  
        close_file(&file);

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

