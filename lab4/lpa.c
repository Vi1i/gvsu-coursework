/******************************************************
 * lpa.c                                              *
 *                                                    *
 * This is a multi-threaded program that receives a   *
 * file access request from a client. This request is *
 * given to a Dispatch thread, who then spawns a      *
 * child thread and passes the simluated filename to  *
 * each child, or worker, thread.                     *
 * This program simluates the thread execution        *
 * manifested by a fileserver process.                *
 *                                                    *
 * @author Lawrence O'Boyle && Megan Maher            *
 * @version 02/04/2015                                *
 ******************************************************/

#include <pthread.h> 
#include <stdio.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <signal.h>

#define N_CPUS 4
#define NUM_THREADS (N_CPUS+1)

struct worker_data {
   int thread_num;
   char filename[100];
};

void sig_handler(int sig, siginfo_t *si, void *context);
void * dispatcher(void * args);
void * worker(void * filename);

int avail_ths[NUM_THREADS];
int count;
pthread_mutex_t lock;

int main(int argc, char ** argv){
	int status;
	void * rslt;
	pthread_t th1;
	if((status = pthread_create (&th1, NULL,  dispatcher, NULL)) != 0) { 
        fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
        exit (1); 
    }
	pthread_join(th1, &rslt);
	exit(0);
}

void * dispatcher(void * args){
	pthread_t th[NUM_THREADS];

	int place,
		status,
		z;
	char input[100];
	struct sigaction sa;

	//Sets up the signal handler
	sa.sa_handler = sig_handler;
	//Sets the flag
	sa.sa_flags = SA_RESTART|SA_SIGINFO;
	//Initializes the signals
	sigemptyset(&sa.sa_mask);

	//Sets up the signal to listen to SIGINT
	if(sigaction(SIGINT, &sa, NULL) == -1) {//If sigaction fails
		perror("sigaction");
		exit(1);
	}

	for(z = 0; z < NUM_THREADS; z++){
		avail_ths[z] = 1;
	}

	if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        exit(1);
    }

	for(;;){
		//Get filename and input
		printf("Please enter the file to retrieve: ");
		fgets(input, 100, stdin);
		for(z = 0; z < 100; z++){
			if(input[z] == '\n'){
				input[z] = '\0';
				break;
			}
		}	

		place = -1;
		for(z = 0; z < NUM_THREADS; z++){
			if(avail_ths[z]){
				place = z;
				break;
			}
		}
		if(place < NUM_THREADS && place >= 0){
			count++;
			struct worker_data *wd = malloc(sizeof(*wd));
			wd->thread_num = place;
			strcpy(wd->filename, input);

			if((status = pthread_create (&th[place], NULL,  worker, wd)) != 0) { 
    		    fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
    		    exit (1); 
    		}
			avail_ths[place] = 0;
		}else{
			printf("There are no available threads at this moment, please try again shortly\n");
			sleep(1);
			fflush(stdout);
		}
	}
}

void * worker(void * data){
	pthread_mutex_lock(&lock);
	struct worker_data *wd = (struct worker_data *) data;
	free(data);
	int r;
	if(!wd){
		pthread_exit((void *)1);
	}

	srand(time(NULL));
	r = (rand() % 100) + 1;
	if(r <= 20){
		r = (rand() % 4) + 7;
		sleep(r); //sleeping for 7-10 seconds
		fflush(stdout);
	}else{
		sleep(1);
		fflush(stdout);
	}
	printf("\nThread %d, File %s finished.", wd->thread_num, wd->filename);
	fflush(stdout);
	avail_ths[wd->thread_num] = 1;
	free(wd);
	pthread_mutex_unlock(&lock);
	pthread_exit((void *)0);
}

void sig_handler(int signum, siginfo_t *si, void *context){
	write(0, " received.  Die? Didn't you know? Spartans never die.\n", 54);
	printf("Files retrieved: %d\n", count);
	pthread_mutex_destroy(&lock);
	exit(0);
}
