#include <stdio.h>
#include "functions.h"

void usage(){
	printf("1. Рассчет интеграла функции sin(x) на отрезке [A, B] с шагом e\n");
	printf("USAGE: Float SinIntegral(float A, float B, float e)\n\n");

	printf("2. Подсчёт количества простых чисел на отрезке [A, B] (A, B - натуральные)\n");
	printf("USAGE: Int PrimeCount(int A, int B)\n\n");
}

int main(){
	usage();
	int command;
	while(scanf("%d", &command) != EOF){
		switch(command){
			case 1:{
				float a, b, e;
				if(scanf("%f%f%f", &a, &b, &e) != 3){
					printf("Wrong arguements!\n");
					continue;
				} 
				printf("%f\n", SinIntegral(a, b, e));
				break;
			}
			case 2:{
				int a, b;
				if(scanf("%d%d", &a, &b) != 2){
					printf("Wrong arguements!\n");
					continue;
				} 
				printf("%d\n", PrimeCount(a, b));
				break;
			}
			default:{
				printf("Wrong command!\n");
			}
		}
	}
}