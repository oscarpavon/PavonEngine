#include "model.h"

#define CGLTF_IMPLEMENTATION
#include "../ThirdParty/cgltf.h"
#include "stdio.h"
#include "../ThirdParty/cglm/vec3.h"

#include "file_loader.h"

#include "engine.h"

#include "vertex.h"

cgltf_data* current_data;
cgltf_animation* current_animation;
cgltf_animation_channel* current_channel;
cgltf_animation_sampler* current_sampler;
bool copy_nodes = false;
int nodes_counter = 0;

Array* actual_vertex_array;
Array* actual_index_array;
Array* current_array;

int models_parsed = 0;

Node* pe_node_by_name(Array* array, const char* name){
  for( int i = 0; i < array->count ; i++ ){
    Node* node = array_get(array,i);
    if( strcmp( node->name , name ) == 0){
      return node;
    }
  }
}

void read_accessor_indices(cgltf_accessor* accessor){
  array_init(actual_index_array,sizeof(u8),accessor->count);
  for(u8 i = 0 ; i < accessor->count ; i++){
		u8 index = cgltf_accessor_read_index(accessor,i);
      array_add(actual_index_array,&index);
  }
}
/*Read accessor and allocate data in current_array or actual_vertex_array */
void pe_loader_read_accessor(cgltf_accessor* accessor, float* out){
  switch (accessor->type)
  {
  case cgltf_type_vec2:
    for(size_t i = 0 ; i < accessor->count ; i++){
        Vertex* vertex = array_get(actual_vertex_array,i);
        cgltf_accessor_read_float(accessor, i, &vertex->uv[0], 2);
    }
    break;
  case cgltf_type_vec3:{
  
    for(int i = 0 ; i < accessor->count ; i++){
        cgltf_accessor_read_float(accessor, i , &out[i*3] , 3);        
    }

    break;
  }    
  case cgltf_type_vec4:{
    
    for(int i = 0 ; i < accessor->count ; i++){
        cgltf_accessor_read_float(accessor, i, &out[i*4] , 4);
    }      
    break;
  } 
  case cgltf_type_scalar:
    
    for(int i = 0 ; i < accessor->count ; i++){
      float number;      
      cgltf_accessor_read_float(accessor, i, &number, 1);
      array_add(current_array,&number);
    }
    
    break;
  case cgltf_type_mat4:
    {
      for(int i = 0 ; i < accessor->count ; i++){
        cgltf_accessor_read_float(accessor, i, &out[i*16] , 16);
      }      
      break;
    }
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
void pe_loader_attribute(cgltf_attribute* attribute){
  switch (attribute->type)
  {
  case cgltf_attribute_type_position:{
    vec3 vertices_position[attribute->data->count];
    memset(&vertices_position,0,sizeof(vertices_position));

    array_init(actual_vertex_array,sizeof(Vertex),attribute->data->count);

    pe_loader_read_accessor(attribute->data, vertices_position);

    for(int i = 0; i < attribute->data->count ; i++){
        struct Vertex vertex;
        memset(&vertex,0,sizeof(struct Vertex));
        glm_vec3_copy(vertices_position[i],vertex.postion);
        array_add(actual_vertex_array,&vertex);
    }
    break;
  }
  case cgltf_attribute_type_texcoord:
    pe_loader_read_accessor(attribute->data,NULL);
    break;

  case cgltf_attribute_type_joints:{
    vec4 joints[attribute->data->count];
    memset(&joints,0,sizeof(joints));

    pe_loader_read_accessor(attribute->data,joints);
    for(int i = 0; i < attribute->data->count ; i++){
      Vertex* vertex = array_get(actual_vertex_array,i);
      glm_vec4_copy(joints[i],vertex->joint);
    }

    break;
  }

  case cgltf_attribute_type_weights:{
    vec4 weight[attribute->data->count];
    memset(&weight,0,sizeof(weight));

    pe_loader_read_accessor(attribute->data,weight);
    for(int i = 0; i < attribute->data->count ; i++){
      Vertex* vertex = array_get(actual_vertex_array,i);
      glm_vec4_copy(weight[i],vertex->weight);
    }

    break;
  }

  }//end switch


  if(attribute->data->has_min){
    
    glm_vec3_copy(attribute->data->min,selected_model->min);
    
  }
  if(attribute->data->has_max){
    
    glm_vec3_copy(attribute->data->max,selected_model->max);
    
  }
  
}


void load_primitive(cgltf_primitive* primitive){
  
  for(u8 i = 0; i < primitive->attributes_count; i++){
    pe_loader_attribute(&primitive->attributes[i]);
  }
  
  read_accessor_indices(primitive->indices);
}

void pe_loader_mesh(cgltf_mesh* mesh){

  for(int i = 0; i < mesh->primitives_count ; i++){
    new_empty_model();
    actual_vertex_array = &selected_model->vertex_array;
    actual_index_array = &selected_model->index_array;
    load_primitive(&mesh->primitives[i]);
		pe_th_exec_in(pe_th_render_id,&GPU_buffers_create_for_model,selected_model);	
		while(!selected_model->gpu_ready){};	
    models_parsed++;
  }  

}

void check_LOD_names(cgltf_node* node){
  int node_name_size = strlen(node->name);
  char name[node_name_size];
  strcpy(name,node->name);    

  for(int n = 0; n<node_name_size; n++){
    if(name[n] == '_'){
      if(strcmp("LOD0",&name[n]+1) == 0){
        LOG("Found LOD0\n");
        break;
      }
      if(strcmp("LOD1",&name[n]+1) == 0){
        LOG("Found LOD1\n");
        break;
      }
      
      if(strcmp("LOD2",&name[n]+1) == 0){
        LOG("Found LOD2\n");
        break;
      }
      
    }
  }
}



void load_current_sampler_to_channel(AnimationChannel* channel){
  AnimationSampler sampler;
  memset(&sampler,0,sizeof(AnimationSampler));
  array_init(&sampler.inputs,sizeof(float),current_sampler->input->count);
  
  current_array = &sampler.inputs;

  float inputs[current_sampler->input->count];
  pe_loader_read_accessor(current_sampler->input,inputs);

  if(channel->path_type == PATH_TYPE_ROTATION){
    array_init(&sampler.outputs,sizeof(float)*4,current_sampler->output->count);
    vec4 outputs[current_sampler->output->count];  
    pe_loader_read_accessor(current_sampler->output,outputs);
    memcpy(sampler.outputs.data,outputs,sizeof(outputs));
  }else if( channel->path_type == PATH_TYPE_TRANSLATION){
    vec3 outputs[current_sampler->output->count];
    array_init(&sampler.outputs,sizeof(float)*3,current_sampler->output->count);
    pe_loader_read_accessor(current_sampler->output,outputs);
    memcpy(sampler.outputs.data,outputs,sizeof(outputs));
  }

  sampler.outputs.count = current_sampler->output->count; 
  current_array = NULL;
  memcpy(&channel->sampler,&sampler,sizeof(AnimationSampler));
}




void load_current_channel_to_animation(Animation* animation){
  AnimationChannel channel;
  memset(&channel,0,sizeof(AnimationChannel));
  channel.node = pe_node_by_name(&pe_curr_skin_loading->joints,current_channel->target_node->name);
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
   
  array_add(&animation->channels,&channel);
}



void load_current_animation(){
  Animation new_animation;
  memset(&new_animation,0,sizeof(Animation));
  strcpy(new_animation.name,current_animation->name);
	LOG("Loading animation: %s\n",current_animation->name);
  array_init(&new_animation.channels,sizeof(AnimationChannel),current_animation->channels_count);
  for(int i = 0; i< current_animation->channels_count ; i++){
    current_channel = &current_animation->channels[i];
    load_current_channel_to_animation(&new_animation);
  }
  
  float max = 0;
  for(int i  = 0; i < new_animation.channels.count ; i++){
    AnimationChannel* channel = array_get(&new_animation.channels,i);
    float* max_from_channel = array_get(&channel->sampler.inputs,channel->sampler.inputs.count-1);
    if(*max_from_channel > max){
      max = *max_from_channel;
    }
  }
  new_animation.end = max;

  array_add(&pe_curr_skin_loading->animations,&new_animation);
}

int pe_node_load(Node* parent, cgltf_node *in_cgltf_node){

  if (copy_nodes) {
    if (nodes_counter > 1) {

      Node new_node;
      ZERO(new_node);

      if (in_cgltf_node->parent && parent != NULL)
        new_node.parent = parent;

      strcpy(new_node.name, in_cgltf_node->name);

      memcpy(new_node.translation, in_cgltf_node->translation, sizeof(vec3));
      memcpy(new_node.rotation, in_cgltf_node->rotation, sizeof(vec4));

      array_add(&pe_curr_skin_loading->joints, &new_node);
    }
    nodes_counter++;
  }

  if(in_cgltf_node->mesh != NULL){
    check_LOD_names(in_cgltf_node);
    pe_loader_mesh(in_cgltf_node->mesh);   
  }

  if(in_cgltf_node->skin != NULL){    
    current_loaded_component_type = COMPONENT_SKINNED_MESH;

		pe_loader_read_accessor(in_cgltf_node->skin->inverse_bind_matrices,
                            pe_curr_skin_loading->inverse_bind_matrices);

  }
	
	Node* loaded_parent = NULL;
	if(pe_curr_skin_loading)
		loaded_parent = array_pop(&pe_curr_skin_loading->joints); 

  if(in_cgltf_node->children_count == 0 && in_cgltf_node->mesh == NULL)
    return 1;

  for(int i = 0; i < in_cgltf_node->children_count; i++){ 
    pe_node_load( loaded_parent , in_cgltf_node->children[i]);
  }

}

cgltf_result pe_loader_model_from_memory(void* gltf_data, u32 size, const char* path){
  cgltf_options options = {0};
  cgltf_data* data = NULL;

  cgltf_result result = cgltf_parse(&options,gltf_data,size, &data);
  if(result != cgltf_result_success)
    return result;
  
  current_data = data;

  result = cgltf_load_buffers(&options,data,path);
  if(result != cgltf_result_success)
    return result; 
  
	if(!data || !data->scene)
		return cgltf_result_invalid_options;

  current_loaded_component_type = STATIC_MESH_COMPONENT;

  if(data->skins_count >= 1){
		SkinnedMeshComponent skin;
		ZERO(skin);

		array_init(&skin.joints,sizeof(Node),data->nodes_count);	 
    memset(skin.joints.data,0,sizeof(Node) * data->nodes_count);
		
		array_add(&pe_arr_skin_loaded,&skin);
		pe_curr_skin_loading = array_pop(&pe_arr_skin_loaded); 

		copy_nodes = true; 
		current_loaded_component_type = COMPONENT_SKINNED_MESH;

  }

  for(int i = 0; i < data->scene->nodes_count ; i++){
    pe_node_load(NULL, data->scene->nodes[i]);
  }
  
  /* NULL vertex/index array because not needed anymore */
  actual_vertex_array = NULL;
  actual_index_array = NULL;

  if(data->animations_count >= 1){
		LOG("Loding animation\n");
    array_init(&pe_curr_skin_loading->animations,sizeof(Animation),data->animations_count);
    for(int i = 0; i < data->animations_count; i++){
      current_animation = &data->animations[i];
      load_current_animation();
    }
  }

  cgltf_free(data);
  current_data = NULL;

  return result;
}

int pe_loader_model(const char* path){
	File new_file;

  if(load_file(path,&new_file) == -1)
    return -1;

  cgltf_result result = pe_loader_model_from_memory(new_file.data,new_file.size_in_bytes,path);

  if (result != cgltf_result_success){
    LOG("Model no loaded: %s \n", new_file.path);
    if(result == cgltf_result_io_error){
      LOG("Buffer no loaded: %s \n", new_file.path);
      LOG("IO ERROR\n");
    }    
    return -1;
  }
    
  close_file(&new_file);

  //LOG("glTF2 loaded: %s. \n",path);

  int model_result = models_parsed;
  models_parsed = 0;
  nodes_counter = 0;
  return model_result;
}

