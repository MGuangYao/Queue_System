#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

#define queue_size 20

unsigned int KeyNum, Keyflag;

unsigned int queue[queue_size], st[queue_size];
unsigned int front = 0, rear = 0, length = 0, count_size = 0;


unsigned char code led_duan[17][8]={
{0x00,0x00,0x3e,0x41,0x41,0x41,0x3e,0x00},     //0
{0x00,0x00,0x00,0x21,0x7f,0x01,0x00,0x00},     //1
{0x00,0x00,0x27,0x45,0x45,0x45,0x39,0x00},     //2
{0x00,0x00,0x2a,0x49,0x49,0x49,0x36,0x00},     //3
{0x00,0x00,0x0c,0x14,0x24,0x7f,0x04,0x00},     //4
{0x00,0x00,0x72,0x51,0x51,0x51,0x4e,0x00},     //5
{0x00,0x00,0x3e,0x49,0x49,0x49,0x26,0x00},     //6
{0x00,0x00,0x40,0x40,0x4f,0x50,0x60,0x00},     //7
{0x00,0x00,0x36,0x49,0x49,0x49,0x36,0x00},     //8
{0x00,0x00,0x32,0x49,0x49,0x49,0x3e,0x00},     //9
{0x21,0x7F,0x01,0x3E,0x41,0x41,0x41,0x3E},     //10
{0x21,0x7F,0x01,0x00,0x21,0x7F,0x01,0x00},     //11
{0x21,0x7F,0x01,0x00,0x4F,0x49,0x79,0x00},     //12
{0x21,0x7F,0x01,0x00,0x49,0x49,0x7F,0x00},     //13
{0x21,0x7F,0x01,0x78,0x08,0x08,0x7F,0x08},     //14
{0x21,0x7F,0x01,0x00,0x79,0x49,0x49,0x4F},     //15
{0x21,0x7F,0x01,0x00,0x7F,0x49,0x49,0x4F},     //16
};  

unsigned char led_wei[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

void LCD_s()
{
	LCD_Init();
	LCD_ShowString(1, 1, "MGY_work:");
}

unsigned int judge(unsigned int num)
{
	unsigned int cnt = 0;
	while(num)
	{
		num /= 10;
		cnt ++;
	}
	return cnt;
}

unsigned int judge_col(unsigned int n)
{
	unsigned int wei = judge(n);
	if(wei == 1) return 16;
	else if(wei == 2) return 15;
	else if(wei == 0) return 16;
	else if(wei == 3) return 14;
	return 0;
}

void sing()
{
	unsigned int i = 500;
	while(i --)
	{
		BEEP = !BEEP;
		Delay(10);
	}
	BEEP = 0;
}

int query(unsigned int x)
{
	int i, idx = -1;
	for(i = 0; i < length; i ++) {
		if(x == queue[i] && st[i] == 1) {
			idx = i; 
		}
	}
	if(idx == -1) return -1;
	else return idx;
}

void enqueue(unsigned int x)
{
	if(count_size == queue_size) return;
	if(st[(rear + 1) % queue_size]) {
		LCD_ShowString(2, 1, "reserved");
		return;
	}
	
	queue[rear] = x;
	st[rear] = 1;
	
	rear = (rear + 1) % queue_size;
	length ++;
	count_size ++;
}

void dequeue()
{
	if(count_size == 0) return;
	
	queue[front] = 0;
	st[front] = 0;
	
	front = (front + 1) % queue_size;
	count_size --;
}

void display(unsigned int x)
{
	unsigned int i, CNT = 500;
	while(CNT --) {
		for(i = 0; i < 8; i ++) {
			P0 = led_wei[i];
			Hc595SendByte(led_duan[x][i]);
			delay_10us(50);				
			Hc595SendByte(0x00);
		}
		BEEP = !BEEP;
		Delay(10);
	}
	BEEP = 0;
}

void booking(unsigned int num)
{
	if(!st[num - 1]) {
		queue[num - 1] = 520;
		st[num - 1] = 1;
		count_size ++;
		LCD_ShowString(2, 1, "OK");
	}
	else LCD_ShowString(2, 1, "Sorry");
}

void show(unsigned int x)
{
	Keyflag = key_scan();
	
	if(Keyflag == KEY2_PRESS) {
		enqueue(x);
		LCD_ShowString(2, 1, "Queued");
	}
	else if(Keyflag == KEY3_PRESS) {
		if(query(x) == -1) LCD_ShowString(2, 1, "none");
		else {
			LCD_ShowNum(1, judge_col(query(x) + 1), query(x) + 1, judge(query(x) + 1));
			LCD_ShowString(2, 1, "Count");
			LCD_ShowNum(2, judge_col(count_size), count_size, judge(count_size));
		}
	}
	else if(Keyflag == KEY4_PRESS) booking(x);
	else if(Keyflag == KEY6_PRESS) {
		if(query(x) == -1) LCD_ShowString(2, 1, "none");
		else {
			LCD_ShowNum(1, judge_col(queue[query(x) + 1]), queue[query(x) + 1], judge(queue[query(x) + 1]));
			sing();
			//			display(queue[query(x) + 1]);
		}	
		dequeue();
	}
	else if(Keyflag == KEY7_PRESS) {
		if(query(x) == -1) LCD_ShowString(2, 1, "none");
		else {
			LCD_ShowNum(1, judge_col(queue[query(x)]), queue[query(x)], judge(queue[query(x)]));
			sing();
			//			display(queue[query(x)]);
		}
	}
	else if(Keyflag == KEY8_PRESS) {
		enqueue(x);
		dequeue();
		LCD_ShowString(2, 1, "Expired");
	}
}

void main()
{
	unsigned int i;
	for(i = 0; i < queue_size; i ++) st[i] = 0;
	
	LCD_s();

	while(1)
	{	
		if(count_size == 0) LCD_ShowString(2,1,"none");
		else LCD_ShowNum(2,judge_col(queue[front]),queue[front],judge(queue[front]));
		
		KeyNum = MatrixKey();
		
		switch(KeyNum)
		{
			case 1: LCD_s(); show(1); Delay(1000); LCD_s(); break;
			case 2: LCD_s(); show(2); Delay(1000); LCD_s(); break;
			case 3: LCD_s(); show(3); Delay(1000); LCD_s(); break;
			case 4: LCD_s(); show(4); Delay(1000); LCD_s(); break;
			case 5: LCD_s(); show(5); Delay(1000); LCD_s(); break;
			case 6: LCD_s(); show(6); Delay(1000); LCD_s(); break;
			case 7: LCD_s(); show(7); Delay(1000); LCD_s(); break;
			case 8: LCD_s(); show(8); Delay(1000); LCD_s(); break;
			case 9: LCD_s(); show(9); Delay(1000); LCD_s(); break;
			case 10: LCD_s(); show(10); Delay(1000); LCD_s(); break;
			case 11: LCD_s(); show(11); Delay(1000); LCD_s(); break;
			case 12: LCD_s(); show(12); Delay(1000); LCD_s(); break;
			case 13: LCD_s(); show(13); Delay(1000); LCD_s(); break;
			case 14: LCD_s(); show(14); Delay(1000); LCD_s(); break;
			case 15: LCD_s(); show(15); Delay(1000); LCD_s(); break;
			case 16: LCD_s(); show(16); Delay(1000); LCD_s(); break;
		}
	}
}
