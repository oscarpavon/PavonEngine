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

const char* actual_json_file;

ComponentType current_component_type;

static inline int get_token_primitive_value(Token *token);

static inline Token* get_next_object_token(Token* token){
  while(token->type != JSMN_OBJECT){
       token += 1;
  } 
  return token;
}
static inline Token* get_next_array_token(Token* token){
  while(token->type != JSMN_ARRAY){
       token += 1;
  } 
  return token;
}


static int string_equal(Token *tok, const char *s) {
  
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(actual_json_file + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

static inline size_t get_token_size(Token *t){
  size_t length = t->end - t->start;
  return length;
}


static int dump_int(Token *next_t, Token *object_t, size_t count, int indent, Element* model) {

    if (string_equal(object_t, "id") == 0) {
        size_t size = get_token_size(next_t);
        char text[size+1];
        memcpy(&text,&actual_json_file[next_t->start],size);
        text[size] = '\0';
        unsigned int model_id = atoi(text);
        memcpy(&model->id, &model_id, sizeof(unsigned int));
      }
}

static int dump_array_ints(Token *token, size_t count, int index, float* firt_array_element) {
	
	if (token->type == JSMN_PRIMITIVE) {
    float value = get_token_primitive_value(token);
    firt_array_element += index;
    memcpy(firt_array_element, &value, sizeof(float));

		return 1;
	}
}

static inline int get_token_primitive_value(Token *token){
  int size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';
  unsigned int result = atoi(text);
  return result;
}

static inline float get_token_primitive_float(Token *token){
  int size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';
  float result = atof(text);
  return result;
}

static inline void get_token_string(char* out, Token* token){
  size_t size = get_token_size(token);
  char text[size+1];
  memcpy(&text,&actual_json_file[token->start],size);
  text[size] = '\0';      
  strcpy(out, text);   
}

Token* get_token_array_component_transform(Token* token){
  if( string_equal(token,"position") != 0){
    LOG("No token \"position\" string passed\n");
    return;
  }
  vec3 pos;
  Token* array_token = token+1;
  Token* array_value_token = token+2;
  Token* last_token_readed = NULL;
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

ComponentType parse_component_type(Token* type_token){
  if( type_token->type != JSMN_PRIMITIVE)
    LOG("Get type is not a primitive value\n");
  int type = get_token_primitive_value(type_token);
  switch (type)
  {
  case TRASNFORM_COMPONENT:
    {
      current_component_type = TRASNFORM_COMPONENT;
      add_transform_component_to_selected_element();
    }
    break;
  case STATIC_MESH_COMPONENT:{
      current_component_type = STATIC_MESH_COMPONENT;
      StaticMeshComponent mesh_component;
      mesh_component.model = NULL;
      add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);
    }
    break;
  case CAMERA_COMPONENT:
  {
    current_component_type = CAMERA_COMPONENT;
    add_camera_component_to_selected_element();
  }
  default:
    break;
  }
  return current_component_type;
}

Token * fill_components_values(ComponentType type, Token* token_value_name_string){
  Token * last_element_readed;
  switch (type)
  {
  case TRASNFORM_COMPONENT:
  {
    LOG("Pased Transform Component\n");
    last_element_readed = get_token_array_component_transform(token_value_name_string);
    return last_element_readed;
  }    
  case STATIC_MESH_COMPONENT:
  {
    LOG("Parsed Static mesh component\n");
    token_value_name_string++;
    last_element_readed = token_value_name_string;
    int mesh_path_id = get_token_primitive_value(token_value_name_string);
    int texture_id = get_token_primitive_value(token_value_name_string+2);
    StaticMeshComponent* mesh = get_component_from_selected_element(STATIC_MESH_COMPONENT);
    mesh->model_id = mesh_path_id;
    mesh->texture_id = texture_id;
    return last_element_readed;

  }
  case CAMERA_COMPONENT:
  {
    Token* token = token_value_name_string;
    LOG("Parsed Camera component\n");
    if( string_equal(token,"position") != 0){
      LOG("No token \"position\" string passed\n");
      return;
    }
    vec3 pos;
    Token* array_token = token+1;
    Token* array_value_token = token+2;
    Token* last_token_readed = NULL;
    for (int i = 0; i < array_token->size; i++) {
      last_token_readed = array_value_token+i;   
      float value = get_token_primitive_float(array_value_token+i);         
      memcpy(&pos[i],&value,sizeof(float));               
    }
    
    CameraComponent* camera =  get_component_from_selected_element(CAMERA_COMPONENT);
    glm_vec3_copy(pos,camera->position);   

    return last_token_readed;

  }
  break;
  }
}

Token* parse_components_objects(Token* array_components){
  Token * last_element_readed = array_components;
  for(int i= 0; i< array_components->size ; i++){
    Token* object = get_next_object_token(last_element_readed);
    Token* token_primitive_type = object+2;
    ComponentType type = parse_component_type(token_primitive_type);
    last_element_readed = fill_components_values(type,object+3);    
  }
  
  return last_element_readed;
}
Token* get_components_from_token(Token* components_array){
  int components_counts = components_array->size;
  Token* last_element_parsed = parse_components_objects(components_array);
  return last_element_parsed;
}
//return last token readed
Token* parse_element_object_token(Token* object_token){
    Token* name_token = object_token+1;
    if( name_token->type == JSMN_STRING ){
      if( string_equal(name_token,"name") == 0 ){
        char name[20];
        get_token_string(name,name_token+1);
        LOG("Name: %s\n",name);
        strcpy(selected_element->name,name);
        Token* last_element_parsed = get_components_from_token(name_token+3);
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

Token* parse_elements(Token* array_element_token){
  
  int elements_counts = array_element_token->size;
 
  Token* last_element_parsed = array_element_token;
  for(int i = 0; i< elements_counts ; i++){
    new_empty_element();
    last_element_parsed = get_next_object_token(last_element_parsed);
    last_element_parsed = parse_element_object_token(last_element_parsed);
    element_id++;
  }
  return last_element_parsed;
}

void parse_actual_file(Token* tokens, int count){
  Token* last_element_parsed = NULL;
  for(int i = 0; i< count ; i++){
    Token* actual_token = &tokens[i];
    if(actual_token->type == JSMN_STRING){
      if( string_equal(actual_token,"elements") == 0){
        last_element_parsed =  parse_elements(actual_token+1);
        break;
      }
    }
  }

  Token* new_object = get_next_object_token(last_element_parsed);
  for(int i = 0; i< count ; i++){
    last_element_parsed = &tokens[i];
    if(last_element_parsed->type == JSMN_STRING){
        if (string_equal(last_element_parsed, "models") == 0) {
          Token* array_value_token = last_element_parsed+2;
          for (int i = 0; i < (last_element_parsed+1)->size; i++) {   
            char text[20];
            get_token_string(text,array_value_token+i);
            add_to_array(&texts,text);
          }
          break;
        }

    }


  }

  while (string_equal(last_element_parsed,"textures") != 0)
  {
    last_element_parsed++;
  }
  
  Token* texture_array = last_element_parsed+1;
  
  for(int i = 0; i < texture_array->size; i++){
    Token* value = last_element_parsed + 2;
    char text[20];
    get_token_string(text,value+i);
    add_to_array(&textures_paths,text);
  }

}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
  
  int token_count = 20 *(5+5+30);
  
  jsmn_parser parser;
  int max_tokens = token_count+30;
  Token tokens[max_tokens];
  jsmn_init(&parser);
 
  actual_json_file = json_file;

  int parse_result = jsmn_parse(&parser , actual_json_file , json_file_size , tokens,max_tokens);
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

  parse_actual_file(tokens,parse_result);
  
  LOG("json level parsed\n");
}

