#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "unistd.h"
#include "vector.h"
#include "string.h"
/**/
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



int main(int argc, char* argv[]){
	int N;
	read(STDIN_FILENO, &N, sizeof(int));

	//fprintf(stderr, "child: %s\n\n", argv[0]);

	if(argc != 2){
		perror("Execl arguments error!\n");
		return -1;
	}
	

	int desc = open(argv[1], O_RDWR);
	if(desc < 0){
    	perror("Tmp file create error!\n");
		return -2;
	}

	int* fd = mmap(0, N*sizeof(int), 
    				PROT_WRITE,
    				MAP_SHARED,	desc, 0);
    if (fd == MAP_FAILED){
    	perror("Mmap error!\n");
		return -3;
    }
    /*
    FILE *F;
	F = fopen(argv[0], "w");
  	if (F == NULL){
		perror("File can't be opened!\n");
	}
	*/
    int file = open(argv[0], O_WRONLY);
	if(file == -1){
		perror("file error\n");
		return -1;
	}
	

	int delimoe, delitel;

	delimoe = fd[0];

	writing_int(delimoe, file);
	//fprintf(F, "%d ", delimoe);

	for (int i = 1; i < N; i++){
		delitel = fd[i];
		if (delitel == 0){
			//exit(1);
			return 1;
		}
		else{
			int res = delimoe / delitel;

			//fprintf(stderr, "child: %d / %d = %d\n", delimoe, delitel, res);
			writing_int(res, file);
			//fprintf(F, "%d ", res);

			fd[i] = res;
		}
	}//fprintf(stderr, "\n\n");

	close(file);
	//fclose(F);


    if(msync(fd, N*sizeof(int), MS_SYNC) < 0){
		perror("Msync error!");
		return -4;
	}
    if(munmap(fd, N*sizeof(int)) < 0){
		perror("Munmap error!");
		return -5;
	}

	//exit(0);
	return 0;
}