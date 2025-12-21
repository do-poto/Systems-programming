#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	//vars declared
	int fileFd;
	char *path = "/tmp/fifoFile";
	char buffer[256];
	int bytesRead = 0;
	int flag = 0;

	//open file and check for error
	fileFd = open(path, O_RDONLY);
	if(fileFd == -1){
		perror("File opening error");
		exit(EXIT_FAILURE);
	}
	//read bytes from fifo
	while(flag != 1){
		bytesRead = 0;
		bytesRead = read(fileFd, buffer, sizeof(buffer));
		/*
		if(bytesRead == -1){
			perror("Reading error");
			exit(EXIT_FAILURE);
		}*/
		if(bytesRead == 0){
			flag = 1;
		}else{
			printf("%s\n", buffer);
		}
	}
	//close fifo
	if(close(fileFd) == -1){
		perror("Closing error");
		exit(EXIT_FAILURE);
	}
	unlink(path);
	return 0;
}
