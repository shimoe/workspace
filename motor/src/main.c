// 課題: モーター実験
// 提出日: 2015/11/17
// 提出者: 14104064 下松八重　宏太

#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

void forword(int n);
void rotate_left(int deg);
void go_back(int len);
void triangle(int len);
void lect(int len);
void move_poly(int len,int n);

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	go_back(5);
	Wait(500);
	triangle(5);
	Wait(500);
	lect(5);
	Wait(500);
	move_poly(5,2);
	return 0;
}

void forword(int n){
	Mtr_Run_lv(10000,-10000,0,0,0,0);
	Wait(130*n);
	Mtr_Run_lv(0,0,0,0,0,0);
}

void rotate_left(int deg){
	Mtr_Run_lv(10000,10000,0,0,0,0);
	Wait(10*deg);
	Mtr_Run_lv(0,0,0,0,0,0);
}

void go_back(int len){
	forword(len);
	rotate_left(180);
	forword(len);
}

void triangle(int len){
	int i;
	for(i = 0;i < 3;i++){
		forword(len);
		rotate_left(60);
	}
}

void lect(int len){
	int i;
	for(i = 0;i < 4;i++){
		forword(len);
		rotate_left(90);
	}
}

void move_poly(int len,int n){
	int i;
	for(i = 0;i < n;i++){
		forword(len);
		rotate_left(360/n);
	}
}
