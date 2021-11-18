#ifndef DRIVER_CAN_H_
#define DRIVER_CAN_H_


#define TXBnSIDH 0x31 // 0x41 and 0x51
#define TXBnSIDL 0x32
#define TXBnDLC 0x35
#define TXBnDm 0x36

#define RXBnSIDH 0x61
#define RXBnSIDL 0x62
#define RXBnDLC 0x65
#define RXBnDM 0x66

#include <stdint.h>

typedef struct{
	uint16_t id;
	char length;
	char data[8];
}can_message;


void can_init();
void can_send_message(can_message *msg);
can_message *can_recive_msg(uint8_t buffer_number);
int can_clearIfInterrupted(void);




#endif