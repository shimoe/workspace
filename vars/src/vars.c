/*
 ============================================================================
 Name        : vars.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void Print_Love(void){
	int c[4] = {76,79,86,69};
	int i;
	printf("all you need is ");
	for(i = 0;i < 4;i++){
		printf("%c",c[i]);
	}
	printf("\n");
}

void check1(void){
	char c;

	for (c = 1; c > 0; c++) {
		printf("%d\n",c);
	}

}

void check2(void){
	unsigned char c;

	for (c = 1; c > 0; c++) {
		printf("%d\n",c);
	}

}

void check3(void){
	short c;
	clock_t start = clock();
	for (c = 1; c > 0; c++) {
		printf("%d\n",c);
	}
	clock_t finish = clock();

	printf("workingtime = %f sec\n",(double)(finish-start)/CLOCKS_PER_SEC);
}


int main(void) {
	check1();
	printf("--------------------------------------");
	check2();
	printf("--------------------------------------");
	check3();
	printf("--------------------------------------");

	return 0;
}
