#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"


#define POWER 12000
#define DIFF 5000
#define BLACK 800
#define COUNT 2

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	int i = 0;


	int right,left;


	while(1){
		left = ADRead(0);
			right = ADRead(1);



		if(i >= COUNT){
			break;
		}

		if(left > BLACK && right > BLACK){
					while(1){
							Mtr_Run_lv(POWER,-POWER,0,0,0,0);
							LED(0);
							if(ADRead(0) < BLACK && ADRead(1) < BLACK){
								i++;
								LED(3);
								break;

							}
						}
				}
		else if(ADRead(0) > BLACK){
			Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0);
			LED(1);
		}
		else if(ADRead(0) < BLACK){
			Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0);
			LED(2);
		}


	}

	Mtr_Run_lv(0,0,0,0,0,0);
	LED(0);
	return 0;
}

