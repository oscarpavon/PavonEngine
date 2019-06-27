#include "model.h"
#define CGLTF_IMPLEMENTATION
#include "third_party/cgltf.h"
#include "stdio.h"
#include <vec3.h>

#include "file_loader.h"

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
void load_joints(){

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
    LOGW("gltf loaded. \n");

    cgltf_free(data);
    close_file(&new_file);
    return 0;
}

void free_model_load(LoadModel* model){
  free(model->path);

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

static int dump_int(const char *js, jsmntok_t *next_t, jsmntok_t *object_t, size_t count, int indent, LoadModel* model) {

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

static int dump(const char *js, jsmntok_t *t, size_t count, int indent, LoadModel* model) {
	int i, j, k;
	if (count == 0) {
		return 0;
	}
	if (t->type == JSMN_PRIMITIVE) {
		printf("%.*s = primitive ", t->end - t->start, js+t->start);

    if (jsoneq(js, t-1, "id") == 0){
       
        size_t size = get_token_size(t);
        char text[size+1];
        memcpy(&text,&js[t->start],size);
        text[size] = '\0';
        unsigned int model_id = atoi(text);
        memcpy(&model->id, &model_id, sizeof(unsigned int));
    }
    

		return 1;
	} else if (t->type == JSMN_STRING) {
		printf("'%.*s'", t->end - t->start, js+t->start);

    if (jsoneq(js, t, "path") == 0) {
      t = t+1;
      size_t size = get_token_size(t);
      char text[size+1];
      memcpy(&text,&js[t->start],size);
      text[size] = '\0';
      model->path = malloc(size+1);
      strcpy(model->path, text);    
    }

		return 1;
	} else if (t->type == JSMN_OBJECT) {
		printf("\n");
		j = 0;
		for (i = 0; i < t->size; i++) {
			for (k = 0; k < indent; k++) printf("  ");
			j += dump(js, t+1+j, count-j, indent+1, model);
			printf(": ");
      //dump_int(js, t+1+j, t+j , count-j, indent+1, model);
			j += dump(js, t+1+j, count-j, indent+1, model);     



			printf("\n");
		}
		return j+1;
	} else if (t->type == JSMN_ARRAY) {
		j = 0;
    int array_element_index = 0;
		printf("\n");
    vec3 pos;
		for (i = 0; i < t->size; i++) {
     
      for (k = 0; k < indent-1; k++) {
        printf("  ");
      }

      printf("   - ");
      float float_number = 0;
      
      dump_array(js, t+1+j, count-j, array_element_index,pos);
      array_element_index++;
      
      printf("Float: %f\n",float_number);

      j += dump(js, t+1+j, count-j, indent+1, model);
      printf("\n");

		}

    glm_vec3_copy(pos,model->position);
		return j+1;
	}
	return 0;
}

void parse_json(const char* json_file, size_t json_file_size){
  jsmn_parser p;
  jsmntok_t t[10];
  jsmn_init(&p);
 
  int result = jsmn_parse(&p,json_file,json_file_size,t,10);

  LoadModel model01;
  model01.id = 0;

  dump(json_file,t,10,0, &model01);
  
  printf("json parsed\n");
}