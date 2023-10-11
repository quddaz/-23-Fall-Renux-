#include <stdio.h>
#include <stdlib.h>
#define N 4;
#define MAXLINE 100;
#include <math.h>
#include <string.h>
void sinx_taylor(int num_elements, int terms, double* x, double* result)
{
	int fd[2*N]; //부모와 자식 2개씩
	char message[MAXLINE], line[MAXLINE];
	for(int i = 0; i<num_elements; i++){
		int* p_fd =&fd[2*i];
		pipe(p_fd);
	}
	int pid;
	for(int i=0; i<num_elements; i++){
		int* p_fd = &fd[2*i];
		if((pid = fork()) ==0){
			close(p_fd[0]);
			int my_id = i;
			double value = x[i];
			double numer= x[i] * x[i] * x[i];
			double denom= 6.; // 3!
			int sign = -1;
			for(int j=1; j<=terms; j++){
					value += (double)sign * numer/ denom;
					numer*= x[i] * x[i];
					denom*= (2.*(double)j+2.) * (2.*(double)j+3.);
					sign *= -1;
		}
		result[i] = value;
		sprintf(message, "%lf", result[i]);
		length = strlen(message)+1;
		write(p_fd[1], message, length);

		exit(my_id);
	} else 
		close(p_fd[1]);
		
	}
	int* stat; //아이디 받기
	for(int i =0; i<numelements; i++){
		wait(stat);
		int child_id = stat >> 8;
		read(fd[2*child_id], line, MAXLINE);

		double res = atof(line);
		result[child_id] = res;
	}
}