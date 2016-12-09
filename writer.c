#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>

int main() {
	int key = ftok("makefile", 192); //get key
	int shmid = shmget(key, 8, 0); //set share memory id
	int semid = semget(key, 1, 0); //set semaphore id
	int fd;

	struct sembuf semb; //set sembuf
	semb.sem_num = 0;
	semb.sem_flg = SEM_UNDO;
	semb.sem_op = -1; //want to read
	semop(semid, &semb, 1); //go down to read

	int* lsize = shmat(shmid, 0, 0); //retrieves size of last entry

	semb.sem_op = 1; //done reading
	semop(semid, &semb, 1); //go up

	fd = open("stories", O_RDWR, 0644); //open to read from it

	if ( *lsize ) {
		lseek(fd, -1 * (*lsize), SEEK_END); //sets CURR to appropriate spot
		char buffer[50]; //buffer to write into
		read(fd, buffer, *lsize); //write to buffer
		//buffer[*lsize - 1] = 0;
		printf("Last entry: %s", buffer); //print last entry
	}
	else printf("No previous entry.\n");
	
	char entry[50]; //
	fgets(entry, 50, stdin); //while there are still characters in stdin
	*lsize = strlen(entry); //increase length
	semb.sem_op = -3; //want to down to write
	semop(semid, &semb, 1); //down to write
	lseek(fd, 0, SEEK_END); //set CURR to end, to append
	write(fd, entry, strlen(entry)); //write to file

	semb.sem_op = 3; //want to go up entirely
	semop(semid, &semb, 1); //go up, other processes can now read

	return 0;
}

/*
Will attempt to access the resources using the semaphore
Once in, it should display the last line added to the file (the shared memory should contain the size of the last line).
Then prompt the use for the next line.
Once a new line is added, it should write that to the file, update the shared memory and then release the semaphore

Got controller working - KH
Finished writing writer.c, have not tested -KH
*/
