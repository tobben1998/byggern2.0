#define F_CPU 16000000  //  clock  frequency  in Hz


#include "DRIVER_MENU.h"
#include "DRIVER_JOYSTICK.h"
#include "DRIVER_OLED.h"
#include "DRIVER_CAN.h"
//#include "progmem.h"
#include "DRIVER_TIMER.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//OLED string ascces defines
#define s_screen 8 //used to move to by one screen. each screen is size 8

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

int seconds;
int mseconds;
char secondsChar;
char digit1char;
char digit2char;
char digit3char;
char mdigitchar;

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
	
	

	
	menu_item* player_menu=Menu_new_submenu(main_menu, "1PLAYER", &f_1player, s_p1);
	Menu_new_submenu(main_menu, "2PLAYER", &f_2player, s_p2);
	menu_item* controller_menu=Menu_new_submenu(main_menu, "CONTROLLER", &f_controller,s_controller);
	Menu_new_submenu(controller_menu, "JOYSTICK", &f_joystick,s_joystick);
	Menu_new_submenu(controller_menu, "SLIDER", &f_slider,s_slider);
	Menu_new_submenu(main_menu, "CALIBRATE", &f_calibrate,s_calibrate);
	Menu_new_submenu(main_menu, "ANIMATION", &f_animation,s_animation);
	//Menu_new_submenu(player_menu, "GAMEOVER", &f_gameover,s_gameover);
	

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
	joystick_sendPositionButtonCan(joystick_getPosition());//For å sende motoren til midten før vi starter.
	while(!(PINB & (1<<0))){ //
		printf("--");
	}
		can_clearIfInterrupted(); 
		can_clearIfInterrupted();//second one because of race codistion. 
		can_message msg;
		msg.id=1;
		msg.length=3;
		msg.data[2]=(char)1; //ER DETTE RIKTIG?
		can_send_message(&msg);
		uint8_t ctrl = xmem_read(0x800); //Leser variabelen fra SRAM. Viktig at adressen stemmer med der man skrev til.
		//start timer
		timer_init();
				
	switch(ctrl){
		case JOYSTICK:

		//starter counter
			while (!can_clearIfInterrupted()){ //stopper n?r snor treffes			
				joystick_sendPositionButtonCan(joystick_getPosition());
				//tiden kan telle p? skjermen
				
			}
		//	
		break;
		case SLIDER:
			while (!can_clearIfInterrupted()){	 //legge inn en variabel som gj?r at n?r man taper kommer man tl main menu for eksempel
				slider_sendPositionButtonCan(slider_getPosition());
			}
		break;
	}	
		//Legge til Millisekunder også?
		seconds = timer_get_seconds();
		mseconds = timer_get_mseconds();
		uint8_t digit3 =seconds/100;
		uint8_t digit2 =seconds/10;
		uint8_t digit1 =seconds%10;
		uint8_t mdigit = mseconds;
		digit3char = digit3+'0';
		digit2char = digit2+'0';
		digit1char = digit1+'0';
		mdigitchar = mdigit + '0';
	
		oled_reset();
		//char buffer[16];
		oled_print_screen_progmem(s_gameover);
		oled_goto_page(4);
		oled_goto_col(63-(5*(8/2)));
		oled_write_char(digit3char,8);
		oled_write_char(digit2char,8);
		oled_write_char(digit1char,8);
		oled_write_char(44,8);
		oled_write_char(mdigitchar,8);
		//char test = (char)seconds;
		//oled_center_print(&test,8);
		
		
		_delay_ms(1000); //juster denne opp til ås lang tid calibreringa tar
		
		curr_menu=main_menu;
		pos_child=0;
		(*curr_menu->function)(main_menu->name);
}

void f_2player(){
	oled_reset();
	oled_print_screen_progmem(s_p2);
}

void f_controller(){
	oled_reset();
	oled_print_screen_progmem(s_controller);
	oled_print_page_progmem(s_controllerP,header_pages);

}

void f_joystick(){
	oled_reset();
	char buffer[16];
	oled_print_screen_progmem(s_joystick);
	controller ctrl = JOYSTICK;
	xmem_write(ctrl,0x800); //Lagrer i SRAM
	printf("%d",ctrl);
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
		
	
}
void f_slider(){
	oled_reset();
	oled_print_screen_progmem(s_slider);
	controller ctrl = SLIDER;
	xmem_write(ctrl,0x800); //Lagrer i SRAM
	printf("%d",ctrl);
	_delay_ms(1000);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
}

void f_calibrate(){
	oled_reset();
	oled_print_screen_progmem(s_calibrate);
	
	//funskjon her ||||||||||||||||||||||||||||
	//send over canbuss til node 2 at det skal bli kalibrert nå
	_delay_ms(1000); //juster denne opp til ås lang tid calibreringa tar
		
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
	
}

void f_animation(){
	oled_reset();
	oled_print_screen_progmem(s_animation);
	
	//funksjon her fjern delay
	oled_animation(4);
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
	
	
}

/*
void f_gameover(){
	oled_reset();
	char buffer[16];
	for(unsigned char i=0; i<8; i++){
		oled_goto_page(i);
		strcpy_P(buffer,(PGM_P)pgm_read_word(&table[i+s_gameover]));
		oled_center_print(buffer,8);
	}
	oled_goto_page(4);
	//char test = (char)seconds;
	//oled_center_print(&test,8);
	_delay_ms(1000); //juster denne opp til ås lang tid calibreringa tar
	
	curr_menu=main_menu;
	pos_child=0;
	(*curr_menu->function)(main_menu->name);
}
*/

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
			
			oled_print_page_progmem(curr_menu->oledOffset,header_pages+pos_child);
			
			oled_goto_page(pos_child+header_pages-1);
			oled_clear_page(pos_child+header_pages-1);
			//oled_center_print(curr_menu->children[pos_child-1]->nameArrow,fontsize);
			oled_print_page_progmem((curr_menu->oledOffset)+s_screen,header_pages+pos_child-1);
			
			
			pos_child -= 1;
		}
		break;
		case DOWN:
		//_delay_ms(10);
		if(pos_child < curr_menu->numOfChildren-1){
			oled_goto_page(pos_child+header_pages);
			oled_clear_page(pos_child+header_pages);
			//oled_center_print(curr_menu->children[pos_child]->name,fontsize);
			oled_print_page_progmem(curr_menu->oledOffset,header_pages+pos_child);
			
			oled_goto_page(pos_child+header_pages+1);
			oled_clear_page(pos_child+header_pages+1);
			//oled_center_print(curr_menu->children[pos_child+1]->nameArrow,fontsize);
			oled_print_page_progmem((curr_menu->oledOffset)+s_screen,header_pages+pos_child+1);
			
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
		//printf("button pressed");
	}
		
}




	
