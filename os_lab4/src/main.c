//gcc lol_.c string.c -o child
//gcc lol_m.c vector.c string.c
//./a.out

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include "unistd.h"
#include "vector.h"
#include "string.h"

typedef enum{
	R_SUCCESS,
	R_EOL,
	R_EOF,
	R_ERROR,
} r_status;

r_status reading_int(int *cur){//чтение чисел типа int с STDIN
	char c;
	*cur = 0;
	int tmp = read(STDIN_FILENO, &c, sizeof(char));
	while(tmp > 0){
		if(c == '\n') return R_EOL;
		if(c == ' ') break;
		if((c < '0') || (c > '9')){
			return R_ERROR;
		}
		*cur = *cur * 10 + c - '0';
		tmp = read(STDIN_FILENO, &c, sizeof(char));
	}
	if(tmp == 0) return R_EOF;
	return R_SUCCESS;
}

void reading_filename(string* str){
    	char cur;
    	while(read(STDIN_FILENO, &cur, sizeof(char)) > 0){
    		if(cur == '\n'){
    			break;
    		}
    		s_push(str, cur);
    	}
}

void usage_str(){
	printf("Введите имя файла, в который будет записан результат работы программы.\n\n");//----------------------------------------
}
void usage_vect(){
	printf("\n\nВведите числа в виде «число число число<endline>».\nРезультат работы программы - набор чисел - часных от деления первого введеного числа на последующие.\n\n");//----------------------------------------
}


int main(){

	bool first = true;
	vector v;
	string file_name;

	//считывание имени файла и чисел (на первом заходе в родительский процесс)
	if (first){
		usage_str();
		s_init(&file_name);
		reading_filename(&file_name);
		usage_vect();
		int tmp = 0;
		v_init(&v);
		r_status stat = reading_int(&tmp);
		while(stat != R_ERROR){
			v_push_back(&v, tmp);
			if(stat == R_EOF){
				perror("\nUSAGE: «число число число<endline>»\n");
				return -1;
			} else if(stat == R_EOL){
				break;
			}
			tmp = 0;
			stat = reading_int(&tmp);
		}
		if (stat == R_ERROR){
			perror("Wrong value!\n");
			return -2;
		}
		first = false;
	//на втором заходе в родительский процесс удаляем созданную строку и вектор чисел
	}else{
		s_destroy(&file_name);
		v_destroy(&v);
	}
	int N = v_get_size(&v);

	//////////////////////////////////////////////////////////////////////

	//создание уникального временного файла
    char template[] = "/tmp/tmpXXXXXX";
    int desc = mkstemp(template);
    if(desc < 0){
    	perror("Tmp file create error!\n");
		return -3;
    }
	if(ftruncate(desc, N*sizeof(int)) < 0){
		perror("Tmp file filling error!\n");
		return -4;
	}

	//////////////////////////////////////////////////////////////////////

	//создание пайпа, в котором дочернему процессу передается кол-во чисел N, введенных пользователем, чтобы он знал, временный файл какого размера ему необходим
	int fd1[2];
	if(pipe(fd1) < 0){
		perror("Pipe error\n");
		return -5;
	}


	int pid = fork();

	if(pid == 0){ //child
		//fprintf(stdout, "\n[%d] It's child\n\n", getpid());
		//fflush(stdout);

		close(fd1[1]);
		
		//перенаправить pipe1 на консольный ввод дочернему процессу
		if (dup2(fd1[0], STDIN_FILENO) == -1){
			perror("Dupe error!\n");
			return -6;
		}

		//заменяет текущий процесс, процессом, описанном в исп. файле
		if(execl("child", s_get_all(&file_name), template,  NULL) == -1){
			perror("Execl error!");
			return -7;
		}

	} else{ //parent

		//fprintf(stdout, "\n[%d] It's parent. Child id: %d\n\n", getpid(), pid);
		//fflush(stdout);

		//запись числа N в pipe1
		close(fd1[0]);
		write(fd1[1], &N, sizeof(int));
		close(fd1[1]);


		/*
		fprintf(stdout, "Your file_name: %s\n", s_get_all(&file_name));
		fprintf(stdout, "Your numbers: ");
	    for (int i = 0; i < N; i++){
	    	fprintf(stdout, "%d ", v_get(&v, i));
		}
		fprintf(stdout, "\n\n");
		*/


		int desc = open(template, O_RDWR);
		if(desc < 0){
	    	perror("Tmp file create error!\n");
			return -8;
		}

		int* fd = mmap(NULL, N*sizeof(int), 
    				PROT_READ | PROT_WRITE,
    				MAP_SHARED, desc, 0);
		if (fd == MAP_FAILED){
	    	perror("Mmap error!\n");
			return -9;
	    }

	    //запись во временный файл
		for (int i = 0; i < N; i++){
			int x = v_get(&v, i);
			fd[i] = x;
		}

	    if(msync(fd, N*sizeof(int), MS_SYNC) < 0){
			perror("Msync error!");
			return -10;
		}

	    //ждем завершение работы дочернего
   		
		
printf("3\n");	
	
		int status;
		wait(&status);
		int exit_status = WEXITSTATUS(status);
		fprintf(stdout,"Exit status of the child was %d\n\n", exit_status);
	    if (exit_status == 1){
        	perror("You can't divide by zero!\n\n");
        }

		
printf("3\n");
/*
		int status;
	    waitpid(pid, &status, 0);

	    if (WIFEXITED(status)){
	        int exit_status = WEXITSTATUS(status);
	        //fprintf(stdout,"Exit status of the child was %d\n\n", exit_status);
	        if (exit_status == 1){
	        	perror("You can't divide by zero!\n\n");
	        	//fprintf(stdout, "You can't divide by zero!\n\n");
	        	return -11;
	        }
	    }
*/	    

	    unlink(template);
/*
		fprintf(stdout, "[%d] It's parent. Child id: %d\n\n", getpid(), pid);
		fflush(stdout);

		fprintf(stdout, "Your processed numbers: \n");
		for(int i=0; i < N; i++){
	    	fprintf(stdout, "%d ", fd[i]);
	    }fprintf(stdout, "\n\n");
*/
	    if(munmap(fd, N*sizeof(int)) < 0){
			perror("Munmap error!");
			return -12;
		}
		close(desc);
	}
	return 0;
}