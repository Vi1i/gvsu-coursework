#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

main()
{
    int iShmId; /* Segment id */
    char *cpShm; /* Pointer to the segment */
    key_t key; /* Segment key */
    char cBuf[16]; /* Read buffer */

    /* Get a key */
    key = ftok("/etc/passwd", 'S');

    /* Get shared memory id. If it does not exist, do *not* create it. */
    if ((iShmId = shmget(key, 256, 0644)) < 0) {
        perror("get");
        exit(1);
    }

    /* attach shared-memory segment to the process */
    if ((cpShm = shmat(iShmId, 0, 0644)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    printf("Client is ready\n");

    for (;;) {
        printf("Press enter to read the message -> ");
        gets(cBuf);
        printf("Got: \"%s\"\n", cpShm);

        /* Exit on the 'end': detach and remove segment */
        if (!strcmp(cpShm, "end")) {
            struct shmid_ds stShmId;

            puts("Bye");
            shmdt(cpShm);
            if (shmctl(iShmId, IPC_RMID, &stShmId)) {
                perror("shmctl");
                exit(1);
            }
            break;
        }
    }
}
