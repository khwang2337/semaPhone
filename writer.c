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
    
    
    
	return 0;
}

/*
Will attempt to access the resources using the semaphore
    Once in, it should display the last line added to the file (the shared memory should contain the size of the last line).
    Then prompt the use for the next line.
    Once a new line is added, it should write that to the file, update the shared memory and then release the semaphore
    
    Got controller working - KH
*/