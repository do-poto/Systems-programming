#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/mman.h>

//semaphore global var
sem_t accountLock;

//bank global var
int accountBalance1 = 1000;
int accountBalance2 = 3000;
int operation = 100;

//thread function
void* threadOperation(){
	//generate random int
	int randomInt = rand() % 5;

	//sleep for random time
	sleep(randomInt);

	//wait for semaphore to be available
	sem_wait(&accountLock);

	//sleep for random time
	sleep(randomInt);

	printf("Account state before operation: %d\n", accountBalance1);
	printf("Account state before operation: %d\n", accountBalance2);
	printf("---\n");

	//perform operation 10 times
	for(int i = 0; i < 10; i++){
		//copy to local var
		int tmp1 = accountBalance1;
		int tmp2 = accountBalance2;
		tmp1+=operation;
		tmp2-=operation;
		//update balance
		accountBalance1 = tmp1;
		accountBalance2 = tmp2;
	}

	printf("Account state after operation: %d\n", accountBalance1);
	printf("Account state after operation: %d\n", accountBalance2);
	printf("---\n");
	//release semaphore
	sem_post(&accountLock);
}


int main(){
	//threads
	pthread_t thread1, thread2, thread3;

	//initialize semaphore
	if(sem_init(&accountLock, 0, 1) == -1){
		perror("sem init error");
		exit(EXIT_FAILURE);
	}

	//initialize threads
	pthread_create(&thread1, NULL, &threadOperation, NULL);
	pthread_create(&thread2, NULL, &threadOperation, NULL);
	pthread_create(&thread3, NULL, &threadOperation, NULL);

	//wait for threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	//print final balance
	printf("==================================================\n");
	printf("Final account balance: %d\n", accountBalance1);
	printf("Final account balance: %d\n", accountBalance2);

	//destroy semaphore
	sem_destroy(&accountLock);

	return 0;
}

