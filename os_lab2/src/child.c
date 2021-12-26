#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "unistd.h"
#include "vector.h"
#include "string.h"

void write_num(int a, int fd) {
	char* num;
	if (a == 0) num = "0";
	if (a == 1) num = "1";
	if (a == 2) num = "2";
	if (a == 3) num = "3";
	if (a == 4) num = "4";
	if (a == 5) num = "5";
	if (a == 6) num = "6";
	if (a == 7) num = "7";
	if (a == 8) num = "8";
	if (a == 9) num = "9";
	if (a == -1) num = " ";
	write(fd, num, sizeof(char));
}

int pow_ten(int l){
	int res = 1;
	while (l > 0){
		res *= 10;
		l--;
	}
	return res;
}

int length_int(int cur){
    int tmp = cur;

    int c = 0;

    while (tmp > 0){
        tmp /= 10;
        c++;
    }
    return c;
}

void writing_int(int cur, int fd){

    int l = length_int(cur) - 1;
    
	while (cur > 0){
	    int tmp = cur;
    	int c = tmp / pow_ten(l);

    	write_num(c, fd);
    	
    	cur = cur % pow_ten(l);
    	l--;
	}
	write_num(-1, fd);
}

int main(){
	///////////////////////////////////////////////
	
	int len_str;
	read(STDIN_FILENO, &len_str, sizeof(int));
	
	string file_name;
	s_init(&file_name);
	for (int i = 0; i < len_str; i++){
		char x;
		read(STDIN_FILENO, &x, sizeof(char));
		s_push(&file_name, x);
	}
	s_push(&file_name, '\0');
	

	int file = open(s_get_all(&file_name), O_WRONLY);
	if(file == -1){
		s_destroy(&file_name);
		return -1;
	}
  	s_destroy(&file_name);
  	


	///////////////////////////////////////////////
	int x, delimoe, delitel;
	int len;
	read(STDIN_FILENO, &len, sizeof(int));
	write(STDOUT_FILENO, &len, sizeof(int));
	
	bool first = true;
	while (len > 0){
		read(STDIN_FILENO, &x, sizeof(int));
		if (first){
			delimoe = x;
			first = false;
			
			writing_int(delimoe, file);
			write(STDOUT_FILENO, &delimoe, sizeof(int));
		}
		else{
			delitel = x;
			if (delitel == 0){
				exit(1);
			}

			int res = delimoe / delitel;
			
			writing_int(res, file);
			write(STDOUT_FILENO, &res, sizeof(int));
		}
		len--;
	}
	
	close(file);
	return 0;
}
