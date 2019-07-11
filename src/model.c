#include "model.h"

#define CGLTF_IMPLEMENTATION
#include "third_party/cgltf.h"
#include "stdio.h"
#include <vec3.h>

#include "file_loader.h"

#include "engine.h"

VertexArray* actual_vertex_array;
IndexArray* actual_index_array;
struct Model* actual_model;
bool model_loaded = false;

void load_uv(cgltf_data* data, VertexArray* out_vertex_array){


  for(size_t i = 0 ; i < data->accessors[1].count ; i++){

    cgltf_accessor_read_float(&data->accessors[1], i, &out_vertex_array->vertices[i].uv[0], 2);

  }


}
void load_indices(cgltf_data* data, IndexArray* index_array){
   
    for(size_t i = 0 ; i < data->meshes[0].primitives[0].indices->count ; i++){
        unsigned short int index= cgltf_accessor_read_index(data->meshes[0].primitives[0].indices,i);
        add_index_to_array(index_array,index);
    }



}
void read_accesor_vertex(cgltf_accessor* accessor){
  for(size_t v = 0 ; v < accessor->count ; v++){
    struct Vertex vertex;
    memset(&vertex,0,sizeof(struct Vertex));
    cgltf_accessor_read_float(accessor,v,&vertex.postion[0],3);
    add_vextex_to_array(actual_vertex_array,vertex);
  }
}

void read_accessor_indices(cgltf_accessor* accessor){
    for(size_t i = 0 ; i < accessor->count ; i++){
        unsigned short int index= cgltf_accessor_read_index(accessor,i);
        add_index_to_array(actual_index_array,index);
    }
}

void read_accesor_uv(cgltf_accessor* accessor){
  for(size_t i = 0 ; i < accessor->count ; i++){
      cgltf_accessor_read_float(accessor, i, &actual_vertex_array->vertices[i].uv[0], 2);
  }
}

void load_attribute(cgltf_attribute* attribute){
  switch (attribute->type)
  {
  case cgltf_attribute_type_position:
    read_accesor_vertex(attribute->data);
    break;
  case cgltf_attribute_type_texcoord:
    read_accesor_uv(attribute->data);
    break;
  
  default:
    break;
  }
}


void load_primitive(cgltf_primitive* primitive){
  for(unsigned short int i = 0; i < primitive->attributes_count; i++){
    load_attribute(&primitive->attributes[i]);
  }
  
  read_accessor_indices(primitive->indices);
}

void load_mesh(cgltf_mesh* mesh){
  load_primitive(&mesh->primitives[0]);
  model_loaded = true;
}

void load_primitives(cgltf_data* data, VertexArray* out_vertex_array){ 

    for(size_t v = 0 ; v < data->accessors[0].count ; v++){
            struct Vertex vertex;
            vertex.uv[0] = 0;
            vertex.uv[1] = 0;
            cgltf_accessor_read_float(&data->accessors[0],v,&vertex.postion[0],3);
            add_vextex_to_array(out_vertex_array,vertex);
    }

}

void load_node(Node* parent, cgltf_node *in_node, Node* store_nodes, int index_to_store){
  Node new_node;
  memset(&new_node,0,sizeof(Node));  

  if(in_node->parent && parent != NULL)
    new_node.parent = parent;
  memcpy(new_node.rotation, in_node->rotation, sizeof(versor));
  
  memcpy(new_node.translation, in_node->translation, sizeof(vec3));
  new_node.name = malloc(strlen(in_node->name));
  strcpy(new_node.name,in_node->name);

  memcpy(&store_nodes[index_to_store],&new_node,sizeof(Node)); 

  for(int i = 0; i < in_node->children_count; i++){ 
    Node* parent = &store_nodes[index_to_store];    
    load_node( parent, in_node->children[i],store_nodes,index_to_store+1);
  }

}

void load_joints( cgltf_data* data){
  int node_count = data->nodes_count;
  Node* nodes = malloc(sizeof(Node) * node_count);

  memset(nodes,0,sizeof(Node) * node_count);

  load_node(NULL, data->scene->nodes[0],nodes,0);
  Skeletal* skeletal = malloc(sizeof(Skeletal));
  skeletal->joints_count = node_count;  
  skeletal->joints = nodes;
  actual_model->skeletal = skeletal;
}

struct Model* in_model_array = NULL;

void check_LOD(cgltf_data* data){
  if(data->nodes_count > 1){
    for(int i = 0; i < data->nodes_count; i++){
      int node_name_size = strlen(data->nodes[i].name);
      char name[node_name_size];
      strcpy(name,data->nodes[i].name);
      for(int n = 0; n<node_name_size; n++){
        if(name[n] == '_'){
          if(strcmp("LOD0",&name[n]+1) == 0){
         
            printf("Found LOD0\n");
            load_mesh(data->nodes[i].mesh);
            
            break;
          }
          if(strcmp("LOD1",&name[n]+1) == 0){
            actual_model->LOD_count++;
            actual_model = actual_model+1;
            init_index_array(&actual_model->index_array,1);
            init_vertex_array(&actual_model->vertex_array,1);
            actual_index_array = &actual_model->index_array;
            actual_vertex_array = &actual_model->vertex_array;

            printf("Found LOD1\n");            
            load_mesh(data->nodes[i].mesh);
 
            break;
          }
          
          if(strcmp("LOD3",&name[n]+1) == 0){
            printf("Found LOD2\n");
            //load_mesh(data->nodes[i].mesh);
            break;
          }
          if(strcmp("HLOD",&name[n]+1) == 0){
            actual_model = actual_model+1;
            printf("Found HLOD\n");
            init_index_array(&actual_model->index_array,1);
            init_vertex_array(&actual_model->vertex_array,1);
            actual_index_array = &actual_model->index_array;
            actual_vertex_array = &actual_model->vertex_array;            
            load_mesh(data->nodes[i].mesh);
            in_model_array->has_HLOD = true;
             
          }
        }
      }
    }
   
  }
}

int load_model(const char* path , struct Model* model){
  model_loaded = false;
  File new_file;

  load_file(path,&new_file);

  cgltf_options options = {0};
  cgltf_data* data = NULL;

  cgltf_result result = cgltf_parse(&options,new_file.data,new_file.size_in_bytes, &data);

  if (result != cgltf_result_success){
    LOGW("Model no loaded: %s \n", new_file.path);
    return -1;
  }
  cgltf_load_buffers(&options,data,new_file.path);
  in_model_array = model;
  actual_vertex_array = &model->vertex_array;
  actual_index_array = &model->index_array;
  actual_model = model;
  init_vertex_array(actual_vertex_array,1);
  init_index_array(actual_index_array,1);

  check_LOD(data);

  if(model_loaded){
    LOGW("gltf loaded with LODs. \n");
    in_model_array = NULL;
    cgltf_free(data);
    close_file(&new_file);
    return 0;
  }  

  load_primitives(data,&model->vertex_array);
  load_indices(data, &model->index_array);
  load_uv(data,&model->vertex_array);

  if(data->skins){
    load_joints(data);
    
  }
  
  LOGW("gltf loaded. \n");

  cgltf_free(data);
  close_file(&new_file);
  return 0;
}

void free_model_load(Element* model){
  free(model->model_path);

}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

#define LEVEL_HEADER_SIZE 24
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

struct Element* model;
struct Element* _elements_array;

static int dump(const char *js, jsmntok_t *token, size_t count, int indent) {
	int i, j;
  model = &_elements_array[element_id];

	if (count == 0) {
		return 0;
	}

	if (token->type == JSMN_PRIMITIVE) {

    if (jsoneq(js, token-1, "id") == 0){
       
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';
      unsigned int model_id = atoi(text);
      memcpy(&model->id, &model_id, sizeof(unsigned int));
    }
    if (jsoneq(js, token-1, "copy") == 0){
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';
      unsigned int model_id = atoi(text);
      memcpy(&model->duplicated_of_id, &model_id, sizeof(unsigned int));
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
      strcpy(model->model_path, text);    
    }

    if (jsoneq(js, token, "texture") == 0){
      token = token+1;
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';      
      strcpy(model->texture_path, text); 
      
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
        glm_vec3_copy(pos,model->position);
                    
      }
      if (jsoneq(js, token-1, "rot") == 0) {
        vec4 quat;
        for (i = 0; i < token->size; i++) {   

          dump_array(js, token+1+j, count-j, array_element_index,quat);
          array_element_index++;               

          j += dump(js, token+1+j, count-j, indent+1);                 

        }
        glm_vec4_copy(quat,model->rotation);
      }
      for (i = 0; i < token->size; i++) {             

        j += dump(js, token+1+j, count-j, indent+1);     

      }      
            
      return j+1;
	}
	return 0;
}

int parse_elements_count(const char* json_file){
  jsmn_parser parser;
  jsmn_init(&parser);

  jsmntok_t level_info_tokens[3];
  int model_count = 0;
  jsmn_parse(&parser,json_file,22,level_info_tokens,3);

  jsmntok_t* count_model_token = &level_info_tokens[2];
  size_t size = get_token_size(count_model_token);
  char text[size+1];
  memcpy(&text,&json_file[count_model_token->start],size);
  text[size] = '\0';
  unsigned int model_id = atoi(text);

  int model_count_result = 0;
  memcpy(&model_count_result, &model_id, sizeof(unsigned int));
  printf("Elements count: %i\n",model_count_result);
  return model_count_result;
}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
  int elements_count = parse_elements_count(json_file);
  int token_count = elements_count *(2+3+4+2+2+1+1);//id + id_num + pos + array + 3 pos + rot + 4 rot + path + textur + textur + model path  
  
  jsmn_parser parser;
  int max_tokens = token_count+30;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);
  


  char* models_json_file = malloc(json_file_size-LEVEL_HEADER_SIZE);
  memset(models_json_file,0,json_file_size-LEVEL_HEADER_SIZE);
  models_json_file[json_file_size-LEVEL_HEADER_SIZE] = '\0';

  memcpy(models_json_file,json_file+LEVEL_HEADER_SIZE,json_file_size-LEVEL_HEADER_SIZE);

  //printf("%s\n", models_json_file);  

  Element elements[elements_count];
  memset(elements,0,sizeof(elements));
  _elements_array = &elements[0];
  for(int i = 0; i<elements_count;i++){
      elements[i].duplicated_of_id = -1;
  }

  int number_of_tokens_readed = jsmn_parse(&parser , models_json_file , json_file_size-LEVEL_HEADER_SIZE , tokens,max_tokens);

  init_array(out_element,sizeof(Element));   

  dump(models_json_file,tokens,number_of_tokens_readed,0);

  
  for(int i = 0; i < elements_count; i++){
    add_element_to_array(out_element,&elements[i]);
  }
 

  int o = 2;
  free(models_json_file);
  printf("json level parsed\n");
}