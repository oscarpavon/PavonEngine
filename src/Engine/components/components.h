#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

typedef struct Element Element;
#include "../../model.h"
#include <cglm.h>
#include "../array.h"

typedef enum ComponentType{
    TRASNFORM_COMPONENT = 0,
    SPHERE_COMPONENT,
    CAMERA_COMPONENT,
    CUBE_COMPONENT,
    STATIC_MESH_COMPONENT,
    LEVEL_OF_DETAIL_COMPONENT
}ComponentType;


typedef struct ComponentDefinition{
    ComponentType type;
    Element* parent;
    unsigned int id;
    unsigned short int bytes_size;
    void* data;
}ComponentDefinition;

typedef struct SphereComponent{    
    int radius;
    vec3 center;
    Model* model;
}SphereComponent;

typedef struct CubeComponent{    
    vec3 center;
    Model* model;
}CubeComponent;

typedef struct TransformComponent{    
    vec3 position;
    vec4 rotation;
    vec3 scale;
    mat4 model_matrix;
}TransformComponent;

typedef struct CameraComponent{
    mat4 projection;
    mat4 view;
    vec3 front;
    vec3 up;
    vec3 position;
    Model* camera_gizmo;
}CameraComponent;

typedef struct StaticMeshComponent{
    int model_id;
    int texture_id;
    Model* model;
    Array meshes;
    Array distances;
    Array textures;
    vec3 center;
    vec3 bounding_box[2];
}StaticMeshComponent;

typedef struct SkinnedMeshComponent{    
    Model* model;
}SkinnedMeshComponent;

typedef struct HirarchicalLevelOfDetail{
    Model model;
    float distance;
    bool draw;
    bool drew;    
}HirarchicalLevelOfDetail;

typedef struct LevelOfDetailComponent{
    Array meshes;
    Array distances;
    int texture_id;
    int model_id;
    bool initialized;
    HirarchicalLevelOfDetail* hirarchical_level_of_detail;
}LevelOfDetailComponent;


void add_component_to_selected_element(int component_byte_size, void* new_component, ComponentType type);
void init_sphere_component(SphereComponent* component);
void init_camera_component(CameraComponent* component);
void init_cube_component(CubeComponent* component);
void update_per_frame_component(ComponentDefinition*);
void update_component(ComponentDefinition*);
void init_transfrom_component(TransformComponent* component);
void* get_component_from_selected_element(ComponentType type);
void* get_component_from_element(Element* element, ComponentType type);
void change_view_to_camera_component(CameraComponent* camera_component);
void add_transform_component_to_selected_element();
void add_camera_component_to_selected_element();
void update_main_camera_with_camera_component_values(CameraComponent* camera_component);
void clean_component_value(ComponentDefinition* component);
void init_element_component(ComponentDefinition* element_component);
void for_each_element_components(void(*do_to)(ComponentDefinition*));
#endif