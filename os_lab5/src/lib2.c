#include <math.h>

//Подсчет интеграла методом трапеций.
float SinIntegral(float A, float B, float e) {
    float dx = (B - A) / e;
    int steps = (B - A) / dx;
    float cur = A;
    float res = 0;
    for (int  i = 0; i < steps; ++i){
        res += dx * sin(sin(cur) + sin(cur + dx)) / 2;
        cur += dx;
    }
    res += (B - cur) * (sin(B) + sin(cur)) / 2;
    return res;
}

//Решето Эратосфена
int PrimeCount(int a, int b){
	int count = 0;
	int sieve[b + 1];
	for(int i = 0; i < b + 1; ++i){
		sieve[i] = 0;
	}
	sieve[0] = 1;
	sieve[1] = 1;
	for(int i = 2; i <= b; ++i){
		if(sieve[i] != 0){
			continue;
		}
		for(int j = 2 * i; j <= b; j += i){
			sieve[j] = 1;
		}
		++count;
	} 
	return count;
}