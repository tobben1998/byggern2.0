//OLED Header file

#ifndef DRIVER_OLED_H_
#define DRIVER_OLED_H_

#include <avr/pgmspace.h>

#define RIGHTARROW 96+32
#define LEFTARROW 97+32

typedef enum{HORIZONTAL_MODE, VERTICAL_MODE, PAGE_MODE} adressing_mode;
int fontsize;

volatile struct{
	int page;
	int coloumn;
} oled_position;

void oled_reset(void);
void oled_home(void);
void oled_init(void);
void oled_clear_page(uint8_t page);
void oled_clear_screen(void);
void oled_light_screen(void);
void oled_goto_page(uint8_t page);
void oled_goto_col(uint8_t col);
void oled_goto_pos(uint8_t page, uint8_t col);
void oled_wrc(uint8_t data); //oled write command
void oled_wrd(uint8_t data); //oled write data
void oled_mode(adressing_mode mode);
void oled_brightness(uint8_t lvl);
void oled_print(char*, int fs);
void oled_center_print(char* word, int fs);
void oled_write_data(char data);
void oled_write_char(char c, int fs);
char* oled_arrow(char name[]);







#endif