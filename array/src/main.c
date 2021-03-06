#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"
#include <stdlib.h>

#define BLACK 800
#define LENGTH 2
#define POWER 12000
#define DIFF 5000



int sensor(void);

int ary[LENGTH + 20];

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

int i = 0;
int t;

	while(1){
		t = sensor();
		switch(t){
		case 0:while(1){
			   int left,right;
			   left = ADRead(0);
			   right = ADRead(1);

			   Mtr_Run_lv(POWER,-POWER,0,0,0,0);
		       ary[i] = 1;
		       if(left < BLACK && right < BLACK){
		    	   int j,sum;
		    	   sum = 0;
		    	   if(i > LENGTH){
		    	   for(j = i - LENGTH;j < i;j++){
		    		   sum = sum + ary[j];
		    	   }
		    	   }
		    	  /* if(sum > LENGTH) */Mtr_Run_lv(0,0,0,0,0,0);
		       }
		       i++;
		       }
		case 1:Mtr_Run_lv(POWER+DIFF,-POWER+DIFF,0,0,0,0);
		       ary[i] = 0;
		       i++;
		case 2:Mtr_Run_lv(POWER-DIFF,-POWER-DIFF,0,0,0,0);
		       ary[i] = 0;
		       i++;
		}
if(i > sizeof(ary)) i = 0;

//if(tmp = (char*)realloc(ary,i+1) != NULL){
//ary = tmp;
//}
	}
	return 0;
}

int sensor(void){
	int right,left;

	left = ADRead(0);
	right = ADRead(1);

	if(left > BLACK && right > BLACK) return 0;
	else if(left > BLACK) return 1;
	else if(left < BLACK) return 2;
}
