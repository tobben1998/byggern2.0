#ifndef PROGMEM_H_
#define PROGMEM_H_

#include <avr/pgmspace.h>

const char main_1[] PROGMEM="PINGPONG";
const char main_2[] PROGMEM="";
const char main_3[] PROGMEM="1PLAYER";
const char main_4[] PROGMEM="2PLAYER";
const char main_5[] PROGMEM="CONTROLLER";
const char main_6[] PROGMEM="CALIBRATE";
const char main_7[] PROGMEM="ANIMATION";
const char main_8[] PROGMEM="";

const char mainP_1[] PROGMEM="";
const char mainP_2[] PROGMEM="";
const char mainP_3[] PROGMEM=">1PLAYER<";
const char mainP_4[] PROGMEM=">2PLAYER<";
const char mainP_5[] PROGMEM=">CONTROLLER<";
const char mainP_6[] PROGMEM=">CALIBRATE<";
const char mainP_7[] PROGMEM=">ANIMATION<";
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
const char p2_2[] PROGMEM="COMING";
const char p2_3[] PROGMEM="SOON";
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

const char joystick_1[] PROGMEM="";
const char joystick_2[] PROGMEM="";
const char joystick_3[] PROGMEM="JOYSTICK";
const char joystick_4[] PROGMEM="CHOSEN";
const char joystick_5[] PROGMEM="";
const char joystick_6[] PROGMEM="";
const char joystick_7[] PROGMEM="";
const char joystick_8[] PROGMEM="";

const char slider_1[] PROGMEM="";
const char slider_2[] PROGMEM="";
const char slider_3[] PROGMEM="SLIDER";
const char slider_4[] PROGMEM="CHOSEN";
const char slider_5[] PROGMEM="";
const char slider_6[] PROGMEM="";
const char slider_7[] PROGMEM="";
const char slider_8[] PROGMEM="";

const char calibrate_1[] PROGMEM="";
const char calibrate_2[] PROGMEM="";
const char calibrate_3[] PROGMEM="CALIBRATING";
const char calibrate_4[] PROGMEM="";
const char calibrate_5[] PROGMEM="DO NOT TOUCH";
const char calibrate_6[] PROGMEM="THE CONTROLLER";
const char calibrate_7[] PROGMEM="";
const char calibrate_8[] PROGMEM="";

const char animation_1[] PROGMEM="ANIMATION";
const char animation_2[] PROGMEM="";
const char animation_3[] PROGMEM="";
const char animation_4[] PROGMEM="";
const char animation_5[] PROGMEM="";
const char animation_6[] PROGMEM="";
const char animation_7[] PROGMEM="";
const char animation_8[] PROGMEM="";




#define s_page 8 //used to move to by one page. each page is size 8

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



PGM_P const table[] PROGMEM={
	main_1,main_2,main_3,main_4,main_5,main_6,main_7,main_8,
	mainP_1,mainP_2,mainP_3,mainP_4,mainP_5,mainP_6,mainP_7,mainP_8,
	p1_1,p1_2,p1_3,p1_4,p1_5,p1_6,p1_7,p1_8,
	p2_1,p2_2,p2_3,p2_4,p2_5,p2_6,p2_7,p2_8,
	controller_1,controller_2,controller_3,controller_4,controller_5,controller_6,controller_7,controller_8,
	controllerP_1,controllerP_2,controllerP_3,controllerP_4,controllerP_5,controllerP_6,controllerP_7,controllerP_8,
	joystick_1,joystick_2,joystick_3,joystick_4,joystick_5,joystick_6,joystick_7,joystick_8,
	slider_1,slider_2,slider_3,slider_4,slider_5,slider_6,slider_7,slider_8,
	calibrate_1,calibrate_2,calibrate_3,calibrate_4,calibrate_5,calibrate_6,calibrate_7,calibrate_8,
	animation_1,animation_2,animation_3,animation_4,animation_5,animation_6,animation_7,animation_8

};

/*
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
*/
#endif