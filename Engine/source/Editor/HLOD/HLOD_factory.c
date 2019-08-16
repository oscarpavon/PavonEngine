#include "HLOD_factory.h"
#include "../editor.h"

#include "../Textures/texture_factory.h"
#include <math.h>

#define SAVED_DATA_COUNT
vec2 UV_tranlation_offset;
void* saved_vertex_data[SAVED_DATA_COUNT];
Model* saved_vertex_model[SAVED_DATA_COUNT];
int saved_model_data_count = 0;

HLODBoxComponent* current_HLOD_box_component;


void atlas_resize_UV(Model* model){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    init_array(&new_model.vertex_array,sizeof(Vertex),model->vertex_array.count);
    void* data = new_model.vertex_array.data;
    duplicate_model_data(&new_model,model);

    new_model.vertex_array.data = data;
    memcpy(new_model.vertex_array.data,model->vertex_array.data,model->vertex_array.actual_bytes_size);
    
    translate_UV(VEC3(UV_tranlation_offset[0],UV_tranlation_offset[1],0),&new_model,(vec2){0,0});
    UV_tranlation_offset[0] += 1;
    //UV_tranlation_offset[1] = 1;

    scale_UV(0.5, &new_model,(vec2){0,0});

    saved_vertex_model[saved_model_data_count] = model;

    saved_vertex_data[saved_model_data_count] = model->vertex_array.data;
    saved_model_data_count++;
    

    model->vertex_array.data = new_model.vertex_array.data;
}

void check_is_inside(ComponentDefinition* component_definition){
    
    if(component_definition->type == STATIC_MESH_COMPONENT){
        StaticMeshComponent* mesh = component_definition->data;
        
        if(glm_aabb_contains(current_HLOD_box_component->bounding_box,mesh->bounding_box)){
            
            add_to_array(&array_elements_for_HLOD_generation,&component_definition->parent);
            LOG("%s\n",component_definition->parent->name);
            unsigned int* id = get_from_array(&mesh->meshes,mesh->meshes.count-1);
            Model* model = get_from_array(actual_model_array,*id);
             
            
        }
    }
}

void merge_sphere_to_cluster(Sphere* sphere, Sphere* sphere2){

}

bool sphere_inside_sphere(Sphere* sphere01, Sphere* sphere02){
    vec3 distance;
    glm_vec3_sub(sphere02->center,sphere01->center,distance);
    float easy_distance = glm_vec3_dot(distance,distance);
    
    if( (easy_distance+sphere01->radius)<= sphere02->radius)
        return true;
    return false;
}

bool sphere_intersect_with_sphere(Sphere* sphere01, Sphere* sphere02){
    vec3 distance;
    glm_vec3_sub(sphere02->center,sphere01->center,distance);
    float easy_distance = glm_vec3_dot(distance,distance);

    float sum_radius_squared = sphere01->radius + sphere02->radius;
    sum_radius_squared *= sum_radius_squared;
    
    if(easy_distance <= sum_radius_squared)
        return true;

    return false;
}

inline static float get_sphere_volume(Sphere* sphere){
    
    float volume = ( (4.f/3.f) * M_PI * ( sphere->radius * sphere->radius * sphere->radius) );
    sphere->volume = volume;
    return volume;
    
}

void add_spheres(Sphere* sphere01, Sphere* sphere02, Sphere* out){
    if(sphere_inside_sphere(sphere02, sphere01)){
        
        out->radius = sphere01->radius;
    }else if( sphere_inside_sphere(sphere01, sphere02) )
    {
        out->radius = sphere02->radius;

    }else{
        float new_radius = sphere01->radius + sphere02->radius;
        vec3 center;
        glm_vec3_sub(sphere01->center,sphere02->center,center);
        float center_magnitude = sqrt(center[0] * center[0] + center[1] * center[1] + center[2] * center[2]);
        new_radius = (new_radius+center_magnitude) * 0.5;// R = (r1 + r2 + |c1 - c2|) / 2
        out->radius = new_radius;

    }
    

}

float sphere_get_height_cap(Sphere* sphere, float distance){
    float height_sphere01 = (sphere->radius * sphere->radius ) - ( (sphere->radius - distance) * (sphere->radius - distance) );
    return height_sphere01 / (2 * distance);
}

float sphere_volume_overlap(Sphere* sphere01, Sphere* sphere02, float fill_factor01 , float fill_factor02){
    if( !sphere_intersect_with_sphere(sphere01, sphere02) ){
        return 0;
    }

    if( sphere_inside_sphere(sphere01,sphere02) )
        return get_sphere_volume(sphere01);
    
    if( sphere_inside_sphere(sphere02, sphere01 ) )
        return get_sphere_volume(sphere02);


    float distance = glm_vec3_distance(sphere01->center,sphere02->center);

    float height_cap_sphere01 = sphere_get_height_cap(sphere01,distance);
    float height_cap_sphere02 = sphere_get_height_cap(sphere02,distance);

    float distance_square = distance * distance;

    float part01 =  (sphere01->radius + sphere02->radius) * (sphere01->radius + sphere02->radius) - distance_square;

    float part02 = distance_square - ((sphere01->radius - sphere02->radius) * (sphere01->radius - sphere02->radius) );

    float cap_radius = sqrt(part01 * part02) / 2 * distance;


    float PI_divided_six = M_PI / 6.0f;
    float volumen_cap01 = PI_divided_six*(3*(cap_radius*cap_radius) + height_cap_sphere01*height_cap_sphere01)*height_cap_sphere01;
    float volumen_cap02 = PI_divided_six*(3*(cap_radius*cap_radius) + height_cap_sphere02*height_cap_sphere02)*height_cap_sphere02;

    float volume_overlap = fill_factor01 * volumen_cap01 + fill_factor02*volumen_cap02;

    return volume_overlap;
}

float calculate_fill_factor(Sphere* sphere01 , Sphere* sphere02, float fill_factor_sphere01, float fill_factor_sphere02){
    float overlap_volume = sphere_volume_overlap(sphere01,sphere02,fill_factor_sphere01,fill_factor_sphere02);
    Sphere merge_sphere;
    memset(&merge_sphere,0,sizeof(Sphere));
    add_spheres(sphere01,sphere02,&merge_sphere);

    float dividend = fill_factor_sphere01 * get_sphere_volume(sphere01) + 
            fill_factor_sphere02 * get_sphere_volume(sphere02) - overlap_volume;
    float merge_sphres_volumes = get_sphere_volume(&merge_sphere);
    return dividend / merge_sphres_volumes;
}

int short_cluster(const void* cluster01 , const void* cluster02){
    HLODCluster cluster;
    cluster.cost = ((HLODCluster*)cluster01)->cost - ((HLODCluster*)cluster02)->cost;
    return ((HLODCluster*)cluster01)->cost - ((HLODCluster*)cluster02)->cost;
   
}

bool check_if_cluster_contens_same_element(HLODCluster* cluster01, HLODCluster* cluster02){
    for(int e = 0; e < cluster01->elements.count; e++){
        Element** ppElement = get_from_array(&cluster01->elements,e);
        Element* element01 = ppElement[0];
        
        for(int h = 0; h<cluster02->elements.count; h++){
            Element** ppElement2 = get_from_array(&cluster02->elements,h);
            Element* element2 = ppElement2[0];
            if(element01->id == element2->id)
            {
                return true;
            }
        }

    }
    return false;
}

void compute_bounding_sphere_for_every_mesh(){
    int boanding_value = 500;
    int percentage = 50;

   for(int i = 0; i < actual_elements_array->count ; i++){
        Element* element01 = get_from_array(actual_elements_array,i);
        StaticMeshComponent* mesh01 = get_component_from_element(element01,STATIC_MESH_COMPONENT);
        if(!mesh01)
            continue;

        for(int j = i+1; j<actual_elements_array->count  ; j++){
            Element* element02 = get_from_array(actual_elements_array,j);
            StaticMeshComponent* mesh02 = get_component_from_element(element02,STATIC_MESH_COMPONENT);
            if(!mesh02)
                continue;

            HLODCluster cluster;
            memset(&cluster, 0, sizeof(HLODCluster));
            cluster.is_valid = true;

            Sphere sphere01;
            Sphere sphere02;
            memset(&sphere01,0,sizeof(Sphere));
            memset(&sphere02,0,sizeof(Sphere));
            
            sphere01.radius = glm_aabb_radius(mesh01->bounding_box);
            glm_vec3_copy(mesh01->center,sphere01.center);

            sphere02.radius = glm_aabb_radius(mesh02->bounding_box);
            glm_vec3_copy(mesh02->center,sphere02.center);

            Sphere cluster_sphere; 
            add_spheres(&sphere01, &sphere02, &cluster_sphere);

            cluster.bounding_sphere = cluster_sphere;
            LOG("Procesing %s , %s\n",element01->name, element02->name);

            cluster.fill_factor = calculate_fill_factor(&sphere01,&sphere02,1.f,1.f);
            cluster.cost = (cluster.bounding_sphere.radius * cluster.bounding_sphere.radius * cluster.bounding_sphere.radius ) / cluster.fill_factor;
            
            LOG("Fill factor= %f , cost= %f\n",cluster.fill_factor,cluster.cost);

            
            float max_cost = (boanding_value * boanding_value * boanding_value) / percentage;

            if(cluster.cost <=  max_cost ){
                init_array(&cluster.elements,sizeof(Element*),8);
                add_to_array(&cluster.elements, &element01);
                add_to_array(&cluster.elements,&element02);

                strcpy(&cluster.names[0][0],element01->name);
                strcpy(&cluster.names[1][0],element02->name);
                add_to_array(&HLOD_generated_cluster,&cluster);
            }
        }
    }

    for(int i = 0; i<HLOD_generated_cluster.count ; i++){
        HLODCluster* cluster = get_from_array(&HLOD_generated_cluster,i);
        LOG("Before, Cluster %i: with %f\n",i,cluster->cost);
        LOG("   Element: %s\n",&cluster->names[0][0]);
        LOG("   Element: %s\n",&cluster->names[1][0]);
    }

    qsort(HLOD_generated_cluster.data,HLOD_generated_cluster.count,sizeof(HLODCluster),short_cluster);

    for(int i = 0; i<HLOD_generated_cluster.count ; i++){
        HLODCluster* cluster = get_from_array(&HLOD_generated_cluster,i);
        LOG("After short: %f\n",cluster->cost);
    }

    LOG("Merging spheres edges...\n");

    for(int i = 0; i<HLOD_generated_cluster.count ; i++){
        HLODCluster* cluster = get_from_array(&HLOD_generated_cluster,i);
        if(cluster->is_valid){
            for(int j = 0; j<i ; j++){
                HLODCluster* cluster_for_merge = get_from_array(&HLOD_generated_cluster,j);

                if(cluster_for_merge->is_valid){
                    if(!check_if_cluster_contens_same_element(cluster,cluster_for_merge) )
                        continue;
                    
                    HLODCluster new_cluster;
                    memset(&new_cluster,0,sizeof(HLODCluster));
                    new_cluster.is_valid = true;
                    Sphere new_bounding_sphere;
                    memset(&new_bounding_sphere,0,sizeof(Sphere));
                    new_bounding_sphere.radius = cluster->bounding_sphere.radius + cluster_for_merge->bounding_sphere.radius;
                    new_cluster.bounding_sphere = new_bounding_sphere;
                    
                    new_cluster.fill_factor = calculate_fill_factor(&cluster->bounding_sphere,&cluster_for_merge->bounding_sphere,cluster->fill_factor,cluster_for_merge->fill_factor);
                    new_cluster.cost = (new_cluster.bounding_sphere.radius * new_cluster.bounding_sphere.radius * new_cluster.bounding_sphere.radius ) / new_cluster.fill_factor;
                    
                    if(new_cluster.cost <= ( (boanding_value * boanding_value * boanding_value) / percentage) ){
                        Element** ppMergerElement = get_from_array(&cluster_for_merge->elements,1);
                        Element* merger_element = ppMergerElement[0];
                        add_to_array(&cluster_for_merge->elements,&merger_element);

                        cluster_for_merge->fill_factor = new_cluster.fill_factor;
                        cluster_for_merge->bounding_sphere = new_cluster.bounding_sphere;
                        cluster_for_merge->cost = new_cluster.cost;
                        cluster->is_valid = false;
                        
                    }
                    break;
                }
            }
        }
    }

    for(int i = 0; i<HLOD_generated_cluster.count ; i++){
        HLODCluster* cluster = get_from_array(&HLOD_generated_cluster,i);
        if(cluster->is_valid){
            LOG("Cluster active, %i\n",i);
            for(int o = 0; o<cluster->elements.count ; o++){
                Element** ppElement = get_from_array(&cluster->elements,o);
                Element* element = ppElement[0];
                LOG("Element name: %s\n",element->name);
            }
        }
    }
}

char export_folder[] = "../assets/HLOD/";
char format[] = ".gltf";
void export_actives_cluster(){
    for(int i = 0; i<HLOD_generated_cluster.count ; i++){
        HLODCluster* cluster = get_from_array(&HLOD_generated_cluster,i);
        if(cluster->is_valid){
            cluster->id = i;
            clean_array(&array_elements_for_HLOD_generation);
            for (u8 j = 0; j < cluster->elements.count; j++)
            {
                Element** ppElement = get_from_array(&cluster->elements,j);
                Element* element = ppElement[0];
                add_to_array(&array_elements_for_HLOD_generation,&element);

                StaticMeshComponent* mesh = get_component_from_element(element,STATIC_MESH_COMPONENT);
                u32 *id = get_from_array(&mesh->meshes,mesh->meshes.count-1);
                Model* model = get_from_array(actual_model_array,*id);
                atlas_resize_UV(model);
            }
            
            char final_export_name[strlen(export_folder) + strlen(format) + 20];
            sprintf(final_export_name,"%s%s%i%s",export_folder,"HLOD_out",i,format);
            if(export_gltf(final_export_name) == -1){
                LOG("Not exported\n");
                return;
            }
            for (u8 i = 0; i < saved_model_data_count; i++)
            {
                saved_vertex_model[i]->vertex_array.data = saved_vertex_data[i];
            }
            
            char texture_name[30];
            sprintf(texture_name,"../assets/HLOD/HLOD_texture%i.png",i);
            merge_textures(texture_name);
            saved_model_data_count = 0;

            UV_tranlation_offset[0] = 0;
            UV_tranlation_offset[1] = -2;

            clean_array(&array_elements_for_HLOD_generation);
        }
    }
}

void generate_HLODS(){
    UV_tranlation_offset[0] = 0;
    UV_tranlation_offset[1] = -2;
    if(!array_elements_for_HLOD_generation.initialized){
        init_array(&array_elements_for_HLOD_generation,sizeof(Element*),30);
        array_elements_for_HLOD_generation.isPointerToPointer = true;
    }
    clean_array(&array_elements_for_HLOD_generation);

    for(int i = 0; i< HLOD_cluster_count; i++){
       Element* elmeent = HLOD_clusters[i];
       current_HLOD_box_component = get_component_from_element(elmeent,COMPONENT_HLOD_BOX);
       for_each_element_components(check_is_inside);
    }   
    
    if(!HLOD_generated_cluster.initialized){
        init_array(&HLOD_generated_cluster, sizeof(HLODCluster),8);
    }
    
    compute_bounding_sphere_for_every_mesh();

    export_actives_cluster();

    for (u8 i = 0; i < HLOD_generated_cluster.count; i++)
    {
        HLODCluster* cluster  = get_from_array(&HLOD_generated_cluster,i);
        editor_add_HLOD_element(cluster);
    }
    

}