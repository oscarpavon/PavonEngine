#include "geometry.h"
#include "engine/engine.h"

unsigned short int cube_indices[] = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 6, 4, 6, 7, // back
    0, 4, 1, 1, 4, 7, // top
    2, 6, 5, 2, 5, 3, // bottom
    3, 5, 4, 4, 0, 3, // right
    6, 2, 7, 7, 2, 1, // left
};

void geometry_cube_create_vertices(float *box)
{
    array_init(&selected_model->vertex_array, sizeof(Vertex), 8);

    struct Vertex back_right_down; //min
    memset(&back_right_down, 0, sizeof(Vertex));
    struct Vertex front_left_up; //max
    memset(&front_left_up, 0, sizeof(Vertex));

    struct Vertex front_right_down;
    struct Vertex back_left_up;
    struct Vertex front_left_down;
    struct Vertex back_right_up;
    struct Vertex back_left_down;
    struct Vertex front_right_up;

    glm_vec3_copy(&box[0], back_right_down.position);
    glm_vec3_copy(&box[3], front_left_up.position);

    glm_vec3_copy((vec3){back_right_down.position[0], front_left_up.position[1], back_right_down.position[2]}, front_right_down.position);

    glm_vec3_copy((vec3){front_left_up.position[0], back_right_down.position[1], front_left_up.position[2]}, back_left_up.position);

    glm_vec3_copy((vec3){front_left_up.position[0], front_left_up.position[1], back_right_down.position[2]}, front_left_down.position);

    glm_vec3_copy((vec3){back_right_down.position[0], back_right_down.position[1], front_left_up.position[2]}, back_right_up.position);

    glm_vec3_copy((vec3){front_left_up.position[0], back_right_down.position[1], back_right_down.position[2]}, back_left_down.position);

    glm_vec3_copy((vec3){back_right_down.position[0], front_left_up.position[1], front_left_up.position[2]}, front_right_up.position);

    array_add(&selected_model->vertex_array, &front_right_up);
    array_add(&selected_model->vertex_array, &front_left_up);
    array_add(&selected_model->vertex_array, &front_left_down);
    array_add(&selected_model->vertex_array, &front_right_down);

    array_add(&selected_model->vertex_array, &back_right_up);
    array_add(&selected_model->vertex_array, &back_right_down);
    array_add(&selected_model->vertex_array, &back_left_down);
    array_add(&selected_model->vertex_array, &back_left_up);
}

void geometry_cube_create_indices()
{
    array_init(&selected_model->index_array, sizeof(unsigned short int), 36);
    for (int i = 0; i < 36; i++)
    {
        array_add(&selected_model->index_array, &cube_indices[i]);
    }
}

void geometry_sphere_create_vertices(int vertice_count)
{
    array_init(&selected_model->vertex_array, sizeof(Vertex), 360);
    const int NUMPOINTS = 24;

    int i;
	float X;
	float Y;
	float Theta;
	float WedgeAngle;	//Size of angle between two points on the circle (single wedge)

	WedgeAngle = (float)((2*M_PI) / NUMPOINTS);

	for(i=0; i<=NUMPOINTS; i++)
	{
		//Calculate theta for this vertex

		Theta = i * WedgeAngle;

		X = (float)(0 + 2 * cos(Theta));
		Y = (float)(0 - 2 * sin(Theta));
        Vertex vertex;
		glm_vec3_copy(VEC3(X,Y,0),vertex.position);
        array_add(&selected_model->vertex_array,&vertex);
	}

    for(i=0; i<=NUMPOINTS; i++)
	{
		//Calculate theta for this vertex

		Theta = i * WedgeAngle;

		X = (float)(0 + 2 * cos(Theta));
		Y = (float)(0 - 2 * sin(Theta));
        Vertex vertex;
		glm_vec3_copy(VEC3(0,X,Y),vertex.position);
        array_add(&selected_model->vertex_array,&vertex);
	}
}
