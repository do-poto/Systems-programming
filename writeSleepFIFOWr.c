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

	//open file and check for error
	fileFd = open(path, O_WRONLY);
	if(fileFd == -1){
		perror("Opening error");
		exit(EXIT_FAILURE);
	}
	//sleep write end for 10s
	sleep(10);
	//write to fifo
	if(write(fileFd, sample, strlen(sample) + 1) == -1){
		perror("Writing error");
		exit(EXIT_FAILURE);
	}
	//close fifo and unlink
	if(close(fileFd) == -1){
		perror("Closing error");
		exit(EXIT_FAILURE);
	}
	unlink(path);
	return 0;
}
