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
	char buffer[2048];
	int flag = 0;
	int readBytes = 0;
	int sumBytes = 0;

	//open fd
	fileFd = open(path, O_RDONLY);
	if(fileFd == -1){
		perror("Opening error");
		exit(EXIT_FAILURE);
	}
	//sleep reader for 15s
	sleep(15);
	//read buffer
	while(flag != 1){
		readBytes = 0;
		//open reading end
		readBytes = read(fileFd, buffer, sizeof(buffer));
		if(readBytes == -1){
			perror("Reading error");
			exit(EXIT_FAILURE);
		}
		//add sum of bytes read
		sumBytes += readBytes;
		if(readBytes == 0){
			flag = 1;
		}else{
			printf("Read bytes: %d\n", sumBytes);
		}
	}
	//close and unlink path
	if(close(fileFd) == -1){
		perror("Closing error");
		exit(EXIT_FAILURE);
	}
	unlink(path);
	return 0;
}
