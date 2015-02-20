#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

int main()
{
    char *data1;
    int size, i;

    data1 = malloc (SIZE);
    printf ("Please input your eos username: ");
    scanf ("%s", data1);
	size = strlen(data1);
	data1 = (char *) realloc(data1, sizeof(char *) * size);
	data1[size] = '\0';
	for (i=0; i<size; i++)
		printf ("%c", data1[i]);
    printf ("\n");
    free (data1);
    return 0;
}
