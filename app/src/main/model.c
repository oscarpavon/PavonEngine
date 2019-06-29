#include "model.h"

#define CGLTF_IMPLEMENTATION
#include "third_party/cgltf.h"
#include "stdio.h"
#include <vec3.h>

#include "file_loader.h"

#include "engine.h"


void load_uv(cgltf_data* data, VertexArray* out_vertex_array){


  for(size_t i = 0 ; i < data->accessors[1].count ; i++){

    cgltf_accessor_read_float(&data->accessors[1], i, &out_vertex_array->vertices[i].uv[0], 2);

  }


}
void load_indices(cgltf_data* data, IndexArray* index_array){
    init_index_array(index_array,data->meshes[0].primitives[0].indices->count);

    for(size_t i = 0 ; i < data->meshes[0].primitives[0].indices->count ; i++){
        unsigned short int index= cgltf_accessor_read_index(data->meshes[0].primitives[0].indices,i);
        add_index_to_array(index_array,index);
    }



}

void load_primitives(cgltf_data* data, VertexArray* out_vertex_array){

    init_vertex_array(out_vertex_array,data->accessors[0].count);

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
  Node nodes[node_count];
  memset(nodes,0,sizeof(nodes));

  load_node(NULL, data->scene->nodes[0],nodes,0);

}
int load_model(const char* path , struct Model* model){
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
int element_id_limit = 2;
bool can_run = true;



static int dump(const char *js, jsmntok_t *token, size_t count, int indent, Element* model) {
	int i, j, k;
  if(!can_run){
    return 0;
  }

	if (count == 0) {
		return 0;
	}

	if (token->type == JSMN_PRIMITIVE) {
		printf("%.*s = primitive ", token->end - token->start, js+token->start);

    if (jsoneq(js, token-1, "id") == 0){
       
        size_t size = get_token_size(token);
        char text[size+1];
        memcpy(&text,&js[token->start],size);
        text[size] = '\0';
        unsigned int model_id = atoi(text);
        memcpy(&model->id, &model_id, sizeof(unsigned int));
    }
    

		return 1;
	} 
  else if (token->type == JSMN_STRING) {
		printf("'%.*s'", token->end - token->start, js+token->start);

    if (jsoneq(js, token, "path") == 0) {
      token = token+1;
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';
      model->model_path = malloc(size+1);
      strcpy(model->model_path, text);    
    }

    if (jsoneq(js, token, "texture") == 0){
      token = token+1;
      size_t size = get_token_size(token);
      char text[size+1];
      memcpy(&text,&js[token->start],size);
      text[size] = '\0';
      model->texture_path = malloc(size+1);
      strcpy(model->texture_path, text); 
      if(element_id<element_id_limit){
        element_id++;        
      }else{
        can_run = false;
      }
         
    }

		return 1;
	} 
  else if (token->type == JSMN_OBJECT) {  
   
		j = 0;
		for (i = 0; i < token->size; i++) {

      if(!can_run){
        break;
      }           

      Element* element = &model[element_id];
			j += dump(js, token+1+j, count-j, indent+1, element);	

			j += dump(js, token+1+j, count-j, indent+1, element); 

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

                    j += dump(js, token+1+j, count-j, indent+1, model);                 

                  }
                  glm_vec3_copy(pos,model->position);
                    
      }
      if (jsoneq(js, token-1, "rot") == 0) {
                          vec4 quat;
                          for (i = 0; i < token->size; i++) {   

                            dump_array(js, token+1+j, count-j, array_element_index,quat);
                            array_element_index++;               

                            j += dump(js, token+1+j, count-j, indent+1, model);                 

                          }
                          glm_vec4_copy(quat,model->rotation);
      }
      for (i = 0; i < token->size; i++) {
        if(!can_run){
          break;
        }           

        j += dump(js, token+1+j, count-j, indent+1, model);     

      }      
            
      return j+1;
	}
	return 0;
}

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element){
  jsmn_parser parser;
  int max_tokens = 100;
  jsmntok_t tokens[max_tokens];
  jsmn_init(&parser);
 
  int number_of_tokens_readed = jsmn_parse(&parser,json_file,json_file_size,tokens,max_tokens);

  init_array(out_element,sizeof(Element));

  Element models[4];
  memset(models,0,sizeof(Element)*4);
  int count = tokens[0].size;
  
  dump(json_file,tokens,number_of_tokens_readed,0, models);

  add_element_to_array(out_element,&models[0]);
  add_element_to_array(out_element,&models[1]);

  int o = 2;
  
  printf("json level parsed\n");
}