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
	char sample[] = "Hello from write!";

	//create fifo file
	if(mkfifo(path, 0666) == -1){
		perror("MKFIFO error");
		exit(EXIT_FAILURE);
	}

	//open fifo file and check for error
	fileFd = open(path, O_WRONLY);
	if(fileFd == -1){
		perror("File opening error");
		exit(EXIT_FAILURE);
	}

	//write to fifo
	if(write(fileFd, sample, strlen(sample) + 1) == -1){
		perror("Writing error");
		exit(EXIT_FAILURE);
	}

	//close and unlink fifo
	if(close(fileFd) == -1){
		perror("Closing error");
		exit(EXIT_FAILURE);
	}
	unlink(path);
	return 0;
}
