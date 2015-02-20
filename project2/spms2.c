/****************************************************************
 * This program is a Process manager that creates servers       *
 * and takes user input to give commands to the servers.        *
 * It can create many servers, and each server can have many    *
 * processes.  This uses forking to make the servers, and       *
 * the servers processes.  Pipeing is used to keep contact      *
 * between the Process manager and there servers, and each      *
 * server and there processes.                                  *
 *                                                              *
 * @author Lawrence O'Boyle                                     *
 * @version 01/27/2015                                          *
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 100
#define READ 0
#define WRITE 1
#define MAX_SERVERS 100

//Signal handler for SIGCHLD not implemented for gfault toloerence
//void sig_chld(int sig, siginfo_t *si, void *context){
//   if(si->si_code != CLD_EXITED){
//   }
//}

struct Server{
    pid_t pid;
    char * name;
    int fd;
};

//These make sure the command is correct
int get_command(char * args[4]);
int check_command(int argc, char * args[4]);

//These allow for easy call for Struct Server
void add_server(struct Server * servers, pid_t pid, char * name, int fd);
int get_server_loc(struct Server * servers, char * server_name);
void delete_server(struct Server * servers, char * server_name);

//This is what puts the child() into process, taking care of the small details
void create_server(struct Server * servers, char * args[4]);

//This is the grancchild of the process Manager. This is what is created to die.
void grand_child(int fd);

//This is the child of the manager, this is what each server actually is.
void child(int min_proc, int max_proc, int fd);

//These are the commands to call to the server to issue the four commands
void abort_server(struct Server * servers, char * server_name);
void create_process(struct Server * servers, char * server_name);
void abort_process(struct Server * servers, char * server_name);
void display_status(struct Server * servers);

//This is what decides the final command call to said server
void issue_command(struct Server * servers, int argc, char * args[4]);

//This is the process manager, that parent of all!
void parent();

int main(void){
    parent();
    return 0;
}

int get_command(char * args[4]){
    char * token,
         input[COMMAND_LENGTH];

    int argc,
        z;

    printf("command: ");
    fflush(stdin);
    fgets(input, COMMAND_LENGTH, stdin);
    //This makes sure that the input has a null terminator
    for(z = 0; z < COMMAND_LENGTH; z++){
        if(input[z] == '\n'){
            input[z] = '\0';
            break;
        }
    }

    //The tokenizer loop and counter
    token = strtok(input, " ");
    argc = 0;
    while(token){
        args[argc] = malloc(sizeof(* args[argc]) * strlen(token) + 1);
        strcpy(args[argc], token);
        argc++;
        token = strtok(NULL, " ");
    }
    return argc - 1;
}

int check_command(int argc, char * args[4]){
    //This if statement acts as the switch to decide what command it is
    //and if it is correct for said command
    //Each command has a predefined int return that allows the
    //switch to correctly call the resulting command.
    if(strcmp(args[0], "createServer") == 0){
        //Checks for valid amount of arguments
        if(argc == 3){
            //Checks for correct min and max sizes
            if(strtol(args[1], NULL, 10) <= strtol(args[2], NULL, 10)){
                return 0;
            }else{
                printf("INVALID: ");
                printf("Min process is larger that max process!\n");
                return -1;
            }
        }else{
            printf("INVALID: ");
            printf("Incorrect amount of arguments!\n");
            return -1;
        }
    }else if(strcmp(args[0], "abortServer") == 0){
        //Checks for valid amount of arguments
        if(argc == 1){
            return 1;
        }else{
            printf("INVALID: ");
            printf("Incorrect amount of arguments!\n");
            return -1;
        }
    }else if(strcmp(args[0], "createProcess") == 0){
        //Checks for valid amount of arguments
        if(argc == 1){
            return 2;
        }else{
            printf("INVALID: ");
            printf("Incorrect amount of arguments!\n");
            return -1;
        }
    }else if(strcmp(args[0], "abortProcess") == 0){
        //Checks for valid amount of arguments
        if(argc == 1){
            return 3;
        }else{
            printf("INVALID: ");
            printf("Incorrect amount of arguments!\n");
            return -1;
        }
    }else if(strcmp(args[0], "displayStatus") == 0){
        //Checks for valid amount of arguments
        if(argc == 0){
            return 4;
        }else{
            printf("INVALID: ");
            printf("Incorrect amount of arguments!\n");
            return -1;
        }
    }
    printf("%s is not a command!\n", args[0]);
    return -1;
}

void add_server(struct Server * servers, pid_t pid, char * name, int fd){
    int z;
    //Loops through the servers and assigns the new server
    for(z = 0; z < MAX_SERVERS; z++){
        if(servers[z].pid == -1){
            servers[z].pid = pid;
            servers[z].name = name;
            servers[z].fd = fd;
            break;
        }
    }
}

void delete_server(struct Server * servers, char * server_name){
    int z;
    //Loops through servers and removes the server with the
    //equivalent name
    for(z = 0; z < MAX_SERVERS; z++){
        if(strcmp(servers[z].name, server_name) == 0){
            servers[z].pid = -1;
            servers[z].name = "";
            servers[z].fd = -1;
            break;
        }
    }
}

int get_server_loc(struct Server * servers, char * server_name){
    int z;
    //Looks for the location of the name in servers
    for(z = 0; z < MAX_SERVERS; z++){
        if(strcmp(servers[z].name, server_name) == 0){
            return z;
        }
    }
    return -1;
}

void create_server(struct Server * servers, char * args[4]){
    int fd[2];

    pid_t pid;

    int min = strtol(args[1], NULL, 10),
        max = strtol(args[2], NULL, 10);

    char * server_name = args[3];

    if (pipe(fd) < 0) { 
        perror ("plumbing problem"); 
        exit(1); 
    }

    if((pid = fork()) < 0){
        perror("fork bent");
        exit(1);
    }else if(!pid){
        child(min, max, fd[READ]);
        _exit(0);
    }else{
        //Adds the newly creates server to the array
        add_server(servers, pid, server_name, fd[WRITE]);
    }
}

void grand_child(int fd){
    int num;
    char str[3];
    for(;;){
        //Just waits for the parent;
        num = read(fd, (void *) str, (size_t) sizeof(str));
        if(num <= 0){
            printf("The plumbing broke\n");
            exit(1);
        }
        if(strcmp(str, "DIE") == 0){
            //kill(getpid(), SIGTERM);
            exit(0);
        }
    }
}

void child(int min_proc, int max_proc, int fd){
    //Wasn't implmented
    //would allow the server to recreat lost process
    //struct sigaction sigchld_action = {
    //    .sa_handler = SIG_DFL,
    //    .sa_flags = SA_NOCLDWAIT
    //};
    //sigaction(SIGCHLD, &sigchld_action, NULL);

    int live = 1,
        cur = 0,
        cmd,
        t_fd[2],
        c_fds[max_proc],
        num,
        status,
        z;

    pid_t pid,
          proc[max_proc];
    char str[10];

    //Initilizes the bookkeeping
    for(z = 0; z < max_proc; z++){
        proc[z] = -1;
        c_fds[z] = -1;
    }

    //Loops while alive
    while(live){
        //If the the process is less than min, create unti lthey are equal
        if(cur < min_proc){
            cmd = 0;
        }else{
            //Wait for command from Manager
            num = read(fd, (void *) str, (size_t) sizeof(str) + 1);

            if(num <= 0){
                printf("The plumbing broke\n");
                exit(1);
            }

            cmd = strtol(str, NULL, 10);
        }

        switch(cmd){
            //createProcess
            case 0:
                //Check if it can suppport all process
                if(cur < max_proc){
                    if(pipe(t_fd) < 0){
                        perror("plumbing problem\n");
                        exit(1);
                    }
                    if((pid = fork()) < 0){
                        perror("fork bent");
                        exit(1);
                    }else if(!pid){
                        grand_child(t_fd[READ]);
                        exit(0);
                    }else{
                        //Bookeeping
                        for(z = 0; z < max_proc; z++){
                            if(proc[z] == -1){
                                proc[z] = pid;
                                c_fds[z] = t_fd[WRITE];
                                break;
                            }
                        }
                        //Increment aomount of process
                        cur++; 
                    }
                }else{
                    printf("There are to many processes!\n");
                }
                break;

                //abortProcess
            case 1:
                //Check if there are process that can be killed
                if(cur > min_proc){
                    for(z = 0; z < max_proc; z++){
                        if(proc[z] != -1){
                            //Tells the Grandchild to die
                            write(c_fds[z], "DIE\0", 4);
                            //Waits
                            waitpid(proc[z], &status, 0);

                            //bookkeeping
                            proc[z] = -1;
                            c_fds[z] = -1;
                            break;
                        }
                    }
                    //Decrement aomount of process
                    cur--;
                }else{
                    printf("There are not enough processes to kill another!\n");
                }
                break;

                //dispalyStatus
            case 2:
                //finds all active process
                for(z = 0; z < max_proc; z++){
                    //Prints out the info if present
                    if(proc[z] != -1){
                        printf("\t   \\__pid#%d\n", proc[z]);
                    }
                }
                break;

                //abortServer
            case 3:
                //Stop loop
                live = 0;
                //Kill all process
                for(z = 0; z < max_proc; z++){
                    if(proc[z] != -1){
                        kill(proc[z], SIGTERM);
                    }
                }
                break;
        }
    }
}

void abort_server(struct Server * servers, char * server_name){
    char str[10] = "3";
    int loc,
        fd;
    //Get server loccation
    loc = get_server_loc(servers, server_name);
    //Get filedescriptor for said server
    fd = servers[loc].fd;

    //Issue command to server
    write(fd, (const void *) str, (size_t) strlen(str) + 1);
    //Remove server from its list
    delete_server(servers, server_name);
}

void create_process(struct Server * servers, char * server_name){
    char str[10] = "0";
    int loc,
        fd;
    //Get server loccation
    loc = get_server_loc(servers, server_name);
    //Get filedescriptor for said server
    fd = servers[loc].fd;

    //Issue command to server
    write(fd, (const void *) str, (size_t) strlen(str) + 1);
}

void abort_process(struct Server * servers, char * server_name){
    char str[10] = "1";
    int loc,
        fd;
    //Get server loccation
    loc = get_server_loc(servers, server_name);
    //Get filedescriptor for said server
    fd = servers[loc].fd;

    //Issue command to server
    write(fd, (const void *) str, (size_t) strlen(str) + 1);
}

void display_status(struct Server * servers){
    char str[10] = "2";
    int fd,
        z;
    printf("Process Manager\n");
    for(z = 0; z < MAX_SERVERS; z++){
        //All servers get issued display command
        if(servers[z].pid != -1){
            //Get filedescriptor
            fd = servers[z].fd;
            //Print info
            printf("\t\\__%s\n", servers[z].name);
            //Issue command to each server
            write(fd, (const void *) str, (size_t) strlen(str) + 1);
            //Momentart pause
            usleep(100);
            fflush(stdin);
        }
    }
}

void issue_command(struct Server * servers, int argc, char * args[4]){
    //Check/Get command
    switch(check_command(argc, args)){
        case -1:
            break;

        case 0:
            create_server(servers, args);
            break;

        case 1:
            abort_server(servers, args[1]);
            break;

        case 2:
            create_process(servers, args[1]);
            break;

        case 3:
            abort_process(servers, args[1]);
            break;

        case 4:
            display_status(servers);
            break;
    }
}

void parent(){
    struct sigaction sigchld_action = {
        .sa_handler = SIG_DFL,
        .sa_flags = SA_NOCLDWAIT
    };
    sigaction(SIGCHLD, &sigchld_action, NULL);

    int argc,
        z;

    char * args[4];
    struct Server servers[MAX_SERVERS];

    //Initilize the array
    for(z = 0; z < MAX_SERVERS; z++){
        servers[z].pid = -1;
        servers[z].name = "";
        servers[z].fd = -1;
    }

    //Loop for commands and issuing them
    for(;;){
        for(z = 0; z < 4; z++)
            args[z] = "";
        //Gets the command
        argc = get_command(args);
        //Check if it quits
        if(strcmp(args[0], "quit") == 0){
            kill(0, SIGTERM);
            break;
        }
        //Issue command
        issue_command(servers, argc, args);
        //Momentary pause
        sleep(1);
        fflush(stdin);
    }
}
