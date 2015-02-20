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

    key = ftok("/etc/passwd", 'S'); /* Get a key */

    /* if a shared-memory segment exists, get it; otherwise, create one */
    if ((iShmId = shmget(key, 256, 0644 | IPC_CREAT)) < 0) {
        perror("get");
        exit(1);
    }

    /* Attach segment to process. Use an attach address of zero to
     * let the system find a correct virtual address to attach.
     */
    if ((cpShm = shmat(iShmId, 0, 0644)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    printf("Server is ready.\n");
    printf("Any message to continue, 'end' to exit\n");

    for (;;) {
        printf("Enter the message -> ");
        gets(cpShm);
        if (!strcmp(cpShm, "end")) {
            puts("Bye");
            shmdt(cpShm);   /* Detach segment */
            break;
        }
    }
}
