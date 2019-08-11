#ifndef HLOD_FACTORY_H
#define HLOD_FACTORY_H
#include "../Engine/array.h"
typedef struct Element Element;

Element* HLOD_clusters[20];
void generate_HLODS();
int HLOD_cluster_count;
Array array_elements_for_HLOD_generation;
#endif // !HLOD_FACTORY_H