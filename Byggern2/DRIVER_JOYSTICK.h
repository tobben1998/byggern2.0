//Joystick Header file


#ifndef DRIVER_JOYSTICK_H_
#define DRIVER_JOYSTICK_H_

#include <stdint.h>

typedef enum {LEFT, RIGHT, DOWN, UP, NEUTRAL} joystick_direction;

typedef struct {
	int x_pos;
	int y_pos;
} joystick_position;

void joystick_calibrate(void);
//Joystick Header file


#ifndef DRIVER_JOYSTICK_H_
#define DRIVER_JOYSTICK_H_

#include <stdint.h>

typedef enum {LEFT, RIGHT, DOWN, UP, NEUTRAL} joystick_direction;

typedef struct {
	int x_pos;
	int y_pos;
} joystick_position;

void joystick_calibrate(void);

joystick_position joystick_getPosition(void);

joystick_direction joystick_getDirection(void);

void joystick_sendPositionButtonCan(joystick_position pos);

#endif

joystick_position joystick_getPosition(void);

joystick_direction joystick_getDirection(void);

void joystick_sendPositionButtonCan(joystick_position pos);

#endif
