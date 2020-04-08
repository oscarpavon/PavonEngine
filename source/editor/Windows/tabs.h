#ifndef EDITOR_TABS_H
#define EDITOR_TABS_H
#include "../../engine/camera.h"
typedef struct EditorWindow EditorWindow;

typedef struct{
	void(*draw)(void);
	void(*init)(void);
	void(*finish)(void);
	char name[20];
	CameraComponent camera;
}EditorTab;

void tabs_new(EditorWindow* window, const char* name);
void tabs_draw_tabs_bar(EditorWindow* window);
#endif
