#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3
#define KEY4_PRESS	4
#define KEY5_PRESS  5
#define KEY6_PRESS  6
#define KEY7_PRESS  7
#define KEY8_PRESS  8
#define KEY_UNPRESS	0	

sbit BEEP = P2^0;

sbit SRCLK=P2^6;
sbit RC=P2^7;
sbit SER=P2^5;

sbit KEY1 = P3^0;
sbit KEY2 = P3^1;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
sbit KEY5 = P3^4;
sbit KEY6 = P3^5;
sbit KEY7 = P3^6;
sbit KEY8 = P3^7;


void Hc595SendByte(unsigned char dat);
unsigned int MatrixKey();
void delay_10us(unsigned int ten_us);
unsigned int key_scan();

#endif
