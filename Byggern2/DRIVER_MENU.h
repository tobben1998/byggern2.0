//Joystick Header file

#ifndef DRIVER_MENU_H_
#define DRIVER_MENU_H_

#include "DRIVER_JOYSTICK.h"

typedef struct menu_struct menu_item;

typedef struct menu_struct{
	menu_item* parent;
	void (*function)(char*);
	menu_item** children;
	int numOfChildren;
	char* name;
	
} menu_item;

void menu_init();
//void f_high_score();
//void f_calibrate();
//void f_difficulty();
//void f_debugging();
void f_newgame();
menu_item* Menu_new_submenu(menu_item* self, char* name, void (*function)(char*));

void navigate(menu_item* self, joystick_direction dir);


#endif