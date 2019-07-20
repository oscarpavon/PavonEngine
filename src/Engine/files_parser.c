#include "files_parser.h"

#define JSMN_HEADER
#include "../third_party/jsmn.h"
#include "../engine.h"
#include "../level_editor/data.h"

typedef enum DumpArrayType{
  DUMP_ARRAY_TYPE_NULL,
  DUMP_ARRAY_TYPE_INT,
  DUMP_ARRAY_TYPE_COMPONENT,
  DUMP_ARRAY_TYPE_TEXT
}DumpArrayType;

int header_size = 0;
DataType actual_data_type;
unsigned int elements_count = 0;

bool parse_componentes = false;

int element_id = 0;
int button_id = 0;
int previos_id = -1;

bool object_zero = true;

struct Element* actual_element;
Button* actual_button;
struct Element* _elements_array;
Array* actual_array;
const char* actual_json_file;


DumpArrayType dump_array_type = DUMP_ARRAY_TYPE_NULL;
ComponentType current_component_type;

static inline int get_token_primitive_value(jsmntok_t *token);

static int string_equal(jsmntok_t *tok, const char *s) {
  
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(actual_json_file + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

static inline size_t get_token_size(jsmntok_t *t){
  size_t length = t->end - t->start;
  return length;
}

static int dump_array(jsmntok_t* token, int count){
  switch (dump_array_type)
  {
  case DUMP_ARRAY_TYPE_COMPONENT:
    {

    }
    break;
  
  default:
    break;
  }
}

static int dump_int(jsmntok_t *next_t, jsmntok_t *object_t, size_t count, int indent, Element* model) {

    if (string_equal(object_t, "id") == 0) {
        size_t size = get_token_size(next_t);
        char text[size+1];
        memcpy(&text,&actual_json_file[next_t->start],size);
        text[size] = '\0';
        unsigned int model_id = atoi(text);
        memcpy(&model->id, &model_id, sizeof(unsigned int));
      }
}


static int dump_array_ints(jsmntok_t *token, size_t count, int index, float* firt_array_element) {
	
	if (token->type == JSMN_PRIMITIVE) {
    float value = get_token_primitive_value(token);
    firt_array_element += index;
    memcpy(firt_array_element, &value, sizeof(float));

		return 1;
	}
}



static int dump(jsmntok_t *token, size_t count, int indent);

static inline int get_token_primitive_value(jsmntok_t *token){
  int size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';
  unsigned int result = atoi(text);
  return result;
}

static inline float get_token_primitive_float(jsmntok_t *token){
  int size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';
  float result = atof(text);
  return result;
}

static inline void get_token_string(char* out, jsmntok_t* token){
  size_t size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';      
  strcpy(out, text);   
}


int actual_element_components_count = 0;
bool actual_element_component_parsed = false;

void parse_components(jsmntok_t* token){

    if(dump_array_type == DUMP_ARRAY_TYPE_COMPONENT){
      
            
      if (string_equal( token, "type") == 0){ 
        unsigned int value =  get_token_primitive_value(token+1);
        switch (value)
        {
        case TRASNFORM_COMPONENT:
          {
            current_component_type = TRASNFORM_COMPONENT;
            TransformComponent transform;
            init_transfrom_component(&transform);
            add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);

          }
          break;
        case STATIC_MESH_COMPONENT:{
            current_component_type = STATIC_MESH_COMPONENT;
            StaticMeshComponent mesh_component;
            mesh_component.model = NULL;
            add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);
          }
          break;
        default:
          break;
        }
      }


    }

    if(dump_array_type == DUMP_ARRAY_TYPE_TEXT){
      if (string_equal( token, "paths") == 0){
        
      }
    }

    if(current_component_type == TRASNFORM_COMPONENT){
      
      if (string_equal( token, "position") == 0){
        vec3 pos;
        jsmntok_t* array_value_token = token+2;

        for (int i = 0; i < (token+1)->size; i++) {   
          float value = get_token_primitive_float(array_value_token+i);         
          memcpy(&pos[i],&value,sizeof(float));               
        }

        TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
        glm_vec3_copy(pos,transform->position);
      }
    }
}

void parse_level_token(jsmntok_t* token){
  if (string_equal(token, "data") == 0) {
    int i = 0;
  }
  if (string_equal(token, "paths") == 0) {
    jsmntok_t* array_value_token = token+2;
    for (int i = 0; i < (token+1)->size; i++) {   
      char text[20];
      get_token_string(text,array_value_token+i);
      add_to_array(&texts,text);
    }
  }

  if(current_component_type == STATIC_MESH_COMPONENT){
    if (string_equal( token, "path") == 0) {
      int path_id = get_token_primitive_value(token+1);
      StaticMeshComponent* mesh = get_component_from_selected_element(STATIC_MESH_COMPONENT);
      mesh->model_id = path_id;
    }
  }

  if (string_equal( token, "name") == 0) {     
      char text[20];
      get_token_string(text,token+1);      
      strcpy(actual_element->name, text);
      actual_element_component_parsed = false;
  }

  if (string_equal( token, "components_count") == 0) {    
    actual_element_components_count =  get_token_primitive_value(token+1); 
    if(selected_element->components.initialized==false)    
      init_array(&selected_element->components,sizeof(ComponentDefinition),actual_element_components_count);
  }

  if (string_equal( token, "components") == 0){ 
    if(actual_element_component_parsed == true)
      return;
    for(int i = 0; i < actual_element_components_count ; i++){
      parse_componentes = true;
      //dump(token+1,(token+1)->size,0);
      
    }
    
  }

  
  parse_components(token);
   
}

static int dump(jsmntok_t *token, size_t count, int indent) {
	int i, j;
  actual_element = &_elements_array[element_id];

  selected_element = &_elements_array[element_id];

	if (count == 0) {
		return 0;
	}

	if (token->type == JSMN_PRIMITIVE) {
    if (string_equal( token-1, "elements") == 0){
      elements_count = get_token_primitive_value(token);
      LOG("Elements from header count: %i\n",elements_count);
      
    }

    if (string_equal( token-1, "function_id") == 0){
      actual_button->action_function_id = get_token_primitive_value(token);      
    }
    
    if (string_equal( token-1, "type") == 0){     
      
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

    if (string_equal( token-1, "id") == 0){       
      actual_element->id = get_token_primitive_value(token);
    }

  

		return 1;
	} 
  else if (token->type == JSMN_STRING) { 

    if(actual_data_type == DATA_TYPE_HEADER){
      if (string_equal( token, "type") == 0){ 
          unsigned int type = get_token_primitive_value(token+1);
          switch (type)
          {
          case DATA_TYPE_LEVEL:
            actual_data_type = type;
            LOG("Header Type LEVEL\n");
            break;
          case DATA_TYPE_GUI:
            actual_data_type = type;
            LOG("Header Type GUI\n");
          default:
            break;
          } 
      }
      return 1;
    }

    if(actual_data_type == DATA_TYPE_GUI){
      if (string_equal( token, "name") == 0) {  
        char text[20];
        get_token_string(text,token+1);      
        strcpy(actual_button->name, text);
      }
    }
    
    if(actual_data_type == DATA_TYPE_LEVEL)
      parse_level_token(token);

		return 1;
	} 
  else if (token->type == JSMN_OBJECT) {

    if(actual_data_type == DATA_TYPE_GUI){
      if(previos_id != button_id){
        Button new_button;
        memset(&new_button,0,sizeof(Button));
        add_to_array(actual_array,&new_button);
        previos_id = button_id;
        actual_button = get_from_array(actual_array,button_id);      
      }
      
    }
    if(actual_data_type == DATA_TYPE_GUI){
       if(object_zero == true){
        object_zero = false;
      }else
        button_id++;
    }   
  
		j = 0;

		for (i = 0; i <= token->size; i++) {
    
    	j += dump( token+1+j, count-j, indent+1); 

		}
    

		return j+1;
	} 
  else if (token->type == JSMN_ARRAY) {
      j = 0;
      int array_element_index = 0;  
      
      
      if(actual_data_type == DATA_TYPE_GUI){
        if (string_equal( token-1, "pos") == 0) {
            vec2 pos;
            for (i = 0; i < token->size; i++) {   

              dump_array_ints( token+1+j, count-j, array_element_index,pos);
              array_element_index++;               

              j += dump( token+1+j, count-j, indent+1);                 

            }
            actual_button->position[0] = pos[0];
            actual_button->position[1] = pos[1];
                     
        }
        if (string_equal( token-1, "size") == 0) {
            vec2 size;
            for (i = 0; i < token->size; i++) {   

              dump_array_ints( token+1+j, count-j, array_element_index,size);
              array_element_index++;               

              j += dump( token+1+j, count-j, indent+1);                 

            }
            actual_button->size[0] = size[0];
            actual_button->size[1] = size[1];
        }
      }

      if(actual_data_type == DATA_TYPE_LEVEL){
        if (string_equal( token-1, "components") == 0){
          dump_array_type = DUMP_ARRAY_TYPE_COMPONENT;
        }
        if (string_equal( token-1, "paths") == 0){
          dump_array_type = DUMP_ARRAY_TYPE_TEXT;
        }
      }

      for (i = 0; i <= token->size; i++) {             

        j += dump( token+1+j, count-j, indent+1);     

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
  LOG("Json Header size: %i\n",header_size);

  jsmn_parser parser;
  jsmn_init(&parser);

  jsmntok_t header_tokens[20];

  int token_parsed = jsmn_parse(&parser,json_file,header_size,header_tokens,20);
  LOG("Header token parsed: %d\n",token_parsed);
  dump(header_tokens,token_parsed,0);

  return 0;
}

void parse_file(){

}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
  parse_header(json_file);
  int token_count = elements_count *(2+3+4+2+2+1+1);//id + id_num + pos + array + 3 pos + rot + 4 rot + path + textur + textur + model path  
  
  jsmn_parser parser;
  int max_tokens = token_count+30;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);

  char* models_json_file = allocate_memory(json_file_size-header_size);
  memset(models_json_file,0,json_file_size-header_size);
  models_json_file[json_file_size-header_size] = '\0';

  memcpy(models_json_file,json_file+header_size,json_file_size-header_size);

  //LOG("%s\n", models_json_file);

  actual_json_file = models_json_file;

  int number_of_tokens_readed = jsmn_parse(&parser , models_json_file , json_file_size-header_size , tokens,max_tokens);

  Element elements[elements_count];
  memset(elements,0,sizeof(elements));
  _elements_array = &elements[0];


  init_array(out_element,sizeof(Element),elements_count);   

  dump(tokens,number_of_tokens_readed,0);

  
  for(int i = 0; i < elements_count; i++){
    add_to_array(out_element,&elements[i]);
  }

  //free(models_json_file);
  LOG("json level parsed\n");
}

void parse_gui_file(const char* json_file, size_t json_file_size, struct Array* out_element){
  parse_header(json_file);

  int token_count = elements_count * 8;

  jsmn_parser parser;
  int max_tokens = token_count+30;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);

  char* models_json_file = malloc(json_file_size-header_size);
  memset(models_json_file,0,json_file_size-header_size);
  models_json_file[json_file_size-header_size] = '\0';

  memcpy(models_json_file,json_file+header_size,json_file_size-header_size);

  LOG("%s\n", models_json_file);

  actual_json_file = models_json_file;

  int number_of_tokens_readed = jsmn_parse(&parser , models_json_file , json_file_size-header_size , tokens,max_tokens);

  actual_array = out_element;

  dump(tokens,number_of_tokens_readed,0);

}