#include "model.h"

#define CGLTF_IMPLEMENTATION
#include "third_party/cgltf.h"
#include "stdio.h"
#include <vec3.h>

#include "file_loader.h"

#include "engine.h"

cgltf_data* current_data;
cgltf_animation* current_animation;
cgltf_animation_channel* current_channel;
cgltf_animation_sampler* current_sampler;
Array nodes;
bool copy_nodes = false;

Array* actual_vertex_array;
Array* actual_index_array;
Array* current_array;
struct Model* actual_model;
bool model_loaded = false;


void read_accessor_indices(cgltf_accessor* accessor){
  init_array(actual_index_array,sizeof(unsigned short int),accessor->count);
  for(size_t i = 0 ; i < accessor->count ; i++){
      unsigned short int index = cgltf_accessor_read_index(accessor,i);
      add_to_array(actual_index_array,&index);
  }
}

void read_accessor(cgltf_accessor* accessor){
  switch (accessor->type)
  {
  case cgltf_type_vec2:
    for(size_t i = 0 ; i < accessor->count ; i++){
        Vertex* vertex = get_from_array(actual_vertex_array,i);
        cgltf_accessor_read_float(accessor, i, &vertex->uv[0], 2);
    }
    break;
  case cgltf_type_vec3:
    init_array(actual_vertex_array,sizeof(Vertex),accessor->count+1000);
    for(size_t v = 0 ; v < accessor->count ; v++){
      struct Vertex vertex;
      memset(&vertex,0,sizeof(struct Vertex));
      cgltf_accessor_read_float(accessor,v,&vertex.postion[0],3);
      add_to_array(actual_vertex_array,&vertex);
    }
    break;
  case cgltf_type_vec4:
    
    for(int i = 0 ; i < accessor->count ; i++){
      vec4 quaternion;      
      cgltf_accessor_read_float(accessor, i, quaternion , 4);
      add_to_array(current_array,quaternion);      
    }
    
    break;
  case cgltf_type_scalar:
    
    for(int i = 0 ; i < accessor->count ; i++){
      float number;      
      cgltf_accessor_read_float(accessor, i, &number, 1);
      add_to_array(current_array,&number);
    }
    
    break;
  default:
    break;
  }

  switch (accessor->component_type)
  {
  case cgltf_component_type_r_16:
    /* code */
    break;
  
  default:
    break;
  }
}
void load_attribute(cgltf_attribute* attribute){
  switch (attribute->type)
  {
  case cgltf_attribute_type_position:
    read_accessor(attribute->data);
    break;
  case cgltf_attribute_type_texcoord:
    read_accessor(attribute->data);
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

  if(copy_nodes){
    Node new_node;
    memset(&new_node,0,sizeof(Node));  

    if(in_node->parent && parent != NULL)
      new_node.parent = parent;  

    strcpy(new_node.name,in_node->name);

    memcpy(new_node.translation,in_node->translation,sizeof(vec3));
    memcpy(new_node.rotation, in_node->rotation, sizeof(vec4));

    add_to_array(&nodes,&new_node);
  }

  if(in_node->mesh != NULL)
    load_mesh(in_node->mesh);

  if(in_node->skin != NULL){
    Skeletal* skeletal = malloc(sizeof(Skeletal));
    skeletal->joints_count = in_node->skin->joints_count;
    Node* joints = get_from_array(&nodes,index_to_store+1);
    skeletal->joints = joints;
    actual_model->skeletal = skeletal;
    printf("Skin loaded\n");
  }
  
  for(int i = 0; i < in_node->children_count; i++){ 
    Node* parent = get_from_array(&nodes,index_to_store);    
    load_node( parent, in_node->children[i],store_nodes,index_to_store+1);
  }

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
            
 
            break;
          }
          
          if(strcmp("LOD3",&name[n]+1) == 0){
           
            break;
          }
          if(strcmp("HLOD",&name[n]+1) == 0){
           
             
          }
        }
      }
    }
   
  }
}

void load_current_sampler_to_channel(AnimationChannel* channel){
  AnimationSampler sampler;
  init_array(&sampler.inputs,sizeof(float),current_sampler->input->count);
  init_array(&sampler.outputs_vec4,sizeof(vec4),current_sampler->output->count);
  current_array = &sampler.inputs;
  read_accessor(current_sampler->input);
  current_array = &sampler.outputs_vec4;
  read_accessor(current_sampler->output);
  memcpy(&channel->sampler,&sampler,sizeof(AnimationSampler));
}

Node* get_node_by_name(Array* array, const char* name){
  for( int i = 0; i < array->count ; i++ ){
    Node* node = get_from_array(array,i);
    if( strcmp( node->name , name ) == 0){
      return node;
    }
  }
}


void load_current_channel(){
  AnimationChannel channel;
  channel.node = get_node_by_name(&nodes,current_channel->target_node->name);
  switch (current_channel->target_path)
  {
  case cgltf_animation_path_type_rotation:
    channel.path_type = PATH_TYPE_ROTATION; 
    break;
  case cgltf_animation_path_type_translation:
    channel.path_type = PATH_TYPE_TRANSLATION; 
    break;

  default:
    break;
  }

  
  current_sampler = current_channel->sampler;
  load_current_sampler_to_channel(&channel);
  

}



void load_current_animation(){
  Animation new_animation;
  strcpy(new_animation.name,current_animation->name);
  for(int i = 0; i< current_animation->channels_count ; i++){
    current_channel = &current_animation->channels[i];
    //load_current_channel();
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
  current_data = data;

  cgltf_load_buffers(&options,data,new_file.path);
  close_file(&new_file);

  in_model_array = model;
  actual_vertex_array = &model->vertex_array;
  actual_index_array = &model->index_array;
  actual_model = model;
  
  //check_LOD(data);

  if(model_loaded){
    LOGW("gltf loaded with LODs. \n");
    in_model_array = NULL;
    cgltf_free(data);
    
    return 0;
  }  
  
  if(data->skins_count >= 1){
    init_array(&nodes,sizeof(Node),data->nodes_count+1);
    memset(nodes.data,0,sizeof(Node) * data->nodes_count);
    copy_nodes = true; 
  }

  for(int i = 0; i < data->scene->nodes_count ; i++){
    load_node(NULL, data->scene->nodes[i],nodes.data,0);
  }
  
  if(data->animations_count >= 1){
     
    for(int i = 0; i < data->animations_count; i++){
      current_animation = &data->animations[i];
      load_current_animation();
    }
  }

  LOGW("gltf loaded: %s. \n",path);

  cgltf_free(data);
  current_data = NULL;
  in_model_array = NULL;
  actual_vertex_array = NULL;
  actual_index_array = NULL;
  actual_model = NULL;

  return 0;
}

void free_model_load(Element* model){
  free(model->model_path);

}
