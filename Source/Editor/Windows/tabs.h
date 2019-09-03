#ifndef EDITOR_TABS_H
#define EDITOR_TABS_H

typedef struct{
	void(*draw)(void);
	void(*init)(void);
	void(*finish)(void);
	char name[20];
}EditorTab;

#endif
