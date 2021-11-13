#define F_CPU 16000000  //  clock  frequency  in Hz


#include "DRIVER_MENU.h"
#include "DRIVER_JOYSTICK.h"
#include "DRIVER_OLED.h"
#include "DRIVER_CAN.h"
#include "progmem.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//OLED string ascces defines

#define s_page 8 //used to move to by one page. each page is size 8

#define s_main 0
#define s_mainP 8
#define s_p1 16
#define s_p2 24
#define s_controller 32
#define s_controllerP 40




static controller ctrl = JOYSTICK;



static int pos_child=0;
menu_item* main_menu;
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
	main_menu->oledOffset=s_main;
	
/*	Menu_new_submenu(main_menu, "NEW GAME", ">NEW GAME<", &f_newgame);
	Menu_new_submenu(main_menu, "HIGH SCORE", ">HIGH SCORE<", &f_high_score);
	Menu_new_submenu(main_menu, "CALIBRATE", ">CALIBRATE<", &f_calibrate);
	Menu_new_submenu(main_menu, "DIFFICULTY", ">DIFFICULTY<", &f_difficulty);
	Menu_new_submenu(main_menu, "DEBUGGING", ">DEBUGGING<", &f_debugging); */
	
	Menu_new_submenu(main_menu, "1PLAYER", &f_1player, s_p1);
	Menu_new_submenu(main_menu, "2PLAYER", &f_2player, s_p2);
	menu_item* controller_menu=Menu_new_submenu(main_menu, "CONTROLLER", &f_controller,s_controller);
	Menu_new_submenu(controller_menu, "JOYSTICK", &f_joystick,s_controller);
	Menu_new_submenu(controller_menu, "SLIDER", &f_slider,s_controller);
	
	//Menu_new_submenu(main_menu, "HIGH SCORE", &f_high_score);
	//Menu_new_submenu(main_menu, "CALIBRATE", &f_calibrate);
	//Menu_new_submenu(main_menu, "DIFFICULTY", &f_difficulty);
	//Menu_new_submenu(main_menu, "DEBUGGING", &f_debugging);
	
// 	oled_center_print("PINGPONG",8);
// 	for (int i=0; i < (main_menu->numOfChildren); i++){
// 		oled_goto_page(header_pages+i);
// 		oled_center_print(main_menu->children[i]->name, fontsize);
// 	}
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<6; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i]));
		oled_center_print(buffer,8);
	}
	oled_goto_page(header_pages);
	strcpy_P(buffer,(PGM_P)pgm_read_word(&table[s_mainP+2]));
	oled_center_print(buffer,8);
	
	curr_menu = main_menu;
	
}

menu_item* Menu_new_submenu(menu_item* self, char* name, void (*function)(char*), int oledOffset){
	menu_item* new_submenu = malloc(sizeof(menu_item));

	new_submenu->parent=self;
	new_submenu->children=NULL;
	new_submenu->numOfChildren=0;
	new_submenu->name=name;
	new_submenu->nameArrow=oled_arrow(name);
	new_submenu->function = function;
	new_submenu->oledOffset=oledOffset;
		
	self->children=realloc(self->children, (self->numOfChildren+1)*sizeof(menu_item*));

	self->children[self->numOfChildren]=new_submenu;
	
	self->numOfChildren++;
	
	return new_submenu;
}


void f_1player(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_p1]));
		oled_center_print(buffer,8);	
	}
	while(!(PINB & (1<<0))){ //
	}
		can_message msg;
		msg.id=1;
		msg.length=3;
		msg.data[2]=(char)(PINB & (1<<0));
		can_send_message(&msg);
		
		//start timer
	
	
	switch(ctrl){
		case JOYSTICK:

		//starter counter
			while (1){ //stopper n?r snor treffes					
				joystick_sendPositionButtonCan(joystick_getPosition());
				//tiden kan telle p? skjermen
				
			}
		//	
		break;
		case SLIDER:
			while (1){ //legge inn en variabel som gj?r at n?r man taper kommer man tl main menu for eksempel
				slider_sendPositionButtonCan(slider_getPosition());
			}
		break;
	}	
}

void f_2player(){
	oled_reset();
	oled_center_print("2PLAYER", 8);
}

void f_controller(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_controller]));
		oled_center_print(buffer,8);
	}
		oled_goto_page(header_pages);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[s_controllerP+header_pages]));
		oled_center_print(buffer,8);

}

void f_joystick(){
	oled_reset();
	oled_goto_page(3);
	oled_center_print("JOYSTICK",8);
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(curr_menu->name);
		
	
}
void f_slider(){
	oled_reset();
	oled_center_print("SLIDER",8);
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
	
if(joystick_getDirection()!=NEUTRAL){
			_delay_ms(80);
		
	switch (joystick_getDirection()){
		
		case UP:
		//_delay_ms(10);
		if(pos_child>0){
			oled_goto_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages);
			//oled_center_print(curr_menu->children[pos_child]->name,fontsize);
			char buffer[16];
			strcpy_P(buffer,(PGM_P)pgm_read_word(&table[pos_child+(curr_menu->oledOffset)+header_pages]));
			oled_center_print(buffer,8);
			
			
			oled_goto_page(pos_child+header_pages-1);
			oled_clear_page(pos_child+header_pages-1);
			//oled_center_print(curr_menu->children[pos_child-1]->nameArrow,fontsize);
			strcpy_P(buffer,(PGM_P)pgm_read_word(&table[pos_child-1+(curr_menu->oledOffset)+s_page+header_pages])); //s_page to get to arrow page
			oled_center_print(buffer,8);
			
			printf("----------------- \n");
			pos_child -= 1;
		}
		break;
		case DOWN:
		//_delay_ms(10);
		if(pos_child < curr_menu->numOfChildren-1){
			oled_goto_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages);
			//oled_center_print(curr_menu->children[pos_child]->name,fontsize);
			char buffer[16];
			strcpy_P(buffer,(PGM_P)pgm_read_word(&table[pos_child+(curr_menu->oledOffset)+header_pages]));
			oled_center_print(buffer,8);
			
			oled_goto_page(pos_child+header_pages+1);
			oled_clear_page(pos_child+header_pages+1);
			//oled_center_print(curr_menu->children[pos_child+1]->nameArrow,fontsize);
			strcpy_P(buffer,(PGM_P)pgm_read_word(&table[pos_child+1+(curr_menu->oledOffset)+s_page+header_pages])); //s_page to get to arrow page
			oled_center_print(buffer,8);
			pos_child += 1;
		}
		break;
		/*
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
		//
		
		break;*/
			}
}
	if (!(PINB & (1<<PB2))){
		if(curr_menu->children!= NULL){
			curr_menu=curr_menu->children[pos_child];
			pos_child=0;
			(*curr_menu->function)(curr_menu->name);
		}
		_delay_ms(200);
		printf("button pressed");
	}
		
}




	
