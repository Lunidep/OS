#include <math.h>
#include <stdbool.h>
#include <stdio.h>

//Подсчет интеграла методом прямоугольников.
float SinIntegral(float A, float B, float e) {
    float dx = (B - A) / e;
    int steps = (B - A) / dx;
    float cur = A;
    float res = 0;
    for (int  i = 0; i < steps; ++i){
        res += dx * sin(cur + dx / 2);
        cur += dx;
    }
    res += (B - cur) * sin((B + cur) / 2);
    return res;
}

//Наивный алгоритм. Проверить делимость текущего числа на все предыдущие числа.
int PrimeCount(int a, int b){
	int count = 0;
	bool find_div = false;
	for(int i = a; i <= b; ++i){
		if (i == 0 || i == 1){
			find_div = true;
		}
		for(int j = i - 1; j > 1; --j){
			if(i % j == 0){
				find_div = true;
				break;
			}
		}
		if(!find_div) ++count;
		find_div = false;
	}
	return count;
}