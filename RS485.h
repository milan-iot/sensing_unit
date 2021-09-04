#ifndef _RS485_H
#define _RS485_H

#define RS485 Serial1

typedef enum {RS485_OFF, RS485_TX, RS485_RX} RS485_MODE;

void RS485_begin(RS485_MODE mode);
void RS485_setMode(RS485_MODE mode);

#endif
