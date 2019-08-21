#ifndef JSON_WRITER_H
#define JSON_WRITER_H
#include <stdlib.h>

static int token_count = 0;
static int hirarchical_count = 0;
FILE* actual_file;

static inline void hirachical_tab(){
    for(int i = 0; i< hirarchical_count ; i++){
        fputs("\t", actual_file);
    }
}
static inline int add_array_of_numbers(int count){
    
    return 0;
}
static inline void new_text_token(const char* type, const char* value){
    hirachical_tab(); 
    fprintf(actual_file,"\"%s\" : \"%s\",\n",type,value);
    token_count += 2;
}
static inline void  new_text_primitive_token(const char* type, int value){
    hirachical_tab(); 
    fprintf(actual_file,"\"%s\" : %i,\n",type,value);
    token_count += 2;
}

static inline void  new_text_vec4_token(const char* text,vec4 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2], vec[3]);
    token_count += 6;
}

static inline void  new_text_vec3_token(const char* text,vec3 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f , %f],\n", text ,vec[0] , vec[1] , vec[2]);
    token_count += 5;
}

static inline void  new_text_vec2_token(const char* text,vec2 vec){
    hirachical_tab();
    fprintf(actual_file,"\"%s\" : [%f , %f],\n", text ,vec[0] , vec[1]);
    token_count += 4;
}

static inline void  new_array_data_with_pointer(const char* text,void(*function)(void*),void* data){
    hirachical_tab(); fprintf(actual_file,"\"%s\" : [",text);
    hirarchical_count++;
    function(data);
    hirarchical_count--;    
    fputs("],\n",actual_file);
    
}
static inline void  new_array_data(const char* text,void(*function)(void)){
    hirachical_tab(); fprintf(actual_file,"\"%s\" : [\n",text);
    hirarchical_count++;
    function();
    hirarchical_count--;    
    hirachical_tab(); fputs("],\n",actual_file);
    
}

static inline void  new_element(void(*function)(void)){
    hirachical_tab(); fputs("{\n", actual_file);
    hirarchical_count++;
    function();
    hirarchical_count--;
    hirachical_tab(); fputs("},\n",actual_file);
}

static inline void  new_save_element(SaveDataFunction function, int data_id){
    hirachical_tab(); fputs("{\n", actual_file);
    hirarchical_count++;
    function(data_id);
    hirarchical_count--;
    hirachical_tab(); fputs("},\n",actual_file);   
   
    token_count++;
}


#endif