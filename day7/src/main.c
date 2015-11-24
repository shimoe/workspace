#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"



int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	return 0;
}

