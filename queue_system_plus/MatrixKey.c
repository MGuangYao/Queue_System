#include <REGX52.H>
#include <intrins.h>
#include "Delay.h"
#include "MatrixKey.h"

void delay_10us(unsigned int ten_us)
{
	while(ten_us --);
}

void Hc595SendByte(unsigned char dat)
{
   unsigned char i;
   SRCLK = 0;
	 RC = 0;
   for(i = 0; i < 8; i ++)
   {
		SER = dat >> 7;
		dat <<= 1;
		SRCLK = 1;
    _nop_();
		_nop_();
		SRCLK = 0;
   }
	 RC = 1;
	 _nop_();
	 _nop_();
	 RC = 0; 
}

unsigned int key_scan()
{
	static unsigned int key_code = 0;

	while(1)
	{
		while(KEY1 && KEY2 && KEY3 && KEY4 && KEY5 && KEY6 && KEY7 && KEY8);
		
		delay_10us(1000);
		
		if(!KEY1) key_code = KEY1_PRESS;
		else if(!KEY2) key_code = KEY2_PRESS;
		else if(!KEY3) key_code = KEY3_PRESS;
		else if(!KEY4) key_code = KEY4_PRESS;
		else if(!KEY5) key_code = KEY5_PRESS;
		else if(!KEY6) key_code = KEY6_PRESS;
		else if(!KEY7) key_code = KEY7_PRESS;
		else if(!KEY8) key_code = KEY8_PRESS;
		
		while(!KEY1 || !KEY2 || !KEY3 || !KEY4 || !KEY5 || !KEY6 || !KEY7 || !KEY8);
		
		delay_10us(1000);
		
		if(KEY1 && KEY2 && KEY3 && KEY4 && KEY5 && KEY6 && KEY7 && KEY8) break;
	}
	
	return key_code;		
}

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
			如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */
unsigned int MatrixKey()
{
	unsigned int KeyNumber=0;
	
	P1 = 0xFF;
	P1_3 = 0;
	if(P1_7 == 0){ Delay(20); while(P1_7==0); Delay(20); KeyNumber=1;}
	if(P1_6 == 0){ Delay(20); while(P1_6==0); Delay(20); KeyNumber=5;}
	if(P1_5 == 0){ Delay(20); while(P1_5==0); Delay(20); KeyNumber=9;}
	if(P1_4 == 0){ Delay(20); while(P1_4==0); Delay(20); KeyNumber=13;}
	
	P1 = 0xFF;
	P1_2 = 0;
	if(P1_7 == 0){ Delay(20); while(P1_7==0); Delay(20); KeyNumber=2;}
	if(P1_6 == 0){ Delay(20); while(P1_6==0); Delay(20); KeyNumber=6;}
	if(P1_5 == 0){ Delay(20); while(P1_5==0); Delay(20); KeyNumber=10;}
	if(P1_4 == 0){ Delay(20); while(P1_4==0); Delay(20); KeyNumber=14;}
	
	P1 = 0xFF;
	P1_1 = 0;
	if(P1_7 == 0){ Delay(20); while(P1_7==0); Delay(20); KeyNumber=3;}
	if(P1_6 == 0){ Delay(20); while(P1_6==0); Delay(20); KeyNumber=7;}
	if(P1_5 == 0){ Delay(20); while(P1_5==0); Delay(20); KeyNumber=11;}
	if(P1_4 == 0){ Delay(20); while(P1_4==0); Delay(20); KeyNumber=15;}
	
	P1 = 0xFF;
	P1_0 = 0;
	if(P1_7 == 0){ Delay(20); while(P1_7==0); Delay(20); KeyNumber=4;}
	if(P1_6 == 0){ Delay(20); while(P1_6==0); Delay(20); KeyNumber=8;}
	if(P1_5 == 0){ Delay(20); while(P1_5==0); Delay(20); KeyNumber=12;}
	if(P1_4 == 0){ Delay(20); while(P1_4==0); Delay(20); KeyNumber=16;}
	
	return KeyNumber;
}

