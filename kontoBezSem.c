#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	//bank operation vars
	int operationNumber = atoi(argv[1]);
	int operationAmount = atoi(argv[2]);
	//sem and shm vars
	key_t key = 0;
	size_t sharedSize = sizeof(int);
	int sharedId;
	//other vars
	pid_t pid;

	//creation of shared memory
	sharedId = shmget(key, sharedSize, IPC_CREAT | IPC_EXCL | 0666);
	if(sharedId == -1){
		perror("Shared memory creation error");
		shmctl(sharedId, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}

	//create shared memory space
	int *sharedData = shmat(sharedId, NULL, 0);

	*sharedData = 1000;

	//create all children
	for(int i = 0; i < operationNumber; i++){
		pid = fork();
		if(pid == -1){
			perror("Forking error");
			shmctl(sharedId, IPC_RMID, NULL);
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			//access and modify data in shared memory
			int tmp = *sharedData;
			printf("Shared value: %d | Process: %d\n", tmp, getpid());
			tmp+=operationAmount;
			*sharedData = tmp;
			printf(">Opertaion performed by process: %d\n", getpid());
			exit(EXIT_SUCCESS);
		}
	}
	//wait for all children to finish
	for(int i = 0; i < operationNumber; i++){
		wait(NULL);
	}
	//print final result
	printf("===========================\n");
	printf("Saldo: %d\n", *sharedData);

	//delete shared memory
	if(shmdt(sharedData) == -1){
		perror("Shared memory detachment error");
		shmctl(sharedId, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}
	shmctl(sharedId, IPC_RMID, NULL);
	return 0;
}
