#include "sam.h"
#include "DRIVER_PID.h"
#include "DRIVER_MOTOR.h"
#include "can_controller.h"
#include <stdio.h>
#include <math.h>

int refPos;
int dt_ms = 20;
double Kp = 1;
double Kd = 1;
double Ki = 1;


void PID_update_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[0]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}
