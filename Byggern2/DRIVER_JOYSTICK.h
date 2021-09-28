//Joystick Header file

#ifndef DRIVER_JOYSTICK_H_
#define DRIVER_JOYSTICK_H_

typedef enum{LEFT, RIGHT, DOWN, UP, NEUTRAL} joystick_direction;

typedef struct {
	int x_pos;
	int y_pos;
	joystick_direction direction;
} joystick_position;

void joystick_calibrate(void);

joystick_position joystick_getPosition(void);

joystick_position joystick_getDirection(void);

#endif
