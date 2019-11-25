#ifndef EDITOR_PROJECT_MANAGER_H
#define EDITOR_PROJECT_MANAGER_H
#include "../../Engine/types.h"
static const char pavon_the_game_project_folder[] = "~/PavonTheGame/";
static const char content_folder[] = "/Content/";

static const char projects_folder[] = "/home/pavon/PavonProjects";

char project_manager_current_path[500];
char project_manager_current_project_name[100];

typedef struct {
	char name[30];

}EditorProject;

bool project_manager_can_show;

void project_manager_window_draw();
void project_manager_update();
void project_manager_init();
void project_manager_open();

void project_manager_new(const char* name);
#endif
