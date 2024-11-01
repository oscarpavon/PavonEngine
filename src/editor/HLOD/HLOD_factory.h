#ifndef HLOD_FACTORY_H
#define HLOD_FACTORY_H
#include "../../engine/array.h"
#include "../../engine/types.h"

#include "../../engine/Math/sphere.h"

typedef struct Element Element;

typedef struct HLODCluster{
    u32 id;
    Sphere bounding_sphere;
    float fill_factor;
    float cost;
    bool is_valid;
    Array elements;
    float max_cost;
}HLODCluster;


void generate_HLODS(bool);
void HLODs_generated_debug();

Element* HLOD_clusters[20];
int HLOD_cluster_count;
Array array_elements_for_HLOD_generation;
Array HLOD_generated_cluster;


#endif // !HLOD_FACTORY_H
