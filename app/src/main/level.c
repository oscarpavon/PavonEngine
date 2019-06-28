//
// Created by pavon on 6/22/19.
//
#include "level.h"

void load_level(const char* path, Level* output_level){
    init_model_array(&output_level->models_array, 1);

    struct Model model01;
    load_model("police.gltf",&model01);
    struct Model model02;
    load_model("lince.gltf",&model02);

    struct Model model03;
    load_model("skydome.gltf",&model03);

    struct Model model04;
    load_model("terrain.gltf",&model04);

    struct Model skeleta_mesh;
//    load_model("skeletal_mesh.gltf",&skeleta_mesh);

    model01.texture.image = load_image("police.jpg");
    model02.texture.image = load_image("lince.png");
    model03.texture.image = load_image("skydome.jpg");
    model04.texture.image = load_image("grass03.png");

    struct Model fireman_model;
    load_model("fireman.gltf",&fireman_model);
    fireman_model.texture.image = load_image("fireman.png");
    glm_mat4_identity(fireman_model.model_mat);

    glm_mat4_identity(model01.model_mat);
    glm_mat4_identity(model02.model_mat);
    glm_mat4_identity(model03.model_mat);
    glm_mat4_identity(model04.model_mat);
    glm_mat4_identity(skeleta_mesh.model_mat);

    glm_translate(model01.model_mat, (vec3){0.6,0,0});
    glm_scale(model02.model_mat, (vec3){0.4, 0.4, 0.4});
    //glm_rotate(model02.model_mat, 180 , (vec3){1,0,0});

    glm_scale(fireman_model.model_mat, (vec3){0.4, 0.4, 0.4});

    add_model_to_array(&output_level->models_array, model02);
    add_model_to_array(&output_level->models_array, model01);
    add_model_to_array(&output_level->models_array, model03);
    add_model_to_array(&output_level->models_array, model04);
    add_model_to_array(&output_level->models_array, fireman_model);
    //add_model_to_array(&output_level->models_array, skeleta_mesh);

}