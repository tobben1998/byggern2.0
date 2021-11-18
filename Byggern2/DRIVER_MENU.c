#define F_CPU 16000000  


#include "DRIVER_MENU.h"
#include "DRIVER_JOYSTICK.h"
#include "DRIVER_OLED.h"
#include "DRIVER_CAN.h"
#include "DRIVER_TIMER.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>

//OLED string access
#define s_screen 8 //used to move to by one screen in progmem

#define s_main 0
#define s_mainP 8
#define s_p1 16
#define s_p2 24
#define s_controller 32
#define s_controllerP 40
#define s_joystick 48
#define s_slider 56
#define s_calibrate 64
#define s_animation 72
#define s_gameover 80

static controller ctrl;
static int pos_child=0;
menu_item* main_menu;
static menu_item* curr_menu;
static int header_pages=2;

void menu_init(){
	
	menu_item* main_menu = malloc(sizeof(menu_item));
	main_menu->name = "Main Menu";
	main_menu->nameArrow= NULL;
	main_menu->numOfChildren = 0;
	main_menu->parent = NULL;
	main_menu->function = NULL;
	main_menu->oledOffset=s_main;
	
	menu_item* player_menu=Menu_new_submenu(main_menu, "1PLAYER", &f_1player, s_p1);
	Menu_new_submenu(main_menu, "2PLAYER", &f_2player, s_p2);
	menu_item* controller_menu=Menu_new_submenu(main_menu, "CONTROLLER", &f_controller,s_controller);
	Menu_new_submenu(controller_menu, "JOYSTICK", &f_joystick,s_joystick);
	Menu_new_submenu(controller_menu, "SLIDER", &f_slider,s_slider);
	Menu_new_submenu(main_menu, "CALIBRATE", &f_calibrate,s_calibrate);
	Menu_new_submenu(main_menu, "ANIMATION", &f_animation,s_animation);
	

	oled_reset();
	oled_print_screen_progmem(s_main);
	oled_print_page_progmem(s_mainP, header_pages);
	
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
	oled_print_screen_progmem(s_p1);
	joystick_sendPositionButtonCan(joystick_getPosition());//send motor to mid before start
	while(!(PINB & (1<<0))){
		printf("--");
	}
		for(unsigned char i=0; i<5; i++){
			can_clearIfInterrupted();
		}
		//several times because of race condition
		
		can_message msg;
		msg.id = 1;
		msg.length = 3;
		msg.data[2] = (char)1;
		can_send_message(&msg); //solenoide hit
		
		uint8_t ctrl = xmem_read(0x800); //read variable from SRAM joystick/slider.
		
		if(!(ctrl ==1 || ctrl ==0)){
			ctrl=0;
		}
		timer2_set_ctrl(ctrl);
		timer_init();
		timer2_init();
		timer2_stop();
		
				
	switch(ctrl){
		case JOYSTICK:
			timer2_start();
			while (!can_clearIfInterrupted()){
				printf("--");
			}
			timer2_stop();

		break;
		case SLIDER:
			timer2_start();
			while (!can_clearIfInterrupted()){	
				printf("--");
			}
			timer2_stop();
		break;
	}	
	
		oled_reset();
		oled_print_screen_progmem(s_gameover);
		oled_print_time_used();
		
		_delay_ms(1000);
		
		curr_menu=main_menu;
		pos_child=0;
		(*curr_menu->function)(main_menu->name);
}

void f_2player(){
	oled_reset();
	oled_print_screen_progmem(s_p2);
	
	_delay_ms(1000);
	curr_menu = main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
}

void f_controller(){
	oled_reset();
	oled_print_screen_progmem(s_controller);
	oled_print_page_progmem(s_controllerP,header_pages);
}

void f_joystick(){
	oled_reset();
	oled_print_screen_progmem(s_joystick);
	
	cli();
	controller ctrl = JOYSTICK;
	xmem_write(ctrl,0x800); //Lagrer i SRAM
	sei();
	
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
		
	
}
void f_slider(){
	oled_reset();
	oled_print_screen_progmem(s_slider);
	
	cli();
	controller ctrl = SLIDER;
	xmem_write(ctrl,0x800); //Lagrer i SRAM
	sei();
	
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
}

void f_calibrate(){
	oled_reset();
	oled_print_screen_progmem(s_calibrate);
	
	_delay_ms(700);
	joystick_calibrate();
	_delay_ms(300);
		
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
	
}

void f_animation(){
	oled_reset();
	oled_print_screen_progmem(s_animation);
	oled_animation(4);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);	
}

void navigate(){
	
	if(joystick_getDirection()!=NEUTRAL){
				_delay_ms(80);
		
		switch (joystick_getDirection()){
		
			case UP:
			if(pos_child>0){
				oled_goto_page(pos_child+header_pages);
				oled_clear_page(pos_child+header_pages);
				oled_print_page_progmem(curr_menu->oledOffset,header_pages+pos_child);
			
				oled_goto_page(pos_child+header_pages-1);
				oled_clear_page(pos_child+header_pages-1);
				oled_print_page_progmem((curr_menu->oledOffset)+s_screen,header_pages+pos_child-1);
				
				pos_child -= 1;
			}
			break;
			case DOWN:
			if(pos_child < curr_menu->numOfChildren-1){
				oled_goto_page(pos_child+header_pages);
				oled_clear_page(pos_child+header_pages);
				oled_print_page_progmem(curr_menu->oledOffset,header_pages+pos_child);
			
				oled_goto_page(pos_child+header_pages+1);
				oled_clear_page(pos_child+header_pages+1);
				oled_print_page_progmem((curr_menu->oledOffset)+s_screen,header_pages+pos_child+1);
			
				pos_child += 1;
			}
			break;
		}
	}
		if (!(PINB & (1<<PB2))){
			if(curr_menu->children!= NULL){
				curr_menu=curr_menu->children[pos_child];
				pos_child=0;
				(*curr_menu->function)(curr_menu->name);
			}
			_delay_ms(200);
		}
}




	
