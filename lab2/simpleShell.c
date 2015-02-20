/*
 * This program creates a simple shell, to show basic
 * usage with wait, exec, and fork families.  This
 * shell reports time spent on command and involuntary
 * context switches.
 *
 * Lawrence O'Boyle && Megan Maher
 * simpleShell.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(){
	char * prompt = "command: ",
		 input[100],
		 * temp,
		 *command[50];

	int count,
		pid,
		status;

	for(;;){
		//Prompt user
		printf("%s", prompt);
		//Gets user input
		fgets(input, 100, stdin);

		if(strcmp(input, "\n") != 0){
			temp = strtok(input, " \n");

			if(strcmp(temp, "quit") == 0)
				break;

			//Tokenizes the command
			count = 0;
			while (temp) {
				command[count] = temp;
  				temp = strtok(NULL, " \n");
				count++;
			}
			//Terminates the sequence
			command[count] = 0;

			//Forks the process
			pid = fork();
			if(pid < 0){
				perror("failed");
			}else if(pid){
				//Creates the measure of time and context switches.
				struct rusage *usage = malloc(sizeof(struct rusage));

				//Waits for the child and returns the measure
				wait4(pid, &status, 0, usage);

				//Prints the measure
				printf ("CPU time: %ld.%06ld sec\n",
						usage->ru_utime.tv_sec, usage->ru_utime.tv_usec);
				printf ("Involuntary Context Switches: %ld\n",
						usage->ru_nivcsw);

				//Free's up the usage memory
				free(usage);
			}else{
				//Executes the command
				if(execvp(command[0], command) < 0){
					//If error, prints out the ocmmand of cause and why.
					fprintf(stderr ,"%s: ", command[0]);
					perror("");

					return 1;
				}
			}
		}
	}
	return(0);
}
