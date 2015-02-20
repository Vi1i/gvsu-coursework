/********************************************************
 * This program demostrates the semaphores ability to   *
 * lock variable use to one process upon request.       *
 *                                                      *
 * @author Lawrence O'Boyle && Megan Maher              *
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

void cr_s(int semId, int loop, long int * shmPtr, int swch);

#define SIZE 16
#define SEMNUM 1 /* Number of sem's made */

int main (int argc, char *argv[])
{
    int status, rc;
    long int loop, *shmPtr;
    int shmId;
    pid_t pid;

    /* For sem creation */
    int semId;

    if(argc == 2){
        loop = strtol(argv[1], NULL, 10);
    }
    // get value of loop variable (from command-line argument)

    //Get the chared memory
    if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    //Attache the memory
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    //Get the semaphore
    semId = semget(IPC_PRIVATE, SEMNUM, IPC_CREAT | 00600);
    if (semId == -1) {
        perror("main: semget");
        exit(1);
    }

    //Set the initial value of the semaphore
    rc = semctl(semId, 0, SETVAL, 1);
    if (rc == -1) {
        perror("main: semctl");
        exit(1);
    }

    //Initialize the pointer
    shmPtr[0] = 0;
    shmPtr[1] = 1;

    //Fork
    if (!(pid = fork())) {//Child
        cr_s(semId, loop, shmPtr, 0);
        exit(0);
    }else{//Parent
        cr_s(semId, loop, shmPtr, 1);
    }

    //Parent wiats for child
    wait (&status);
    semctl(semId, 0, IPC_RMID); //removes semaphore reference
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    //Remove the attachement
    if (shmdt(shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    //Deallocates the shared memory
    if (shmctl(shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
    return 0;
} 

void cr_s(int semId, int loop, long int * shmPtr, int swch){
    int z,
        swp1,
        swp2;
    long int temp;
    struct sembuf sem_op;

    //Decide if it is parent or child
    if(swch){
        swp1 = 1;
        swp2 = 0;
    }else{
        swp1 = 0;
        swp2 = 1;
    }

    for(z = 0; z < loop; z++) {
        /* Get access rights */
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(semId, &sem_op, 1);

        temp = shmPtr[swp1];
        shmPtr[swp1] = shmPtr[swp2];
        shmPtr[swp2] = temp;
        // swap the contents of shmPtr[1] and shmPtr[0]

        /* release semaphore */
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(semId, &sem_op, 1);
    }
}
