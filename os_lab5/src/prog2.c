#include <stdio.h>
#include <dlfcn.h>

void usage(){
	printf("0. Переключение реализации контрактов\n\n");

	printf("1. Рассчет интеграла функции sin(x) на отрезке [A, B] с шагом e\n");
	printf("USAGE: Float SinIntegral(float A, float B, float e)\n\n");

	printf("2. Подсчёт количества простых чисел на отрезке [A, B] (A, B - натуральные)\n");
	printf("USAGE: Int PrimeCount(int A, int B)\n\n");
}



int main(){
	usage();

	int command;
	int version = 0;

	float (*SinIntegral)(float, float, float);
	int (*PrimeCount)(int, int);

	void *lib1_handler = dlopen("./lib1.so", RTLD_LAZY);
	void *lib2_handler = dlopen("./lib2.so", RTLD_LAZY);

	if (!lib1_handler || !lib2_handler){
	    fprintf(stderr,"dlopen() error: %s\n", dlerror());
	    return -1;
	}

	SinIntegral = dlsym(lib1_handler,"SinIntegral");
	PrimeCount = dlsym(lib1_handler,"PrimeCount");

	while(scanf("%d", &command) != EOF){
		switch(command){
			case 0:{
				version ^= 1;
				if(!version){
					SinIntegral = dlsym(lib1_handler,"SinIntegral");
					PrimeCount = dlsym(lib1_handler,"PrimeCount");
				} else{
					SinIntegral = dlsym(lib2_handler,"SinIntegral");
					PrimeCount = dlsym(lib2_handler,"PrimeCount");
				}
				printf("Switched to realization %d\n", version + 1);
				break;
			}
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
	dlclose(lib1_handler);
	dlclose(lib2_handler);
}
