//Joystick Header file

#ifndef DRIVER_JOYSTICK_H_
#define DRIVER_JOYSTICK_H_

typedef struct {
	int x_pos;
	int y_pos;
	char* direction;
} joystick_position;

void joystick_calibrate(void);

joystick_position joystick_getPosition(void);

joystick_position joystick_getDirection(void);

#endif
