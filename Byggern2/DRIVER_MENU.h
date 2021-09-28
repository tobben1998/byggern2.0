//Joystick Header file

#ifndef DRIVER_MENU_H_
#define DRIVER_MENU_H_

#include "DRIVER_JOYSTICK.h"

struct{
	struct menu_item* parent;
//	void functionPtr* fptr;
	struct menu_item* children[8];
	int numOfChildren=0;
	char name[];
} menu_item;

void menu_init();
void f_high_score();
void f_calibrate();
void f_difficulty();
void f_debugging();
void f_new_game();

void navigate(menu_item* self, joystick_direction dir);

#endif