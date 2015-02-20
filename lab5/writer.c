/******************************************************
 * writer.c                                           *
 *                                                    *
 * This program writes data inputted by the user      *
 * into shared memory. It continuously accepts user   *
 * input until the program is terminated/             *
 *                                                    *
 * @author Lawrence O'Boyle && Megan Maher            *
 * @version 02/12/2015                                *
 ******************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>


#define SIZE 4096

void sig_handler(int sig, siginfo_t *si, void *context);

int main (){
	int shmId;
	char * shmPtr,
		   input[100];
	key_t key = ftok("/", 47);
	struct sigaction sa;

    //Sets up the signal handler
    sa.sa_handler = sig_handler;
    //Sets the flag
    sa.sa_flags = SA_RESTART|SA_SIGINFO;
    //Initializes the signals
    sigemptyset(&sa.sa_mask);

    //Sets up the signal to listen to SIGINT
    if(sigaction(SIGINT, &sa, NULL) == -1) {//If sigaction fails
        perror("sigaction");
        exit(1);
    }
	
	//creating the shared memory segment 
	if ((shmId = shmget (key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror ("i can't get no..\n");
		exit (1);
	}
	
	//attaching the segment to the data space
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
		perror ("can't attach\n");
		exit (1);
	}

	char *p1, *p2, *pointer;
	int next[2], first, second;
   while(1)
   {
        printf("Enter input: ");
        fgets(input, sizeof(input), stdin);

	   //begin writing the data entered 
		pointer = shmPtr + sizeof(next);
		next[0] = 47;
		sprintf(pointer, input);
		pointer += next[0];
		p1 = pointer;
		next[1] = sprintf(pointer, "1") + 1;
		pointer += next[1];
		p2 = pointer;

		sprintf(pointer, "1");
		memcpy(shmPtr, &next, sizeof(next));

		sscanf(p1, "%d", &first);
		sscanf(p2, "%d", &second);

	    while(first && second){
			sscanf(p1, "%d", &first); 
			sscanf(p2, "%d", &second);
		}
   }

	//detaching the shared memory segment
	if(shmdt(shmPtr) == -1) {
		perror("shmdt");
        exit(1);
    }

	exit(0);
}



void sig_handler(int signum, siginfo_t *si, void *context){
    write(0, " received.  Die? Didn't you know? Spartans never die.\n", 54);
    exit(0);
}

