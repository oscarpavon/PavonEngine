#ifndef ENGINE_MATH_SPHERE_H
#define ENGINE_MATH_SPHERE_H

#include "../../third_party/cglm/cglm.h"

typedef struct Sphere{
    vec3 center;
    float radius;
    float volume;
}Sphere;

inline static bool sphere_inside_sphere(Sphere* sphere01, Sphere* sphere02){
    vec3 distance;
    glm_vec3_sub(sphere02->center,sphere01->center,distance);
    float easy_distance = glm_vec3_dot(distance,distance);
    
    if( (easy_distance+sphere01->radius)<= sphere02->radius)
        return true;
    return false;
}

inline static bool sphere_intersect_with_sphere(Sphere* sphere01, Sphere* sphere02){
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

inline static void sphere_merge(Sphere* sphere01, Sphere* sphere02, Sphere* out){
    if(sphere_inside_sphere(sphere02, sphere01)){
        
        out->radius = sphere01->radius;
    }else if( sphere_inside_sphere(sphere01, sphere02) )
    {
        out->radius = sphere02->radius;

    }else{
       /*  New radius
        R = (r1 + r2 + |c1 - c2|) / 2 */
        float new_radius = sphere01->radius + sphere02->radius;
        vec3 center;
        glm_vec3_sub(sphere01->center,sphere02->center,center);
        float center_magnitude = sqrt(center[0] * center[0] + center[1] * center[1] + center[2] * center[2]);
        new_radius = (new_radius+center_magnitude) * 0.5;
        out->radius = new_radius;

       /*  New center
        C = c1 + (c2 - c1) * (R - r1) / |c2 - c1| (linear interpolation) */
        vec3 differece;
        glm_vec3_sub(sphere02->center,sphere01->center,differece);
        vec3 new_center;
        glm_vec3_add(sphere01->center,differece,new_center);
        glm_vec3_scale(new_center,(new_radius-sphere01->radius),new_center);
        float magniture2 = sqrt(differece[0] * differece[0] + differece[1] * differece[1] + differece[2] * differece[2]);
        glm_vec3_divs(new_center,magniture2,out->center);
        
    }   
    

}

inline static float sphere_get_height_cap(Sphere* sphere, float distance){
    float height_sphere01 = (sphere->radius * sphere->radius ) - ( (sphere->radius - distance) * (sphere->radius - distance) );
    return height_sphere01 / (2 * distance);
}

inline static bool sphere_equals(Sphere* sphere, Sphere* sphere2){
    if (glm_vec3_eqv(sphere2->center,sphere->center) )
    {
        if(sphere->radius == sphere2->radius){
            return true;
        }
    }
    return false;
}

#endif