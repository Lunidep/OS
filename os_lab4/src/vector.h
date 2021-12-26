#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct{
	int *buf;
	size_t size;
	size_t capacity;
} vector;

bool v_init(vector *v);
void v_destroy(vector *v);
int v_get(vector *v, size_t i);
void v_set(vector *v, size_t i, int val);
size_t v_get_size(vector *v);
size_t v_get_cap(vector *v);
bool empty(vector *v);

bool v_set_size(vector *v, size_t size);
int v_pop_back(vector *v);
bool v_push_back(vector *v, int val);
void v_print(vector *v);
#endif
