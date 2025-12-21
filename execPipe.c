#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	//declare vars
	int pipeFd[2];
	pid_t pid;
	char funOut[4096];
	char *argsLs[] = {"ls", "-a", NULL};
	ssize_t reader;

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
			perror("Child write end closing error");
			exit(EXIT_FAILURE);
		}
		//read output from pipe
		if(dup2(pipeFd[0], STDIN_FILENO) == -1){
			perror("dup2 reading error");
			exit(EXIT_FAILURE);
		}
		//read function output
		if(read(STDIN_FILENO, funOut, sizeof(funOut) - 1) == -1){
			perror("Reading error");
			exit(EXIT_FAILURE);
		}
		//close read end
		if(close(pipeFd[0]) == -1){
			perror("Child read end cloding error");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}
	else{
		//close read end of pipe
		if(close(pipeFd[0]) == -1){
			perror("Parent read end closing error");
			exit(EXIT_FAILURE);
		}
		//send output to write end of fd
		if(dup2(pipeFd[1], STDIN_FILENO) == -1){
			perror("dup2 writing error");
			exit(EXIT_FAILURE);
		}
		//close write end
		if(close(pipeFd[1]) == -1){
			perror("Parent write end closing error");
			exit(EXIT_FAILURE);
		}

		//execute ls
		execvp("ls", argsLs);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}

	return 0;
}

