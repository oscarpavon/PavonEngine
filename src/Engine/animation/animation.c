#include "animation.h"
#include "../../engine.h"

void play_animation(Animation* animation){
    for(int i = 0; i<animation->channels.count ; i++){
        AnimationChannel* channel = get_from_array(&animation->channels,i);
        AnimationSampler* sampler = &channel->sampler;
        for(int j = 0; j < sampler->outputs_vec4.count ; j++){
            float* quaternion = get_from_array(&sampler->outputs_vec4,j);
            glm_vec4_print(quaternion,stdout);
        }                
        Node* node = channel->node;
        glm_vec4_print(node->rotation,stdout);

        float* quaternion = get_from_array(&sampler->outputs_vec4,sampler->outputs_vec4.count-1);
        glm_vec4_copy(quaternion,node->rotation);
        
    }
    LOG("Played animation: %s\n",animation->name);
}

void play_animation_by_name(SkinnedMeshComponent* skin_component , const char* name){
    Animation* animation; 
    for( int i = 0 ; i<skin_component->animations.count ; i++){
        Animation* geted_animation = get_from_array(&skin_component->animations, i);
        if( strcmp(name , geted_animation->name) == 0 ){
            animation = geted_animation;
            break;
        }
    }

    if(!animation){
        LOG("Animation not found: %s\n",name);
        return;
    }

    play_animation(animation);
}

