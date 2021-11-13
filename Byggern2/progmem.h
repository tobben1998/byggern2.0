#ifndef PROGMEM_H_
#define PROGMEM_H_

#include <avr/pgmspace.h>

const char main_1[] PROGMEM="PINGPONG";
const char main_2[] PROGMEM="";
const char main_3[] PROGMEM="1PLAYER";
const char main_4[] PROGMEM="2PLAYER";
const char main_5[] PROGMEM="CONTROLLER";
const char main_6[] PROGMEM="";
const char main_7[] PROGMEM="";
const char main_8[] PROGMEM="";

const char mainP_1[] PROGMEM="";
const char mainP_2[] PROGMEM="";
const char mainP_3[] PROGMEM=">1PLAYER<";
const char mainP_4[] PROGMEM=">2PLAYER<";
const char mainP_5[] PROGMEM=">CONTROLLER<";
const char mainP_6[] PROGMEM="";
const char mainP_7[] PROGMEM="";
const char mainP_8[] PROGMEM="";

const char p1_1[] PROGMEM="";
const char p1_2[] PROGMEM="";
const char p1_3[] PROGMEM="PLACE BALL";
const char p1_4[] PROGMEM="ON SOLONOID";
const char p1_5[] PROGMEM="PRESS RIGHT";
const char p1_6[] PROGMEM="BUTTON START";
const char p1_7[] PROGMEM="";
const char p1_8[] PROGMEM="";

const char p2_1[] PROGMEM="";
const char p2_2[] PROGMEM="INSTRUCTIONS";
const char p2_3[] PROGMEM="HERE";
const char p2_4[] PROGMEM="";
const char p2_5[] PROGMEM="";
const char p2_6[] PROGMEM="";
const char p2_7[] PROGMEM="";
const char p2_8[] PROGMEM="";

const char controller_1[] PROGMEM="CONTROLLER";
const char controller_2[] PROGMEM="";
const char controller_3[] PROGMEM="JOYSTICK";
const char controller_4[] PROGMEM="SLIDER";
const char controller_5[] PROGMEM="";
const char controller_6[] PROGMEM="";
const char controller_7[] PROGMEM="";
const char controller_8[] PROGMEM="";

const char controllerP_1[] PROGMEM="";
const char controllerP_2[] PROGMEM="";
const char controllerP_3[] PROGMEM=">JOYSTICK<";
const char controllerP_4[] PROGMEM=">SLIDER<";
const char controllerP_5[] PROGMEM="";
const char controllerP_6[] PROGMEM="";
const char controllerP_7[] PROGMEM="";
const char controllerP_8[] PROGMEM="";




PGM_P const table[] PROGMEM={
	main_1,main_2,main_3,main_4,main_5,main_6,main_7,main_8,
	mainP_1,mainP_2,mainP_3,mainP_4,mainP_5,mainP_6,mainP_7,mainP_8,
	p1_1,p1_2,p1_3,p1_4,p1_5,p1_6,p1_7,p1_8,
	p2_1,p2_2,p2_3,p2_4,p2_5,p2_6,p2_7,p2_8,
	controller_1,controller_2,controller_3,controller_4,controller_5,controller_6,controller_7,controller_8,
	controllerP_1,controllerP_2,controllerP_3,controllerP_4,controllerP_5,controllerP_6,controllerP_7,controllerP_8

};


PGM_P const p1_table[] PROGMEM={
p1_1,p1_2,p1_3,p1_4
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