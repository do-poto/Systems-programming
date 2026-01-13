#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	//bank operation vars
	int operationNumber = atoi(argv[1]);
	int operationAmount = atoi(argv[2]);
	//sem and shm vars
	key_t keyShmOne = 0;
	key_t keyShmTwo = 1;
	key_t keySem = 1;
	int semId;
	int sharedIdOne;
	int sharedIdTwo;
	size_t sharedSize = sizeof(int);
	//sem ops vars set
	struct sembuf semOpsLock = {0, -1, 0};
	struct sembuf semOpsUnlock = {0, 1, 0};
	//other vars
	pid_t pid;

	//creation of shared memory for account one
	sharedIdOne = shmget(keyShmOne, sharedSize, IPC_CREAT | IPC_EXCL | 0666);
	if(sharedIdOne == -1){
		perror("Shared memory creation error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}
	//creation of second account
	sharedIdTwo = shmget(keyShmTwo, sharedSize, IPC_CREAT | IPC_EXCL | 0666);
	if(sharedIdTwo == -1){
		perror("Shared memory creation error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}

	//create shared memory space
	int *sharedDataOne = shmat(sharedIdOne, NULL, 0);
	*sharedDataOne = 1000;
	int *sharedDataTwo = shmat(sharedIdTwo, NULL, 0);
	*sharedDataTwo = 2000;

	//creation of semaphore
	semId = semget(keySem, 1, IPC_CREAT | IPC_EXCL | 0666);
	if(semId == -1){
		perror("Semaphore creation error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}
	//set value of semaphore
	if(semctl(semId, 0, SETVAL, 1) == -1){
		perror("Semaphore value setting error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}

	//create all children
	for(int i = 0; i < operationNumber; i++){
		pid = fork();
		int randomNum = rand() % 5;
		if(pid == -1){
			perror("Forking error");
			semctl(semId, 0, IPC_RMID, NULL);
			shmctl(sharedIdOne, IPC_RMID, NULL);
			shmctl(sharedIdTwo, IPC_RMID, NULL);
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			sleep(randomNum);
			//access and modify data in shared memory
			semop(semId, &semOpsLock, 1);
			sleep(randomNum);
			int tmpOne = *sharedDataOne;
			int tmpTwo = *sharedDataTwo;
			printf("Account one: %d | Account two: %d | Process: %d\n", tmpOne, tmpTwo, getpid());
			tmpOne+=operationAmount;
			tmpTwo-=operationAmount;
			*sharedDataOne = tmpOne;
			*sharedDataTwo = tmpTwo;
			printf(">Opertaion performed by process: %d\n", getpid());
			semop(semId, &semOpsUnlock, 1);
			exit(EXIT_SUCCESS);
		}
	}
	//wait for all children to finish
	for(int i = 0; i < operationNumber; i++){
		wait(NULL);
	}
	//print final result
	printf("===========================\n");
	printf("Saldo one: %d\n", *sharedDataOne);
	printf("Saldo two: %d\n", *sharedDataTwo);

	//detach shared memory
	if(shmdt(sharedDataOne) == -1){
		perror("Shared memory detachment error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}

	if(shmdt(sharedDataTwo) == -1){
		perror("Shared memory detachment error");
		semctl(semId, 0, IPC_RMID, NULL);
		shmctl(sharedIdOne, IPC_RMID, NULL);
		shmctl(sharedIdTwo, IPC_RMID, NULL);
		exit(EXIT_FAILURE);
	}

	//delete semaphore and shared memory
	semctl(semId, 0, IPC_RMID, NULL);
	shmctl(sharedIdOne, IPC_RMID, NULL);
	shmctl(sharedIdTwo, IPC_RMID, NULL);
	return 0;
}
