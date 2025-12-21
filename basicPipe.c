#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	//Declare vars
	int pipefd[2];
	pid_t pid;
	int userInput;
	int userOut;

	//Create pipe and check for error
	if(pipe(pipefd) == -1){
		perror("Pipe unsuccessful");
		exit(EXIT_FAILURE);
	}

	//Fork the process
	pid = fork();

	if(pid == -1){
		//Print message if error has occured
		perror("Fork unsuccessful");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		//Fork successful, instructions for child process
		//Close unused write end
		if(close(pipefd[1]) == -1){
			perror("Write end child closing error");
			exit(EXIT_FAILURE);
		}
		//Read from pipe
		if(read(pipefd[0], &userOut, sizeof(userOut)) == -1){
			perror("Reading error");
			exit(EXIT_FAILURE);
		}

		//Close read side
		if(close(pipefd[0]) == -1){
			perror("Read end child closing error");
			exit(EXIT_FAILURE);
		}
		//Print results
		printf("Process: %d\n", pid);
		printf("%d\n", userOut);
		exit(EXIT_FAILURE);
	} else{
		//Fork successsful, instructions for parent process
		//Close unused read side
		if(close(pipefd[0]) == -1){
			perror("Read end parent closing error");
			exit(EXIT_FAILURE);
		}

		//Get user input
		printf("Process %d\nInput int:\n", pid);
		scanf("%d", &userInput);

		//Write in pipe and close that end
		if(write(pipefd[1], &userInput, sizeof(userInput)) == -1){
			perror("Writing error");
			exit(EXIT_FAILURE);
		}
		if(close(pipefd[1]) == -1){
			perror("Write end parent closing error");
			exit(EXIT_FAILURE);
		}

		wait(NULL);
		printf("Pipe ended successfully\n");
		exit(EXIT_SUCCESS);
	}

	return 0;
}
