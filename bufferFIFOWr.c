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
	char sample[2048];
	int sentBytes;
	int bytesIn = 0;

	fileFd = open(path, O_WRONLY);
	while(1){
		sentBytes = write(fileFd, sample, sizeof(sample));
		if(sentBytes == -1){
			perror("Write error");
			exit(EXIT_FAILURE);
		}
		bytesIn+=sentBytes;
		printf("Bytes written: %d\n", bytesIn);
	}
	if(close(fileFd) == -1){
		perror("Closing error");
		exit(EXIT_FAILURE);
	}
	unlink(path);
	return 0;
}
