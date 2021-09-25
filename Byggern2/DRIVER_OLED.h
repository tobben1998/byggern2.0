//OLED Header file

#ifndef DRIVER_OLED_H_
#define DRIVER_OLED_H_

#include <avr/pgmspace.h>

typedef enum{HORIZONTAL_MODE, VERTICAL_MODE, PAGE_MODE} adressing_mode;


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
void print_print(char*);
void oled_write_data(char data);







#endif