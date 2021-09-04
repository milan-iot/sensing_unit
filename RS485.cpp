#include "RS485.h"
#include <Arduino.h>

//RS-485 pins
#define RE  25
#define DE  33

#define RS485_RX_ON  digitalWrite(RE, LOW)  //receiver enable (active LOW)
#define RS485_RX_OFF digitalWrite(RE, HIGH)
#define RS485_TX_ON  digitalWrite(DE, HIGH) //driver enable (active HIGH)
#define RS485_TX_OFF digitalWrite(DE, LOW)

void RS485_begin(RS485_MODE mode)
{
  Serial1.begin(115200, SERIAL_8N1, 26, 32); //RXD1=26, TXD1=32
  RS485_setMode(mode);
  RS485.readString();
}

void RS485_setMode(RS485_MODE mode)
{
  switch (mode)
  {
    case RS485_OFF:
      pinMode(RE, INPUT);
      pinMode(DE, INPUT);
      break;
    case RS485_TX:
      pinMode(RE, OUTPUT);
      pinMode(DE, OUTPUT);
      RS485_RX_OFF;
      RS485_TX_ON;
      break;
    case RS485_RX:
      pinMode(RE, OUTPUT);
      pinMode(DE, OUTPUT);
      RS485_RX_ON;
      RS485_TX_OFF;
      break; 
  }
}
