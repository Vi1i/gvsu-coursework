/********************************
 * This program is a process    *
 * manager who has the ability  *
 * to spawn off Servers and     *
 * Processes depending on the   *
 * command inputted by the user.*
 * It should be able to create  *
 * a Server, a process, and     *
 * abort either, and display    *
 * the status of the Proces     *
 * Manager.                     *
 *                              *
 * @author Megan Maher          *
 * @version 02/11/2015          *
 ********************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>



//Signal handler
void sig_handler(int signum);

struct Server{
  pid_t pid;
  char * name;
};

void process_manager();
void server();

int serverCmd = 0;

int main(int argc, char **argv){
    process_manager();
    exit(0);
}

void process_manager(){
    struct Server * servers = malloc(100 * sizeof *servers);
    pid_t pid;
    char * token;
    char input[100];
    char * cmd;
    char * args[3];
    int y,
        z;
    for(z = 0; z < 3; z++)
        args[z] = malloc(sizeof(input));

    signal(SIGINT, sig_handler);

    for(z = 0; z < 100; z++){
        servers[z].pid = -1;
        servers[z].name = "";
    }

    for(;;){
        printf("command: ");
        fflush(stdin);
        fgets(input, 100, stdin);
        for(z = 0; z < 100; z++){
            if(input[z] == '\n'){
                input[z] = '\0';
				//collects all the user input
                break;
            }
        }
        token = strtok(input, " ");

        z = 0;
        while(token){
            if(z > 3){
                printf("Too many arguments\n");
                z = 10;
                break;
            }else if(z > 0){
                y = z - 1;
                strcpy(args[y], token);
            }else{
                cmd = malloc(sizeof(* cmd) * strlen(token) + 1);
                strcpy(cmd, token);
            }
            z++;
            token = strtok(NULL, " ");
        }
        z--;
        if(z != 10){
            if(strcmp(cmd, "createServer") == 0){
                if(z == 3){
                    for(y = 0; y < 100; y++){
                        if(servers[y].pid == -1){
                            if((pid = fork()) < 0){
                                perror("fork failed");
                                exit(1);
                            }else if(!pid){//Child
                                server(atoi(args[0]), atoi(args[1]));
                                exit(0);
                            }else{//Parent
                                printf("%d\n", pid);
                                servers[y].pid = pid;
                                servers[y].name = args[2];
                            }
                            break;
                        }
                    }
                }else{
                    printf("Try again!\n");
                }
            }else if(strcmp(cmd, "abortServer") == 0){
                if(z == 1){
                    for(y = 0; y < 100; y++){
                        if(strcmp(servers[y].name, args[0]) == 0){
                            kill(servers[y].pid, SIGCHLD);
							//sends the single to abort the server
                            printf("%s, pid#%d, has been aborted.\n", servers[y].name, servers[y].pid);
                            servers[y].pid = -1;
                            servers[y].name = "";
                            break;
                        }
                    }
                }else{
                    printf("Wrong number of arguments! Ex:\n");
                    printf("\tabortServer serverName\n");
                }
            }else if(strcmp(cmd, "createProcess") == 0){
                if(z == 1){
                    for(y = 0; y < 100; y++){
                        if(strcmp(servers[y].name, args[0]) == 0){
                            kill(servers[y].pid, SIGUSR1);
							//sends the signal to create the process
                            printf("%s, pid#%d, has been notified to create a process.\n", servers[y].name, servers[y].pid);
                            break;
                        }
                    }
                }else{
                    printf("Wrong number of arguments! Ex:\n");
                    printf("\tcreateProcess serverName\n");
                }
            }else if(strcmp(cmd, "abortProcess") == 0){
                if(z == 1){
                    for(y = 0; y < 100; y++){
                        if(strcmp(servers[y].name, args[0]) == 0){
							//sends the signal to abort the process
                            kill(servers[y].pid, SIGUSR2);
                            printf("%s, pid#%d, has been notified to abort a process.\n", servers[y].name, servers[y].pid);
                            break;
                        }
                    }
                }else{
                    printf("Wrong number of arguments! Ex:\n");
                    printf("\tabortProcess serverName\n");
                }
            }else if(strcmp(cmd, "displayStatus") == 0){
                if(z == 0){
                    printf("This will display the status\n");
					//calls the system call to display the process tree
					system("ps f | grep _ | grep -Eiv defunct | grep -Eiv ps | grep -Eiv grep");

                }else{
                    printf("Wrong number of arguments! Ex:\n");
                    printf("\tdisplayStatus\n");
                }
            }else{
                printf("%s is an incorrect command!\n", cmd);
            }
        }
        free(cmd);
        sleep(1);
        fflush(stdin);
    }
    exit(0);
}

void server(int minProcs, int maxProcs){
    printf("Server created\n");
    
	//sets up the signals
	signal(SIGCHLD, sig_handler);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    signal(SIGINT, SIG_DFL);

    pid_t pid,
          proc[maxProcs + 10];
    int min = minProcs;
    int max = maxProcs;
    int cur = 0;
    int z;

    
    for(z = 0; z < (maxProcs + 10); z++)
        proc[z] = -1;

    while(1){
        if(serverCmd == 0 && cur > max){
            serverCmd = 2;
        }else if(serverCmd == 0 && cur < min){
            serverCmd = 1;
        }
        switch(serverCmd){
            case 1:
				//if sigUser is called, the process will be created
                serverCmd = 0;
                printf("Creating\n");
                if((pid = fork()) < 0){
                    perror("fork failed");
                    _exit(1);
                }else if(!pid){ //child
                    signal(SIGCHLD, sig_handler);
                    for(;;){
                    }
                }
                for(z = 0; z < (max + 10); z++){
                    if(proc[z] == -1){
                        proc[z] = pid;
                        break;
                    }
                }
                cur++;
                break;
            case 2:
				//if sigUser2 is called, then the process will abort
                serverCmd = 0;
                printf("Aborting process\n");
                for(z = 0; z < (max + 10); z++){
                    if(proc[z] != -1){
                        kill(proc[z], SIGINT);
						//sends the signal to abort the process 
                        while(kill(proc[z], 0) != -1){
                            usleep(100);
                            fflush(stdin);
                            kill(proc[z], SIGINT);
							//sends the signal to abort the process
                        } 
                        printf("Killed process %d\n", proc[z]);
                        proc[z] = -1;
                        cur--;
                        break;
                    }
                }
                
                break;
            default:
                break;
        }
    }
}

void sig_handler(int signum){
    if(signum == SIGUSR1){
        serverCmd = 1;
        signal(SIGUSR1, sig_handler);
    }
    if(signum == SIGUSR2){
        serverCmd = 2;
        signal(SIGUSR2, sig_handler);
    }
    if(signum == SIGCHLD){
        exit(0);
	}
    if(signum == SIGINT){
        printf(" Bye!\n");
        //kills all children of this process.
        kill(0, SIGCHLD);
        exit(0);
    }
}
