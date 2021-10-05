

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
	main_menu->nameArrow= NULL;
	main_menu->numOfChildren = 0;
	main_menu->parent = NULL;
	main_menu->function = NULL;
	
/*	Menu_new_submenu(main_menu, "NEW GAME", ">NEW GAME<", &f_newgame);
	Menu_new_submenu(main_menu, "HIGH SCORE", ">HIGH SCORE<", &f_high_score);
	Menu_new_submenu(main_menu, "CALIBRATE", ">CALIBRATE<", &f_calibrate);
	Menu_new_submenu(main_menu, "DIFFICULTY", ">DIFFICULTY<", &f_difficulty);
	Menu_new_submenu(main_menu, "DEBUGGING", ">DEBUGGING<", &f_debugging); */
	
	Menu_new_submenu(main_menu, "NEW GAME", &f_newgame);
	Menu_new_submenu(main_menu, "HIGH SCORE", &f_high_score);
	Menu_new_submenu(main_menu, "CALIBRATE", &f_calibrate);
	Menu_new_submenu(main_menu, "DIFFICULTY", &f_difficulty);
	Menu_new_submenu(main_menu, "DEBUGGING", &f_debugging);
	
	oled_center_print("PINGPONG",8);
	for (int i=0; i < (main_menu->numOfChildren); i++){
		oled_goto_page(header_pages+i);
		oled_center_print(main_menu->children[i]->name, fontsize);
	}
	oled_goto_page(header_pages);
	oled_center_print(main_menu->children[0]->nameArrow,fontsize);
	
	curr_menu = main_menu;
	
}

menu_item* Menu_new_submenu(menu_item* self, char* name, void (*function)(char*)){
	menu_item* new_submenu = malloc(sizeof(menu_item));
	
	new_submenu->parent=self;
	new_submenu->children=NULL;
	new_submenu->numOfChildren=0;
	new_submenu->name=name;
	new_submenu->nameArrow=oled_arrow(new_submenu->name);
	new_submenu->function = function;
		
	self->children=realloc(self->children, (self->numOfChildren+1)*sizeof(menu_item*));
	self->children[self->numOfChildren]=new_submenu;
	
	self->numOfChildren++;
	
	return new_submenu;
}

void f_newgame(){
	oled_reset();
	oled_center_print("New Game", 8);
}


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
	oled_center_print("High Score", 8);
}


void navigate(){
switch (joystick_getDirection()){
	case UP:
		if(pos_child>0){
			oled_goto_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages);
			oled_center_print(curr_menu->children[pos_child]->name,fontsize);
			
			oled_goto_page(pos_child+header_pages-1);
			oled_clear_page(pos_child+header_pages-1);
			oled_center_print(curr_menu->children[pos_child-1]->nameArrow,fontsize);
			
			pos_child -= 1;
		}
		break;
	case DOWN:
		if(pos_child < curr_menu->numOfChildren-1){
			oled_goto_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages);
			oled_center_print(curr_menu->children[pos_child]->name,fontsize);
			
			oled_goto_page(pos_child+header_pages+1);
			oled_clear_page(pos_child+header_pages+1);
			oled_center_print(curr_menu->children[pos_child+1]->nameArrow,fontsize);
			
			pos_child += 1;
		}
		break;
	case LEFT:
		if(curr_menu->parent!=NULL){
			curr_menu=curr_menu->parent;
			pos_child=0;
			(*curr_menu->function)(curr_menu->name);
			
		}
	
		break;
	case RIGHT:
		if(curr_menu->children!= NULL){
			curr_menu=curr_menu->children[pos_child];
			pos_child=0;
			(*curr_menu->function)(curr_menu->name);
		}
		//curr_menu=curr_menu->children[curr_menu->numOfChildren];
		break;
	
	case NEUTRAL:
	
		break;
	}
		
}





	
