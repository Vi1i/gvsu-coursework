#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stack.h"

int main()
{
	stack s1;
	
	stackInit(&s1);
	
	stackPush(&s1, "Jag");
	stackPush(&s1, "Hans");
	stackPush(&s1, "John");
	stackPush(&s1, "500");
	
	printf("stack length: %d\n", stackLength(&s1));
	
	printf("stack popped: %s\n", stackPop(&s1));
	printf("stack popped: %s\n", stackPop(&s1));
	printf("stack popped: %s\n", stackPop(&s1));
	printf("stack length: %d\n", stackLength(&s1));
	
	// how to convert an int to a string?
	char *str = malloc(10 * sizeof(char));
	int num = 1250;
	sprintf(str, "%d", num);
	stackPush(&s1, str);
	
	str =  malloc(10 * sizeof(char));
	num = 1500;
	sprintf(str, "%d", num);
	stackPush(&s1, str);
	
	printf("stack length: %d\n", stackLength(&s1));
	printf("stack popped: %s\n", stackPop(&s1));
	printf("stack popped: %s\n", stackPop(&s1));
	printf("stack popped: %s\n", stackPop(&s1));
	
	stackDestroy(&s1);
	printf("stack length: %d\n", stackLength(&s1));
	
	return 0;
}
