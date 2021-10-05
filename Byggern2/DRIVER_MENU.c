#include "DRIVER_MENU.h"
#include "DRIVER_JOYSTICK.h"
#include "DRIVER_OLED.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int pos_child=0;
//static menu_item* main_menu;
static menu_item* curr_menu;
static int header_pages=2;
//volatile int fontsize=8;

/*
struct{
	struct menu_item* parent;
	//	void functionPtr* fptr;
	struct menu_item* children[8];
	int numOfChildren=0;
	char name[];
} menu_item;
*/
void menu_init(){
	
	menu_item* main_menu = malloc(sizeof(menu_item));
	
	main_menu->name = "Main Menu";
	main_menu->numOfChildren = 0;
	main_menu->parent = NULL;
	main_menu->function = NULL;
	
	Menu_new_submenu(main_menu, "New Game", &f_newgame);
	
	
	/*
	main_menu->name="MAINMENU";
	*/
	
	oled_goto_page(header_pages);
	oled_center_print(main_menu->children[0]->name, fontsize );
	for (int i=1; i < (main_menu->numOfChildren); i++){
		oled_goto_page(header_pages+i);
		oled_center_print(main_menu->children[i]->name, fontsize);
	}
	
	curr_menu = main_menu;
	
}

menu_item* Menu_new_submenu(menu_item* self, char* name, void (*function)(char*)){
	menu_item* new_submenu = malloc(sizeof(menu_item));
	
	new_submenu->parent=self;
	new_submenu->children=NULL;
	new_submenu->numOfChildren=0;
	new_submenu->name=name;
	new_submenu->function = function;
		
	self->children=realloc(self->children, (self->numOfChildren+1)*sizeof(menu_item*));
	self->children[self->numOfChildren]=new_submenu;
	
	self->numOfChildren++;
	
	return new_submenu;
}



/*struct menu_item {
	menu_item* parent;
	name[]="Main";
//	functionPtr*=void;
	menu_item* children[8] = {&new_game}//, &high_score, &calibrate_joystick, &difficulty, &debugging};
	numOfChildren=5;
	}main_menu;

struct menu_item{
	menu_item* parent = &main_menu;
	name[]="New game";
//	functionPtr*=newgame;
	menu_item* children[8]
	numOfChildren=0;
	}new_game;


struct menu_item{
	menu_item* parent = &main_menu;
	name[]="High Scores";
//	functionPtr*=high_score;
	menu_item* children[8]
	numOfChildren=0;
}high_score;

struct menu_item{
	menu_item* parent = &main_menu;
	name[]="Calibrate";
//	functionPtr*=calibrate;
	menu_item* children[8]
	numOfChildren=0;
}calibrate_joystick;

struct menu_item{
	menu_item* parent = &main_menu;
	name[]="Difficulty";
//	functionPtr*=difficulty;
	menu_item* children[8]
	numOfChildren=0;
}difficulty;

struct menu_item{
	menu_item* parent = &main_menu;
	name[]="Debugging";
//	functionPtr* = debugging;
	menu_item* children[8]
	numOfChildren=0;
}debugging;



*/

void f_newgame(){
	oled_reset();
	oled_center_print("New Game", 8);
}

/*
void f_debugging(){
	oled_reset();
	oled_center_print("Debugging", 8);
}

void f_difficulty(){
	oled_reset();
	oled_center_print("Difficulty", 8);
}

void f_calibrate(){
	oled_reset();
	oled_center_print("Calibrate", 8);
}

void f_high_score(){
	oled_reset();
	oled_center_print("Highscore", 8);
}

*/
void navigate(menu_item* self, joystick_direction dir){
switch (dir){
	case UP:
		if(pos_child>0){
			oled_clear_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages-1);
			oled_center_print(self->children[pos_child]->name,fontsize);
			oled_center_print(oled_arrow(self->children[pos_child-1]->name),fontsize);
			pos_child--;
		}
		break;
	case DOWN:
		if(pos_child < self->numOfChildren-1){
			oled_clear_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages+1);
			oled_center_print(self->children[pos_child]->name,fontsize);
			oled_center_print(oled_arrow(self->children[pos_child+1]->name),fontsize);
			pos_child++;
		}
		break;
	case LEFT:
	
		break;
	case RIGHT:
		curr_menu=curr_menu->children[pos_child];
		pos_child=0;
		(*curr_menu->function)(curr_menu->name);
		//curr_menu=curr_menu->children[curr_menu->numOfChildren];
		break;
	
	case NEUTRAL:
	
		break;
	}
		
}





	
