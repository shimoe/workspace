/*
 ============================================================================
 Name        : loop.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void loop1();
void loop2();
void loop3();
void loop4();
void loop5();
void loop6();

int main(void) {

	return EXIT_SUCCESS;
}

void loop1(){
	int i;
	for(i = 0;i < 10;i++){
		printf("%d/n",i);
	}
}

void loop2(){
	int i;
	for(i = 10;i < 20;i++){
		printf("%d/n",i);
	}
}

void loop3(){
	int i;
	for(i = 1;i <= 20;i++){
		if(i%2 == 1){
			printf("%d/n",i);
		}
	}
}

void loop4(){
	int i;
	for(i = 1;i <= 20;i++){
		if(i%2 == 0){
			printf("%d/n",i);
		}
	}
}

void loop5(){
	int i;
	for(i = 10;i > 0;i--){
		printf("%d/n",i);
	}
}

void loop6(int start,int end, int step,int mode){
	switch(mode){
	case 0:
		break;
	}
}
