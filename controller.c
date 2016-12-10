#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

int main(int args, char * argv[]){
	int semid;
	int shmid;
	int key = ftok("makefile", 192);
	int test;
	int fd;
	int len;

	if (strcmp( argv[1], "-c") == 0) {
		//printf("%d\n", key);
		shmid = shmget(key, 8, IPC_CREAT | 0644);
		//printf("%d\n",shmid );
		semid = semget(key, 1, IPC_CREAT | 0644);

		union semun sem;
		sem.val = 3;
		test = semctl(semid, 0, SETVAL, sem);
		//printf("%d\n",test );

		fd = open("stories", O_CREAT | O_TRUNC, 0644);
	}
	
	else if (strcmp(argv[1], "-v") == 0) {
		fd = open("stories", O_RDONLY , 0644);
		int character;
		FILE *story;
		story = fopen("stories", "r");
		if (story) {
    		while ( (character = getc(story)) != EOF ) putchar(character);
    		fclose(story);
		}
	}

	else if (strcmp(argv[1], "-r") == 0) {
		shmid = shmget(key, 8, 0);
		semid = semget(key, 1, 0);
		
		struct sembuf semb; //set sembuf
		semb.sem_num = 0;
		semb.sem_flg = SEM_UNDO;
		semb.sem_op = -3; //want to remove
		semop(semid, &semb, 1);

		test = semctl(semid, 0, IPC_RMID);
		//printf("%d\n", test);
		struct shmid_ds d;
		test = shmctl(shmid, IPC_RMID, &d);
		//printf("%d\n", test);
		
		
		fd = open("stories", O_RDONLY , 0644);
		int character;
		FILE *story;
		story = fopen("stories", "r");
		if (story) {
    		while ( (character = getc(story)) != EOF ) putchar(character);
    		fclose(story);
		}
	}

	return 0;
}
