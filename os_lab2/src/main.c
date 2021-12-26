/*
Попов Илья М80-206Б-20

Группа вариантов 1

Родительский процесс создает дочерний процесс. Первой строчкой пользователь в консоль
родительского процесса пишет имя файла, которое будет передано при создании дочернего
процесса. Родительский и дочерний процесс должны быть представлены разными программами.
Родительский процесс передает команды пользователя через pipe1, который связан с
стандартным входным потоком дочернего процесса. Дочерний процесс принеобходимости
передает данные в родительский процесс через pipe2. Результаты своей работы дочерний
процесс пишет в созданный им файл. Допускается просто открыть файл и писать туда, не
перенаправляя стандартный поток вывода.

3 вариант) Пользователь вводит команды вида: «число число число<endline>». Далее эти числа
передаются от родительского процесса в дочерний. Дочерний процесс производит деление
первого числа, на последующие, а результат выводит в файл. Если происходит деление на 0, то
тогда дочерний и родительский процесс завершают свою работу. Проверка деления на 0 должна
осуществляться на стороне дочернего процесса. Числа имеют тип int. Количество чисел может
быть произвольным.
*/

//gcc child.c string.c -o child
//gcc main.c vector.c string.c
//./a.out

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
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
	int fd1[2];
	int fd2[2];
	bool first = true;
	vector v;
	string file_name;
	
	if (first){//считывание имени файла и чисел (на первом заходе в родительский процесс)
	//////////////////////////////////////////////////////////////////////
		usage_str();
		s_init(&file_name);
		reading_filename(&file_name);
	//////////////////////////////////////////////////////////////////////
		usage_vect();
		int tmp = 0;
		v_init(&v);
		r_status stat = reading_int(&tmp);
		while(stat != R_ERROR){
			v_push_back(&v, tmp);
			if(stat == R_EOF){
				perror("\nUSAGE: «число число число<endline>»\n");
				return -8;
			} else if(stat == R_EOL){
				break;
			}
			tmp = 0;
			stat = reading_int(&tmp);
		}
		if (stat == R_ERROR){
			perror("Wrong value\n");
			return -9;
		}
		first = false;
	//////////////////////////////////////////////////////////////////////
	}else{//на втором заходе в родительский процесс удаляем созданную строку и вектор чисел
		s_destroy(&file_name);
		v_destroy(&v);
	}
	
	// Попытаемся создать pipe'ы
	if(pipe(fd1) < 0){// Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу
		perror("Pipe error\n");
		return -1;
	}
	if(pipe(fd2) < 0){// Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу
		perror("Pipe error\n");
		return -2;
	}
	
	// Порождаем новый процесс
	int pid = fork();
	if(pid < 0){ // Если создать процесс не удалось, сообщаем об этом и завершаем работу
		perror("Fork error\n");
		return -3;
	}
	
	//parent
	else if (pid > 0){ 
		printf("\n[%d] It's parent. Child id: %d\n\n", getpid(), pid);//----------------------------------------
		fflush(stdout);
		
		close(fd1[0]);//закрываем канал на чтение pipe1, т.к. мы в него пишем
		close(fd2[1]);//закрываем канал на запись pipe2, т.к. мы из него потом читать будем(после завершения работы дочернего прочеса)
		
		//запись в pipe1 от родителя ребенку
		
		////////////////////////////////////////////////
		//запись в pipe1 имени файла
		int len_str = s_get_size(&file_name);
		write(fd1[1], &len_str, sizeof(int));
		
		printf("Your file_name: %s\n", s_get_all(&file_name));//----------------------------------------
		
		for(int i = 0; i < len_str; i++){
			char x = s_get(&file_name, i);
			write(fd1[1], &x, sizeof(char));
		}
		
		///////////////////////////////////////////////
		//запись в pipe1 чисел
		int len = v_get_size(&v);
		write(fd1[1], &len, sizeof(int));
		
		printf("Your numbers: ");//----------------------------------------
		for (int i = 0; i < len; i++){
			int x = v_get(&v, i);
			printf("%d ", x);//----------------------------------------
			write(fd1[1], &x, sizeof(int));
		}printf("\n\n");

		//ждем завершение работы дочернего
		int status;
	    waitpid(pid, &status, 0);
	 
	    if (WIFEXITED(status)){
	        int exit_status = WEXITSTATUS(status);       
	        printf("Exit status of the child was %d\n", exit_status);//----------------------------------------
	        if (exit_status == 1){
	        	perror("You can't divide by zero!\n\n");
	        	return -8;
	        }
	        else if (exit_status != 0){
	        	perror("Some error!\n\n");
	        	return -9;
	        }
	    }
		
		printf("\n[%d] It's parent. Child id: %d\n\n", getpid(), pid);//----------------------------------------
		fflush(stdout);
		
		//читаем результат работы дочернего процесса из pipe2
		int res;
		int length;
		read(fd2[0], &length, sizeof(int));
		
		printf("Your processed numbers: ");//----------------------------------------
		
		while(length > 0){
			read(fd2[0], &res, sizeof(int));
			printf("%d ", res);
			length--;
		}
		printf("\n\n\n");//----------------------------------------
		
		close(fd1[1]);
		close(fd2[0]);//закрываем оставшиеся каналы у pipe1 и pipe2
	}
	
	
	//child
	else { 
		printf("[%d] It's child\n\n", getpid());//----------------------------------------
		fflush(stdout);
		
		close(fd1[1]);
		close(fd2[0]);//закрываем эти каналы, т.к. они нам не понадобятся, а два оставшихся закрывать смысла нет, т.к. после завершения функции execl() мы сразу вернемся в процесс parent
		
		//создем копии файловых дескрипторов
		if (dup2(fd1[0], STDIN_FILENO) == -1){//перенаправить pipe1 на консольный ввод дочернему процессу
			perror("Cannot dup reading channel of pipe1 to stdin\n");
			return -5;
		}
		if (dup2(fd2[1], STDOUT_FILENO)== -1){//перенаправить консольный вывод дочернего процесса в pipe2
			perror("Cannot dup recording channel of pipe2 to stdout\n");
			return -6;
		}
		if (execl("child", "", NULL) == -1){//заменяет текущий процесс, процессом, описанном в исп. файле
			perror("Execl child problem\n");
			return -7;
		}
	}
	
	
	return 0;
}
