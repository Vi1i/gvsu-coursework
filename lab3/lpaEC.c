/******************************************************
 * lpaEC.c											  *
 *													  *
 * This is a program to demostrate signal handeling	  *
 * with sig action, instead of the previous signal,	  *
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
#include <errno.h>
#include <signal.h>
#include <time.h>

void sig_handler(int sig, siginfo_t *si, void *context);
void child_process(pid_t ppid, struct sigaction sa);

int main(void){
	pid_t pid,
		  ppid;

	struct sigaction sa;

	sigset_t oldmask;

	//Sets up the signal handler
	sa.sa_handler = sig_handler;
	//Sets the flag
	sa.sa_flags = SA_RESTART|SA_SIGINFO;
	//Initializes the signals
	sigemptyset(&sa.sa_mask);

	//Gets the parent pid
	ppid = getpid();

	//Forks
	if((pid = fork()) < 0){//If fork fails
		perror("fork failed");
		exit(1);
	}else if(pid){//Parent
		if((pid = fork()) < 0){//If fork fails
			perror("fork failed");
			exit(1);
		}else if(!pid){//Child
			child_process(ppid, sa);
		}
		//Sets up the signal to listen to SIGINT
		//Sets up the signal to listen to SIGINT
		if(sigaction(SIGINT, &sa, NULL) == -1) {//If sigaction fails
			perror("sigaction");
			exit(1);
		}
	
		//Sets up the signal to listen to SIGUSR1
		if(sigaction(SIGUSR1, &sa, NULL) == -1) {//If sigaction fails
			perror("sigaction");
			exit(1);
		}
	
		//Sets up the signal to listen to SIGUSR2
		if(sigaction(SIGUSR2, &sa, NULL) == -1) {//If sigaction fails
			perror("sigaction");
			exit(1);
		}
		//Sleep for asecond to allow the child to print once
		sleep(1);
		fflush(stdout);
		
		printf("Waiting...   ");
		fflush(stdout);


		//Infinite loop to continously capture signals
		for(;;){
			//Waiting for signals
			sigprocmask (SIG_BLOCK, &sa.sa_mask, &oldmask);
			sigsuspend (&oldmask);
			sigprocmask (SIG_UNBLOCK, &sa.sa_mask, NULL);
		}
	}else{
		child_process(ppid, sa);
	}
	pause();

	return 0;
}

void sig_handler(int signum, siginfo_t *si, void *context){
	if(signum == SIGUSR1){
		printf("Received a SIGUSER1 signal from: PID#%d\n", si->si_pid);
		printf("Waiting...   ");
		fflush(stdout);
	}

	if(signum == SIGUSR2){
		printf("Received a SIGUSER2 signal from: PID#%d\n", si->si_pid);
		printf("Waiting...   ");
		fflush(stdout);
	}

	if(signum == SIGCHLD){
		exit(0);
	}

	if(signum == SIGINT){
		write(0, " received.  Die? Didn't you know? Spartans never die.\n", 54);
		//Kills al children of this process.
		kill(0, SIGCHLD);
		exit(0);
	}
}

void child_process(pid_t ppid, struct sigaction sa){
	int r;
	//Seeds the rand()
	srand(time(NULL) + getpid());
	//Prints off the child process pid
	printf("Spawned child process %ld\n", (long)getpid());
	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	for(;;){
		r = rand();
		sleep((r % 5) + 1);
		fflush(stdout);
		r = rand();
		if(r % 2 == 0){
			kill(ppid, SIGUSR1);
		}else{
			kill(ppid, SIGUSR2);
		}
	}
	
}
