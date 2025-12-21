#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argsCount, char *args[]){
	//declare vars
	int pipeFd[2];
	pid_t pid;
	char *argsCom1[] = {args[1], NULL};
	char *argsCom2[] = {args[2], NULL};

	//create pipe and check for fail
	if(pipe(pipeFd) == -1){
		perror("Pipe failed");
		exit(EXIT_FAILURE);
	}
	//create fork and check for fail
	pid = fork();
	if(pid == -1){
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		//close write end
		if(close(pipeFd[1]) == -1){
			perror("Child write close error");
			exit(EXIT_FAILURE);
		}
		//read previous output
		if(dup2(pipeFd[0], 0) == -1){
			perror("dup2 read error");
			exit(EXIT_FAILURE);
		}
		//close read end
		if(close(pipeFd[0]) == -1){
			perror("Child read error");
			exit(EXIT_FAILURE);
		}
		//use second command
		execvp(args[2], argsCom2);
		exit(EXIT_FAILURE);
	}
	else{
		//close read end
		if(close(pipeFd[0]) == -1){
			perror("Parent close read end error");
			exit(EXIT_FAILURE);
		}
		//open writing end
		if(dup2(pipeFd[1], 1) == -1){
			perror("dup2 write error");
			exit(EXIT_FAILURE);
		}
		//close the write end
		if(close(pipeFd[1]) == -1){
			perror("Parent write end error");
			exit(EXIT_FAILURE);
		}
		//use first command
		execvp(args[1], argsCom1);
		wait(NULL);
		fflush(stdout);
		exit(EXIT_SUCCESS);
	}

	return 0;
}

