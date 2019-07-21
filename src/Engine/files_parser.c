#include "files_parser.h"

#define JSMN_HEADER
#include "../third_party/jsmn.h"
#include "../engine.h"
#include "../level_editor/data.h"

typedef jsmntok_t Token;


int header_size = 0;
DataType actual_data_type;
unsigned int elements_count = 0;


int element_id = 0;
int button_id = 0;
int previos_id = -1;


bool object_zero = true;

struct Element* actual_element;
Button* actual_button;
struct Element* _elements_array;
Array* actual_array;
const char* actual_json_file;


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

jsmntok_t* get_token_array_component_transform(jsmntok_t* token){
  if( string_equal(token,"position") != 0){
    LOG("No token \"position\" string passed\n");
    return;
  }
  vec3 pos;
  jsmntok_t* array_token = token+1;
  jsmntok_t* array_value_token = token+2;
  jsmntok_t* last_token_readed = NULL;
  for (int i = 0; i < array_token->size; i++) {
    last_token_readed = array_value_token+i;   
    float value = get_token_primitive_float(array_value_token+i);         
    memcpy(&pos[i],&value,sizeof(float));               
  }

  last_token_readed += 1;
  if( string_equal(last_token_readed,"rotation") != 0){
      LOG("No token \"rotation\" string passed\n");
      return;
  }
  vec4 rot;
  array_token = last_token_readed+1;
  array_value_token = array_token+1;
  for (int i = 0; i < array_token->size; i++) {
    last_token_readed = array_value_token+i;   
    float value = get_token_primitive_float(array_value_token+i);         
    memcpy(&rot[i],&value,sizeof(float));               
  }  


  TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
  glm_vec3_copy(pos,transform->position);
  glm_vec4_copy(rot,transform->rotation);

  return last_token_readed;
}

ComponentType parse_component_type(jsmntok_t* type_token){
  if( type_token->type != JSMN_PRIMITIVE)
    LOG("Get type is not a primitive value\n");
  int type = get_token_primitive_value(type_token);
  switch (type)
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
  return current_component_type;
}
jsmntok_t * fill_components_values(ComponentType type, jsmntok_t* token_value_name_string){
  jsmntok_t * last_element_readed;
  switch (type)
  {
  case TRASNFORM_COMPONENT:{
    last_element_readed = get_token_array_component_transform(token_value_name_string);
    return last_element_readed;
    }    
  case STATIC_MESH_COMPONENT:
    LOG("Static mesh component\n");
    token_value_name_string++;
    last_element_readed = token_value_name_string;
    int mesh_path_id = get_token_primitive_value(token_value_name_string);
    StaticMeshComponent* mesh = get_component_from_selected_element(STATIC_MESH_COMPONENT);
    mesh->model_id = mesh_path_id;
    return last_element_readed;
  break;
  }
}

jsmntok_t* parse_component_object_token(jsmntok_t* object_token_component){
  jsmntok_t * last_element_readed = object_token_component;
  for(int i= 0; i< object_token_component->size ; i++){
    jsmntok_t* token_primitive_type;
    if(last_element_readed->type == JSMN_OBJECT){
      token_primitive_type = last_element_readed+2;
      object_token_component = last_element_readed;
    }      
    else if(last_element_readed->type == JSMN_PRIMITIVE){
      if((last_element_readed+1)->type == JSMN_OBJECT)
        object_token_component = last_element_readed+1;
      token_primitive_type = last_element_readed+3;
    }
      

    ComponentType type = parse_component_type(token_primitive_type);
    last_element_readed = fill_components_values(type,object_token_component+3);    
  }
  
  return last_element_readed;
}
jsmntok_t* get_components_from_token(jsmntok_t* components_array){
  int components_counts = components_array->size;
  init_array(&selected_element->components,sizeof(ComponentDefinition),components_counts);
  jsmntok_t* last_element_parsed = parse_component_object_token(components_array+1);
  return last_element_parsed;
}
//return last token readed
jsmntok_t* parse_element_object_token(jsmntok_t* object_token){
    jsmntok_t* name_token = object_token+1;
    if( name_token->type == JSMN_STRING ){
      if( string_equal(name_token,"name") == 0 ){
        char name[20];
        get_token_string(name,name_token+1);
        LOG("Name: %s\n",name);
        strcpy(selected_element->name,name);
        jsmntok_t* last_element_parsed = get_components_from_token(name_token+3);
        return last_element_parsed;
      }else{
        char text[20];
        get_token_string(text,name_token);
        LOG("Found: %s\n",text);
      }
    }else{
      LOG("token name not found\n");
    }
}

jsmntok_t* get_next_object_token(jsmntok_t* token){
  while(token->type != JSMN_OBJECT){
       token += 1;
  } 
  return token;
}

jsmntok_t* parse_elements(jsmntok_t* array_element_token){
  
  int elements_counts = array_element_token->size;
 
  jsmntok_t* last_element_parsed = array_element_token;
  for(int i = 0; i< elements_counts ; i++){
    selected_element = &_elements_array[element_id];
    last_element_parsed = get_next_object_token(last_element_parsed);
    last_element_parsed = parse_element_object_token(last_element_parsed);
    element_id++;
  }
  return last_element_parsed;
}

void parse_actual_file(jsmntok_t* tokens, int count){
  jsmntok_t* last_element_parsed = NULL;
  for(int i = 0; i< count ; i++){
    jsmntok_t* actual_token = &tokens[i];
    if(actual_token->type == JSMN_STRING){
      if( string_equal(actual_token,"elements") == 0){
        last_element_parsed =  parse_elements(actual_token+1);
        break;
      }
    }
  }

  Token* new_object = get_next_object_token(last_element_parsed);
  for(int i = 0; i< count ; i++){
    Token* actual_token = &tokens[i];
    if(actual_token->type == JSMN_STRING){
        if (string_equal(actual_token, "paths") == 0) {
          Token* array_value_token = actual_token+2;
          for (int i = 0; i < (actual_token+1)->size; i++) {   
            char text[20];
            get_token_string(text,array_value_token+i);
            add_to_array(&texts,text);
          }
        }

    }

  }
  

}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
 
  int token_count = 20 *(5+5+30);
  
  jsmn_parser parser;
  int max_tokens = token_count+30;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);
 
  actual_json_file = json_file;

  int parse_result = jsmn_parse(&parser , actual_json_file , json_file_size-header_size , tokens,max_tokens);
  switch (parse_result)
  {
  case JSMN_ERROR_INVAL:
    LOG("ERROR in parse json file ERROR_INVAL\n");
    break;
  case JSMN_ERROR_NOMEM:
    LOG("ERROR in parse json file ERROR_NOMEM\n");
    break;
  case JSMN_ERROR_PART:
    LOG("ERROR in parse json file ERROR_PART\n");
    break;
  
  default:
    break;
  }
  Element elements[elements_count];
  memset(elements,0,sizeof(elements));
  _elements_array = &elements[0];


  init_array(out_element,sizeof(Element),elements_count);   

  parse_actual_file(tokens,parse_result);
  
  for(int i = 0; i < elements_count; i++){
    add_to_array(out_element,&elements[i]);
  }

  LOG("json level parsed\n");
}

