#include "files_parser.h"

#define JSMN_HEADER
#include "../third_party/jsmn.h"
#include "../engine.h"
#include "../level_editor/data.h"

int header_size = 0;
DataType actual_data_type;
unsigned int elements_count = 0;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

static inline size_t get_token_size(jsmntok_t *t){
  size_t length = t->end - t->start;
  return length;
}

static int dump_int(const char *js, jsmntok_t *next_t, jsmntok_t *object_t, size_t count, int indent, Element* model) {

    if (jsoneq(js, object_t, "id") == 0) {
        size_t size = get_token_size(next_t);
        char text[size+1];
        memcpy(&text,&js[next_t->start],size);
        text[size] = '\0';
        unsigned int model_id = atoi(text);
        memcpy(&model->id, &model_id, sizeof(unsigned int));
      }
}

static int dump_array(const char *js, jsmntok_t *t, size_t count, int index, float* firt_array_element) {
	
	if (t->type == JSMN_PRIMITIVE) {
    size_t size = get_token_size(t);
    char text[size+1];
    memcpy(&text,&js[t->start],size);
    text[size] = '\0';
    float model_id = atof(text);
    firt_array_element += index;
    memcpy(firt_array_element, &model_id, sizeof(float));

		return 1;
	}
}

int element_id = 0;

struct Element* actual_element;
struct Element* _elements_array;
const char* actual_json_file;

int get_token_primitive_value(jsmntok_t *token){
  int size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';
  unsigned int result = atoi(text);
  return result;
}
static int dump(const char *js, jsmntok_t *token, size_t count, int indent) {
	int i, j;
  actual_element = &_elements_array[element_id];
  actual_json_file = js;

	if (count == 0) {
		return 0;
	}

	if (token->type == JSMN_PRIMITIVE) {
    if (jsoneq(js, token-1, "elements") == 0){
      elements_count = get_token_primitive_value(token);
    }
    if(actual_data_type == DATA_TYPE_HEADER){
      if (jsoneq(js, token-1, "type") == 0){ 
          unsigned int type = get_token_primitive_value(token);
          switch (type)
          {
          case DATA_TYPE_LEVEL:
            actual_data_type = type;
            break;
          case DATA_TYPE_GUI:
            actual_data_type = type;
          default:
            break;
          } 
      }
    }
    if (jsoneq(js, token-1, "type") == 0){     
      
      switch (get_token_primitive_value(token))
      {
      case ELEMENT_TYPE_PLAYER_START:
        actual_element->type = ELEMENT_TYPE_PLAYER_START;
        break;
      case ELEMENT_TYPE_PLAYER_CONTROLLER:
        actual_element->type = ELEMENT_TYPE_PLAYER_CONTROLLER;
      default:
        break;
      }

      
    }
    if (jsoneq(js, token-1, "model_id") == 0){
      actual_element->model_id = get_token_primitive_value(token);
    }

    if (jsoneq(js, token-1, "id") == 0){       
      actual_element->id = get_token_primitive_value(token);
    }

    if (jsoneq(js, token-1, "copy") == 0){
      actual_element->duplicated_of_id = get_token_primitive_value(token);
      element_id++;
    }

		return 1;
	} 
  else if (token->type == JSMN_STRING) {    

    if (jsoneq(js, token, "path") == 0) {
      token = token+1;
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';      
      strcpy(actual_element->model_path, text);    
    }

    if (jsoneq(js, token, "texture") == 0){
      token = token+1;
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';      
      strcpy(actual_element->texture_path, text); 
      
      element_id++;        
             
    }

		return 1;
	} 
  else if (token->type == JSMN_OBJECT) {  
   
		j = 0;

		for (i = 0; i < token->size; i++) {
    
    	j += dump(js, token+1+j, count-j, indent+1); 

		}
    
		return j+1;
	} 
  else if (token->type == JSMN_ARRAY) {
      j = 0;
      int array_element_index = 0;
      
      if (jsoneq(js, token-1, "pos") == 0) {
        vec3 pos;
        for (i = 0; i < token->size; i++) {   

          dump_array(js, token+1+j, count-j, array_element_index,pos);
          array_element_index++;               

          j += dump(js, token+1+j, count-j, indent+1);                 

        }
        glm_vec3_copy(pos,actual_element->position);
                    
      }
      if (jsoneq(js, token-1, "rot") == 0) {
        vec4 quat;
        for (i = 0; i < token->size; i++) {   

          dump_array(js, token+1+j, count-j, array_element_index,quat);
          array_element_index++;               

          j += dump(js, token+1+j, count-j, indent+1);                 

        }
        glm_vec4_copy(quat,actual_element->rotation);
      }
      for (i = 0; i < token->size; i++) {             

        j += dump(js, token+1+j, count-j, indent+1);     

      }      
            
      return j+1;
	}
	return 0;
}


int parse_header(const char* json_file){
  actual_json_file = json_file;
  actual_data_type = DATA_TYPE_HEADER;
  for(int i = 0; i < 100 ; i++){
    if(json_file[i] == '}'){
      break;
    }
    header_size++;
  }
  header_size += 2;
  printf("Json Header size: %i\n",header_size);

  jsmn_parser parser;
  jsmn_init(&parser);

  jsmntok_t header_tokens[20];

  int token_parsed = jsmn_parse(&parser,json_file,header_size,header_tokens,20);
  
  dump(actual_json_file,header_tokens,token_parsed,0);

  return 0;
}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
  parse_header(json_file);
  int token_count = elements_count *(2+3+4+2+2+1+1);//id + id_num + pos + array + 3 pos + rot + 4 rot + path + textur + textur + model path  
  
  jsmn_parser parser;
  int max_tokens = token_count+30;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);
  


  char* models_json_file = malloc(json_file_size-header_size);
  memset(models_json_file,0,json_file_size-header_size);
  models_json_file[json_file_size-header_size] = '\0';

  memcpy(models_json_file,json_file+header_size,json_file_size-header_size);

  //printf("%s\n", models_json_file);

  actual_json_file = models_json_file;
  Element elements[elements_count];
  memset(elements,0,sizeof(elements));
  _elements_array = &elements[0];
  for(int i = 0; i<elements_count;i++){
      elements[i].duplicated_of_id = -1;
  }

  int number_of_tokens_readed = jsmn_parse(&parser , models_json_file , json_file_size-header_size , tokens,max_tokens);

  init_array(out_element,sizeof(Element));   

  dump(models_json_file,tokens,number_of_tokens_readed,0);

  
  for(int i = 0; i < elements_count; i++){
    add_element_to_array(out_element,&elements[i]);
  }
 

  int o = 2;
  free(models_json_file);
  printf("json level parsed\n");
}

void parse_gui_file(const char* json_file, size_t json_file_size, struct Array* out_element){
    parse_header(json_file);
}