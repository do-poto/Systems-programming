#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(){
	//mkfifo vars
	int fileFd;
	char *path = "/tmp/mplayerFIFO";
	//mplayer inputs
	char pause[] = "pause\n";
	char quit[] = "quit\n";
	char forward[] = "seek 10\n";
	char back[] = "seek -10\n";
	//input vars
	char input[64];
	char inputChar;
	//flags
	int flag = 0;
	int pauseFlag = 0;
	//front end messages
	char topInfo[] = "=========================\n"
			"Mplayer front end\n";

	char infoCommands[] = 	"=========================\n"
			"List of valid commands:\n"
			"p - pause/unpause\n"
			"f - forward 10s\n"
			"b - backwards 10s\n"
			"q - quit\n"
			"i - list available commands\n"
			"=========================\n";
	//open fifo
	if(mkfifo(path, 0666) == -1){
		perror("MKFIFO error");
		exit(EXIT_FAILURE);
	}

	//open field descriptor and for error
	fileFd = open(path, O_WRONLY);
	if(fileFd == -1){
		perror("FD opening failed");
		exit(EXIT_FAILURE);
	}

	//print info when launchning program
	printf("%s", topInfo);
	printf("%s", infoCommands);

	//input loop
	while(flag == 0){
		printf("Provide input:");
		fgets(input, sizeof(input), stdin);
		inputChar = input[0];
		//decide what to send to mkplayer
		switch(inputChar){
			case 'p':
				if(pauseFlag == 0){
					//pause program
					if(write(fileFd, pause, strlen(pause)) == -1){
						perror("Writing error p - pausing");
						exit(EXIT_FAILURE);
					}
					printf("Pausing...\n");
					pauseFlag = 1;
				}else{
					//upause if already paused
					if(write(fileFd, pause, strlen(pause)) == -1){
						perror("Writing error p - resuming");
						exit(EXIT_FAILURE);
					}
					printf("Resuming...\n");
					pauseFlag = 0;
				}
				break;
			case 'q':
				//quit program
				if(write(fileFd, quit, strlen(quit)) == -1){
					perror("Writing error q");
					exit(EXIT_FAILURE);
				}
				printf("Quiting...\n");
				flag = 1;
				if(close(fileFd) == -1){
					perror("FIFO closing error");
					exit(EXIT_FAILURE);
				}
				break;
			case 'f':
				//go 10s forward
				if(write(fileFd, forward, strlen(forward)) == -1){
					perror("Writing error f");
					exit(EXIT_FAILURE);
				}
				break;
			case 'b':
				//go 10s backwards
				if(write(fileFd, back, strlen(back)) == -1){
					perror("Writing error b");
					exit(EXIT_FAILURE);
				}
				break;
			case 'i':
				printf("%s", infoCommands);
				break;
			default:
				printf("\nInvalid input, try again with valid input\n\n%s", infoCommands);
				break;
		}
	}

	unlink(path);
	return 0;
}
