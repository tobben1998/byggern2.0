//Joystick Header file


#ifndef DRIVER_SLIDER_H_
#define DRIVER_SLIDER_H_

#include <stdint.h>



typedef struct {
	int l_pos;
	int r_pos;
} slider_position;



slider_position slider_getPosition(void);

void slider_sendPositionButtonCan(slider_position pos);

#endif
