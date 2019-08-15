#ifndef HLOD_FACTORY_H
#define HLOD_FACTORY_H
#include "../../Engine/array.h"
#include "../../Engine/types.h"

typedef struct Element Element;


Element* HLOD_clusters[20];
void generate_HLODS();
int HLOD_cluster_count;
Array array_elements_for_HLOD_generation;

typedef struct HLODCluster{
    u32 id;
    Sphere bounding_sphere;
    float fill_factor;
    float cost;
    bool is_valid;
    Array elements;
    char names[8][20];
}HLODCluster;

Array HLOD_generated_cluster;
#endif // !HLOD_FACTORY_H