#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include "DRIVER_ADC.h"
#include "DRIVER_OLED.h"
#include "fonts.h"



volatile char *ext_oled_command = (char *) 0x1000;
volatile char *ext_oled_data = (char *) 0x1200;
int fontsize = 8;


//oled write command
void oled_wrc(uint8_t data){
	ext_oled_command[0] = data;
}
//oled write data
void oled_wrd(uint8_t data){
	ext_oled_data[0] = data;
}

void oled_mode(adressing_mode mode){
	oled_wrc(0x20);
	oled_wrc(mode);
	
}

void oled_reset(){
	oled_home();
	oled_clear_screen();
	oled_mode(PAGE_MODE);
}

void oled_home(){
	oled_goto_pos(0,0);
}

void oled_clear_page(uint8_t page){
	oled_goto_pos(page,0);
	for (int j=0; j<128; j++){
		ext_oled_data[j] = 0x00;
	}
}
void oled_goto_page(uint8_t page){
		if(page>7 || page<0){
			printf("denne posisjonen eksisterer ikke");
		}
		else{
			oled_wrc(0xB0+page);
			oled_position.page = page;
		}
}
void oled_goto_col(uint8_t col){
	if(col<0 || col>127){
		printf("denne posisjonen eksisterer ikke");
	}
	else{
		int ms=col/16;
		int ls=col%16;
		oled_wrc(0x10+ms);
		oled_wrc(0x00+ls);
		oled_position.coloumn = col;
	}
}

void oled_goto_pos(uint8_t page, uint8_t col){
	oled_goto_page(page);
	oled_goto_col(col);
}


void oled_clear_screen(void){
	oled_goto_pos(0,0);
	oled_mode(HORIZONTAL_MODE);
	for (int j=0; j<1024; j++){
		ext_oled_data[j] = 0x00;
	}
	oled_goto_pos(4,0);
	for (int j=0; j<1024; j++){
		ext_oled_data[j] = 0x00;
	}
	oled_mode(PAGE_MODE);
}

void oled_brightness(uint8_t lvl){
	if(lvl < 0 || lvl>255){
		lvl = lvl%255; //error correction 
	} 
	oled_wrc(0x81); //Command "Set Contrast Control"
	oled_wrc(lvl);
}

void oled_light_screen(void){
	oled_goto_pos(0,0);
	oled_mode(HORIZONTAL_MODE);
	for (int j=0; j<1024; j++){
		ext_oled_data[j] = 0xff;
	}
	oled_goto_pos(4,0);
	for (int j=0; j<1024; j++){
		ext_oled_data[j] = 0xff;
	}
	oled_mode(PAGE_MODE);
}


void oled_write_char(char c, int fs){
	c=c-32;
	
	switch(fs){
		case 8 :
			for(int i=0; i<fontsize;i++){
				oled_wrd(pgm_read_word(&font8[c][i]));
				oled_position.coloumn++;
			}
			break;
		case 6 :
			for(int i=0; i<6;i++){
				oled_wrd(pgm_read_word(&font6[c][i]));
				oled_position.coloumn++;
			}
			break;
		case 4 :
			for(int i=0; i<4;i++){
				oled_wrd(pgm_read_word(&font4[c][i]));
				oled_position.coloumn++;
			}
			break;
		case 0 :
			for(int i=0; i<fontsize;i++){
				oled_wrd(pgm_read_word(&font_animation[c][i]));
				oled_position.coloumn++;
			}
			break;
	}
	
	
	//PROGMEM font4[95][4];
	
}



void oled_print(char* word, int fs){
	for( int i = 0; i < strlen(word); i++){
		oled_write_char(word[i], fs);
	}
}

void oled_center_print(char* word, int fs){
	oled_goto_col(63-(strlen(word)*fs/2));
	oled_print(word, fs);
}

char* oled_arrow(char* name){
	char* str = malloc(sizeof(char)*20);
	strcpy(str,">");
	strcat(str,name);
	strcat(str,"<");
	return str;
}

void oled_animation_frame(char c, int col,char c1,int col1){
	oled_goto_col(col);
	oled_write_char(c,0);
	oled_goto_col(col1);
	oled_write_char(c1,0);
	_delay_ms(50);
	oled_goto_col(col1);
	oled_write_char('H',0);
}

void oled_animation(int page){
	oled_goto_page(page);
	oled_goto_col(120);
	oled_write_char('J',0);
	for( int i = 0; i < 3; i++){
		oled_animation_frame('A',0,'I',2);
		oled_animation_frame('B',0,'I',3);
		oled_animation_frame('C',0,'I',4);
		oled_animation_frame('D',0,'I',5);
		oled_animation_frame('E',0,'I',6);
		oled_animation_frame('F',0,'I',7);
		oled_animation_frame('G',0,'I',8);
		oled_animation_frame('F',0,'I',9);
		oled_animation_frame('E',0,'I',10);
		oled_animation_frame('D',0,'I',11);
		oled_animation_frame('C',0,'I',12);
		oled_animation_frame('B',0,'I',13);
		oled_animation_frame('A',0,'I',14);
		
		for(int i=15; i<118; i++){
			oled_goto_col(i);
			oled_write_char('I',0);
			_delay_ms(50);
			oled_goto_col(i);
			oled_write_char('H',0);
		}

		oled_animation_frame('J',120,'I',118);
		oled_animation_frame('K',120,'I',117);
		oled_animation_frame('L',120,'I',116);
		oled_animation_frame('M',120,'I',115);
		oled_animation_frame('N',120,'I',114);
		oled_animation_frame('O',120,'I',113);
		oled_animation_frame('P',120,'I',112);
		oled_animation_frame('O',120,'I',111);
		oled_animation_frame('N',120,'I',110);
		oled_animation_frame('M',120,'I',109);
		oled_animation_frame('L',120,'I',108);
		oled_animation_frame('K',120,'I',107);
		oled_animation_frame('J',120,'I',106);
		for(int i=105; i>1; i--){
			oled_goto_col(i);
			oled_write_char('I',0);
			_delay_ms(50);
			oled_goto_col(i);
			oled_write_char('H',0);
		}
	}
}


void oled_init (void){
	      oled_wrc(0xae);        // display off
	      oled_wrc(0xa1);        //segment remap
	      oled_wrc(0xda);        //common pads hardware: alternative
	      oled_wrc(0x12);
	      oled_wrc(0xc8);        //common  output scan direction:com63~com0
	      oled_wrc(0xa8);        //multiplex ration mode:63
	      oled_wrc(0x3f);
	      oled_wrc(0xd5);        //display  divide ratio/osc. freq. mode
	      oled_wrc(0x80);
	      oled_wrc(0x81);        //contrast control
	      oled_wrc(0x50);
	      oled_wrc(0xd9);        //set pre-charge period
	      oled_wrc(0x21);
	      oled_wrc(0x20);        //Set Memory Addressing Mode
	      oled_wrc(0x02);
	      oled_wrc(0xdb);        //VCOM deselect level mode
	      oled_wrc(0x30);
	      oled_wrc(0xad);        //master configuration
	      oled_wrc(0x00);
	      oled_wrc(0xa4);        //out follows RAM content
	      oled_wrc(0xa6);        //set normal display
	      oled_wrc(0xaf);        // display on
		  oled_clear_screen();
		  oled_goto_pos(0,0);

	
}
