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
	
	
	Menu_new_submenu(main_menu, "1PLAYER", &f_1player, s_p1);
	Menu_new_submenu(main_menu, "2PLAYER", &f_2player, s_p2);
	menu_item* controller_menu=Menu_new_submenu(main_menu, "CONTROLLER", &f_controller,s_controller);
	Menu_new_submenu(controller_menu, "JOYSTICK", &f_joystick,s_joystick);
	Menu_new_submenu(controller_menu, "SLIDER", &f_slider,s_slider);
	Menu_new_submenu(main_menu, "CALIBRATE", &f_calibrate,s_calibrate);
	

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
	
					printf("JOYSTIadasdasasdT");		
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
			while (1){	 //legge inn en variabel som gj?r at n?r man taper kommer man tl main menu for eksempel
				slider_sendPositionButtonCan(slider_getPosition());
			}
		break;
	}	
}

void f_2player(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_p2]));
		oled_center_print(buffer,8);
	}
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
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_joystick]));
		oled_center_print(buffer,8);
	}
	controller ctrl = JOYSTICK;
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
		
	
}
void f_slider(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_slider]));
		oled_center_print(buffer,8);
	}
	controller ctrl = SLIDER;
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
}

void f_calibrate(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_calibrate]));
		oled_center_print(buffer,8);
	}
	
	//funskjon her ||||||||||||||||||||||||||||
	//send over canbuss til node 2 at det skal bli kalibrert n?
	_delay_ms(1000); //juster denne opp til ?s lang tid calibreringa tar
		
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
	
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




	
