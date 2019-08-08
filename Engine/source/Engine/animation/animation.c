#include "animation.h"
#include "../../engine.h"

void play_animation(Animation* animation){
    animation->time += 0.01;
    float time = animation->time;
    for(int i = 0; i<animation->channels.count ; i++){
        AnimationChannel* channel = get_from_array(&animation->channels,i);
        AnimationSampler* sampler = &channel->sampler;

        Node* node = channel->node;        
        

        for(int j = 0; j < sampler->inputs.count - 1 ; j++){
            float* input0 = get_from_array(&sampler->inputs,j);
            float* input1 = get_from_array(&sampler->inputs,j+1);
            
            if( (time >= *input0) && (time <= *input1) ){
                float time_mix = (time - *input0) / (*input1 - *input0);
                switch (channel->path_type)
                {
                    case PATH_TYPE_ROTATION:
                    {
                        float* quaternion0 = get_from_array(&sampler->outputs_vec4,j);
                        float* quaternion1 = get_from_array(&sampler->outputs_vec4,j+1);
                        float* rotation  = get_from_array(&sampler->outputs_vec4,1);
                        
                        vec4 interpolated;
                        glm_vec4_lerp(quaternion0,quaternion1,time_mix,interpolated);
                        glm_vec4_normalize_to(interpolated,node->rotation);
                        break;
                    }          
                    case PATH_TYPE_TRANSLATION:
                    {
                        
                        break;
                    }          
                }
            }//end if time >=
        }//end for inputs       
    
    }
    //LOG("Played animation: %s\n",animation->name);
}

void play_animation_by_name(SkinnedMeshComponent* skin_component , const char* name, bool loop){
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
    animation->loop = loop;
    add_to_array(&array_animation_play_list,&animation);
    
}


void play_animation_list(){
    for(int i = 0; i< array_animation_play_list.count; i++){
        Animation** ppAnimation = get_from_array(&array_animation_play_list,i);
        Animation* animation = ppAnimation[0];
        if(animation->time <= animation->end){
            play_animation(animation);
            update_vertex_bones_gizmos = true;
        }            
        else{
            if(animation->loop)
                animation->time = 0;
        }
    }
}
