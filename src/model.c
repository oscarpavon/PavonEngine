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


void load_primitives(cgltf_data* data, VertexArray* out_vertex_array){ 
  if(data->accessors[0].has_min){
    
    glm_vec3_copy(data->accessors[0].min,actual_model->min);
    
  }
  if(data->accessors[0].has_max){
    
    glm_vec3_copy(data->accessors[0].max,actual_model->max);
    
  }

  for(size_t v = 0 ; v < data->accessors[0].count ; v++){
    struct Vertex vertex;
    vertex.uv[0] = 0;
    vertex.uv[1] = 0;
    cgltf_accessor_read_float(&data->accessors[0],v,&vertex.postion[0],3);
    add_vextex_to_array(out_vertex_array,vertex);
  }

}


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

  if(attribute->data->has_min){
    
    glm_vec3_copy(attribute->data->min,actual_model->min);
    
  }
  if(attribute->data->has_max){
    
    glm_vec3_copy(attribute->data->max,actual_model->max);
    
  }
}


void load_primitive(cgltf_primitive* primitive){
  
  for(unsigned short int i = 0; i < primitive->attributes_count; i++){
    load_attribute(&primitive->attributes[i]);
  }
  
  read_accessor_indices(primitive->indices);
}

void load_mesh(cgltf_mesh* mesh){
  for(int i = 0; i < mesh->primitives_count ; i++){
    load_primitive(&mesh->primitives[i]);
  }  
  model_loaded = true;
}

void load_node(Node* parent, cgltf_node *in_node, Node* store_nodes, int index_to_store){
  Node new_node;
  memset(&new_node,0,sizeof(Node));  

  if(in_node->parent && parent != NULL)
    new_node.parent = parent;
  memcpy(new_node.rotation, in_node->rotation, sizeof(versor));
  
  memcpy(new_node.translation, in_node->translation, sizeof(vec3));
  strcpy(new_node.name,in_node->name);

  memcpy(&store_nodes[index_to_store],&new_node,sizeof(Node)); 

  if(in_node->mesh != NULL)
      load_mesh(in_node->mesh);

  if(in_node->skin != NULL){
    Skeletal* skeletal = malloc(sizeof(Skeletal));
    skeletal->joints_count = in_node->skin->joints_count;  
    skeletal->joints = &store_nodes[index_to_store];
    actual_model->skeletal = skeletal;
    printf("Skin loaded\n");
  }
   
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
  close_file(&new_file);

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
    
    return 0;
  }  
  
  Node* nodes = malloc(sizeof(Node) * data->scene->nodes_count);

  memset(nodes,0,sizeof(Node) * data->scene->nodes_count);  
  
  for(int i = 0; i < data->scene->nodes_count ; i++){
    load_node(NULL, data->scene->nodes[i],nodes,0);
  }
  //load_mesh(data->nodes[0].mesh);

  //load_primitives(data,&model->vertex_array);
  //load_indices(data, &model->index_array);
  load_uv(data,&model->vertex_array);

  if(data->skins){
    //load_joints(data);
    
  }
  
  LOGW("gltf loaded. \n");

  cgltf_free(data);
  
  return 0;
}

void free_model_load(Element* model){
  free(model->model_path);

}
