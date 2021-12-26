#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"

void s_init(string *s){
	s->buf = NULL;
	s->size = 0;
	s->capacity = 0;
}

void s_destroy(string *s){
	s->size = 0;
	free(s->buf);
	s->capacity = 0;
	s->buf = NULL;
}

int s_get_cap(string *s){
	return s->capacity;
}

int s_get_size(string *s){
	return s->size;
}

bool s_is_empty(string *s){
	return s->size == 0;
}

bool s_grow_buf(string *s){
	int tmp = s->capacity * 3 / 2;
	if(!tmp){
		tmp = 10;
	}
	char *newd = realloc(s->buf, sizeof(char) * tmp);
	if(newd != NULL) {
		s->buf = newd;
		s->capacity = tmp;
		return true;
	}
	return false;
}

bool s_push(string *s, char new_char){
	if(s_get_size(s) == s_get_cap(s)){
		if(!s_grow_buf(s))
			return false;
	}
	s->buf[s_get_size(s)] = new_char;
	s->size++;
	return true;
}

bool s_shrink_buf(string *s){
	int tmp = s->capacity * 4 / 9;
	if(tmp < s_get_size(s)){
		return true;
	}
	char *newd = realloc(s->buf, sizeof(char) * tmp);
	if(newd != NULL) {
		s->buf = newd;
		s->capacity = tmp;
		return true;
	}
	return false;
}

char s_pop(string *s){
	char tmp = s->buf[s_get_size(s) - 1];
	s_shrink_buf(s);
	s->size--;
	return tmp;
}

char s_get(string *s, int i){
	return s->buf[i];
}

char* s_get_all(string *s){
	return s->buf;
}
