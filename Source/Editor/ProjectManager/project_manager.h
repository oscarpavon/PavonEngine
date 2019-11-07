#ifndef EDITOR_PROJECT_MANAGER_H
#define EDITOR_PROJECT_MANAGER_H
#include "../../Engine/types.h"
static const char pavon_the_game_project_folder[] = "/home/pavon/PavonTheGame/";
static const char content_folder[] = "/Content/";

typedef struct {
	char name[30];

}EditorProject;

bool project_manager_can_show;

void project_manager_window_draw();
void project_manager_update();
void project_manager_init();

#endif
