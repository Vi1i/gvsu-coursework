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
    int shmId,
         * p,
        p1;
    char * shmPtr,
         * ptr,
         * input[2];
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


 //locating the shared data segment
    if ((shmId = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror("shmget");
        exit(1);
    }

    //attaching the shared data segment
    if ((shmPtr = shmat(shmId, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    while(1) {
        p = (int *)shmPtr;
        ptr = shmPtr + sizeof (int) * 2;
        input[0] = ptr;
        ptr += *p++;
        input[1] = ptr;
        printf("I got:  %s\n", input[0]);
		sprintf(ptr, "0");
        sscanf(ptr, "%d", &p1);
        while(!p1)
            sscanf(ptr, "%d", &p1);
    }

    //detaching the shared memory segment
    if (shmdt(shmPtr) == -1) {
        perror("shmdt");
        exit(1);
    }

    exit(0);
}

void sig_handler(int signum, siginfo_t *si, void *context){
    write(0, " received.  Die? Didn't you know? Spartans never die.\n", 54);
    exit(0);
}


