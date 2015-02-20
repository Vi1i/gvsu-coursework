#include <stdio.h>
#include <string.h>

#include "infix2postfix.h"

int main()
{
	int choice;
	char infix[50], *postfix;
	
	do {
		printf("(1) Convert infix to postfix expression and evaluate\n");
		printf("(2) Evaluate postfix expression\n");
		printf("(3) Quit\n");
		printf("Enter selection (1, 2, or 3): ");
		scanf("%d", &choice);
		getchar();  // read and ignore newline
		if (choice == 1) {
			printf("Enter Infix Expression: ");
			fgets(infix, 50, stdin);
			infix[strlen(infix) - 1] = '\0'; // trim off newline character
			postfix = infixToPostfix(infix);
			printf("Postfix: %s\n", postfix);
			printf("Evaluates to: %d\n", evaluatePostfix(postfix));
		} else if (choice == 2) {
			printf("Enter Postfix Expression: ");
			fgets(infix, 50, stdin);
			infix[strlen(infix) - 1] = '\0';
			printf("Evaluates to: %d\n", evaluatePostfix(infix));		
		} else if (choice == 3) {
			printf("Bye.\n");
		} else {
			printf("Invalid selection. Try again...\n");
		}
	} while (choice != 3);
	
	return 0;
}
