#ifndef STRING_H_ 
#define STRING_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	int size;
	int capacity;
	char *buf;
} string;

void s_init(string *s);
void s_destroy(string *s);
int s_get_cap(string *s);
int s_get_size(string *s);//
bool s_is_empty(string *s);
bool s_grow_buf(string *s);
bool s_push(string *s, char new_char);
bool s_shrink_buf(string *s);
char s_pop(string *s);
char s_get(string *s, int i);
char* s_get_all(string *s);

#endif
