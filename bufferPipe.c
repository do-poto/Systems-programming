#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	//declare vars
	int pipeFd[2];
	pid_t pid;
	//vars for parent
	int written;
	char buff[2048];
	int bytesIn = 0;
	//vars for child
	int output;
	ssize_t readBuffer;
	int bytesOut = 0;

	//check if pipe was created successfully
	if(pipe(pipeFd) == -1){
		exit(EXIT_FAILURE);
	}

	//create a fork of the function
	pid = fork();

	//check if fork was successful
	if(pid == -1){
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		//close the write end
		if(close(pipeFd[1]) == -1){
			perror("Child write closing error");
			exit(EXIT_FAILURE);
		}
		//sleep process for 3 sec
		sleep(3);
		//print child info
		printf("Process: %d\n", pid);
		//read the data
		readBuffer = read(pipeFd[0], &output, sizeof(output));
		if(readBuffer == -1){
			perror("Reading error has occured");
			exit(EXIT_FAILURE);
		}
		bytesOut += readBuffer;
		//close read end of fd
		if(close(pipeFd[0]) == -1){
			perror("Child read closing error");
			exit(EXIT_FAILURE);
		}
		//print results
		printf("Read bytes: %d\n", bytesOut);
		exit(EXIT_FAILURE);
	}
	else {
		//close read end
		if(close(pipeFd[0]) == -1){
			perror("Parent read closing error");
			exit(EXIT_FAILURE);
		}
		//print starting info
		printf("Process: %d\n", pid);
		printf("Default pipe capacity: 65536\n");
		//write to fd to overload buffer
		while(1){
			written = write(pipeFd[1], buff, sizeof(buff));
			//checks for write errors
			if(written == -1){
				perror("Writing error");
				exit(EXIT_FAILURE);
			}
			//add bytes written to pipe
			bytesIn += written;
			//print status of bytes
			printf("Bytes sent: %d\n", bytesIn);
		}
		//close write end
		if(close(pipeFd[1]) == -1){
			perror("Parent write closing error");
			exit(EXIT_FAILURE);
		}

		wait(NULL);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
