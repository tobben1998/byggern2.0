#ifndef PROGMEM_H_
#define PROGMEM_H_

#include <avr/pgmspace.h>

const char p1_1[] PROGMEM="PLACE BALL";
const char p1_2[] PROGMEM="ON SOLONOID";
const char p1_3[] PROGMEM="PRESS RIGHT";
const char p1_4[] PROGMEM="BUTTON START";

const char main_1[] PROGMEM="PINGPONG";
const char main_2[] PROGMEM="";
const char main_3[] PROGMEM="1PLAYER";
const char main_4[] PROGMEM="2PLAYER";
const char main_5[] PROGMEM="CONTROLLER";

const char mainP_3[] PROGMEM=">1PLAYER<";
const char mainP_4[] PROGMEM=">2PLAYER<";
const char mainP_5[] PROGMEM=">CONTROLLER<";




PGM_P const p1_table[] PROGMEM=
{
	p1_1,
	p1_2,
	p1_3,
	p1_4
};

PGM_P const menu_table[] PROGMEM=
{
	main_1,
	main_2,
	main_3,
	main_4,
	main_5

};

PGM_P const menuP_table[] PROGMEM=
{
	mainP_3,
	mainP_4,
	mainP_5
};

#endif