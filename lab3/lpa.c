/******************************************************
 * lpa.c											  *
 *													  *
 * This is a program to demostrate signal handeling	  *
 * between the parent and child.  This program will	  *
 * fork, the parent will wait and catch SIGUSR		  *
 * signals and the ^C signal.  It will notify the	  *
 * different signals to the user, and continue on	  *
 * until ^C is thrown.  It will then die gracefully,  *
 * killing children proccess's.						  *
 *													  *
 * @author Lawrence O'Boyle && Megan Maher			  *
 * @version 01/27/2015								  *
 ******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

//Signal handler
void sig_handler(int signum);

int main(int argc, char **argv){
	int r;
	pid_t pid,
		  ppid;

	//Gets the parent pid
	ppid = getpid();
	
	//Seeds the rand()
	srand(time(NULL));

	//Forks once
	if((pid = fork()) < 0){//If there is an error forking
		perror("fork failed");
		exit(1);
	}else if(pid){//Parent
		sleep(1);
		fflush(stdout);
		//Set the signals to cathc and handle
		signal(SIGUSR1, sig_handler);
		signal(SIGUSR2, sig_handler);
		signal(SIGINT, sig_handler);

		//Infinite loop to keep the parent waiting for signals
		for(;;){
			printf("Waiting...   ");
			//Forces the stdout to be printed
			fflush(stdout);
			//Waits for a signal
			pause();
		}
	}else{
		//Prints off the child process pid
		printf("Spawned child process %ld\n", (long) getpid());

		//Sets signal to catch the parents kill signal
		signal(SIGCHLD, sig_handler);
		
		//Infinite loop for pauseing and sening signals
		for(;;){
			//Generates a number
			r = rand();
			//Sleeps for 1-5s
			sleep((r % 5) + 1);
			fflush(stdout);
			//Generates another number
			r = rand();
			if(r % 2 == 0){//if even
				kill(ppid, SIGUSR1);
			}else{//if odd
				kill(ppid, SIGUSR2);
			}
		}
		exit(0);
	}
	exit(0);
}

void sig_handler(int signum){
	if(signum == SIGUSR1){
		printf("Received a SIGUSER1 signal\n");
	}

	if(signum == SIGUSR2){
		printf("Received a SIGUSER2 signal\n");
	}

	if(signum == SIGCHLD){
		exit(0);
	}

	if(signum == SIGINT){
		printf(" received.  Die? Didn't you know? Spartans never die.\n");
		//Kills al children of this process.
		kill(0, SIGCHLD);
		exit(0);
	}
}
