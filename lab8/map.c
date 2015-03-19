#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SLEEPT  10

static char *init = "Meet your maker";
static char *un_init;

int main (int argc, char *argv[]){
    char * heap_var = (char *)malloc(500);
    int stack = 12;
    int stackA[80];
    printf ("address of init = \t\t%012p\n", &init);
    printf ("address of un_init = \t\t%012p\n", &un_init);
    printf ("address of heap start = \t%012p\n", sbrk(0));
    printf ("address of heap_varS = \t\t%012p\n", &heap_var[0]);
    printf ("address of heap_varE = \t\t%012p\n", &heap_var[499]);
    printf ("address of stackA[0] = \t\t%012p\n", &stackA[0]);
    printf ("address of stackA[79] = \t%012p\n", &stackA[79]);
    printf ("address of stack = \t\t%012p\n", &stack);

    printf ("memory map file: /proc/%d/maps\n", getpid());
    printf ("sleeping %d...", SLEEPT);
    fflush (NULL);
    sleep (SLEEPT);

    free(heap_var);


    printf ("\ndone\n");
    exit (EXIT_SUCCESS);
}
