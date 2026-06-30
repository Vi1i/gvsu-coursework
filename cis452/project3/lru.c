#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FRAMES 16
#define PAGES 64
#define PROCS 5

typedef struct frame {
    int pid;
    int page;
    int age;
} Frame;

typedef struct physMem {
    Frame frames[FRAMES];
} PhysMem;

typedef struct proc {
    int pages[PAGES];
    int fault;
    int memRefs;
} Proc;

typedef struct pcb {
    Proc procs[PROCS];
} PCB;

int getCurProc(char * pId);
void updatePCBFrame(int pid, int page, int frame);
int getPCBFrame(int pid, int page);
int openSlot();
void addToPhysMem(int frame, int pid, int page);
void step(int pid, int page);
void printPhysMem();
void viewProc(int pid);

int AGE;
PCB pcb;
PhysMem physMem;

int main(int argc, char ** argv){
    int y, z;
    FILE *fp;
    char c,
         pRef[6],
         pId[5];
    bool ref = false;
    int count = 0,
        curProc,
        page;

    //Initializes the PCB
    for(z = 0; z < PROCS; z++){
            pcb.procs[z].fault = 0;
            pcb.procs[z].memRefs = 0;
        for(y = 0; y < PAGES; y++){
            pcb.procs[z].pages[y] = -1;
        }
    }

    //Initializes the Physical Memory
    for(z = 0; z < FRAMES; z++){
        physMem.frames[z].pid = -1;
        physMem.frames[z].page = -1;
        physMem.frames[z].age = -1;
    }

    AGE = 0;

    if(argc != 2){
        printf("Incorrect usage!\n");
        return 1;
    }

    fp = fopen(argv[1],"r");

    if(fp == NULL){
        perror("Error while opening file.\n");
        return 1;
    }

    while((c = fgetc(fp)) != EOF){
        switch(c){
            case' ':
                break;

            case':':
                ref = true;
                pId[count] = '\0';

                curProc = getCurProc(pId);

                count = 0;

                break;

            case'\n':
                for(y = 0; y < 100; y++){
                    printf("\n");
                }
                page = (int)strtoul(pRef, (char **)0, 2);
                printf("%s is accessing page %d\n", pId, page);

                step(curProc - 1, page);
                ref = false;
                count = 0;
                printPhysMem();
                printf("Either press enter to continue,\n");
                printf("or press the number of the\n");
                printf("process to view: ");

                c = getchar();
                if(c != '\n'){
                    viewProc((int)strtoul(&c, (char**)0, 10));
                    printf("Press enter to continue\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    c = getchar();
                }

                break;

            default:
                if(ref){
                    pRef[count] = c;
                }else{
                    pId[count] = c;
                }
                count++;
                break;
        }
    }
    printf("\n");
    int pageCount;
    for(z = 0; z < PROCS; z++){
        pageCount = 0;
        for(y = 0; y < PAGES; y++){
            if(pcb.procs[z].pages[y] != -1){
                pageCount++;
            }
        }
        printf("\nP%d:\n", z + 1);
        printf("\tFaults: %d\n", pcb.procs[z].fault);
        printf("\tMemRefs: %d\n", pcb.procs[z].memRefs);
        printf("\tPage size: %d\n", pageCount);
    }
    printf("\n%d\n", AGE);
    fclose(fp);
    return 0;
}

int getCurProc(char * pId){
    char temp[4]; //Depends on the number of procs
    memcpy(temp, &pId[1], strlen(pId) - 1);
    temp[strlen(pId ) - 1] = '\0';
    return (int)strtoul(temp, (char**)0, 10);
}

//Will return a frame if it is use, otherwise returns -1
int getPCBFrame(int pid, int page){
    return pcb.procs[pid].pages[page];
}

//Updates the procss page to the frame
void updatePCBFrame(int pid, int page, int frame){
    pcb.procs[pid].pages[page] = frame;
}

//Checks for an openslot in physMem
int openSlot(){
    int z;
    for(z = 0; z < FRAMES; z++){
        if(physMem.frames[z].pid == -1){
            return z;
        }
    }
    return -1;
}

void addToPhysMem(int frame, int pid, int page){
    physMem.frames[frame].pid = pid; 
    physMem.frames[frame].page = page; 
    physMem.frames[frame].age = AGE;
}

int getOldestSlot(){
    int z;
    int frame = 0;
    int smallestAge = physMem.frames[0].age;
    for(z = 1; z < FRAMES; z++){
        if(physMem.frames[z].age < smallestAge){
            smallestAge = physMem.frames[z].age;
            frame = z;
        }
    }
    return frame;
}

//Step
void step(int pid, int page){
    AGE++;
    int frame;
    pcb.procs[pid].memRefs++;
    if((frame = getPCBFrame(pid, page)) == -1){
        pcb.procs[pid].fault++;
        //check if there are open slots
        if((frame = openSlot()) != -1){//Open slot
            addToPhysMem(frame, pid, page);//add to physMem
            updatePCBFrame(pid, page, frame);//updatePCB
        }else{//closed slot
            frame = getOldestSlot();

            int oldPid = physMem.frames[frame].pid;
            int oldPage = physMem.frames[frame].page;
            updatePCBFrame(oldPid, oldPage, -1);//update kicked slot

            addToPhysMem(frame, pid, page);//add to physMem && kicks old
            updatePCBFrame(pid, page, frame);//updatePCB
        }
    }else{//update physMem age
        addToPhysMem(frame, pid, page);//updates age
    }
}

void printPhysMem(){
    int z;
    printf("Frame\tProc\tPage\tAge\n");
    for(z = 0; z < FRAMES; z++){
        printf("%d\tP%d\t%d\t%d\n", z, physMem.frames[z].pid + 1, physMem.frames[z].page, physMem.frames[z].age);
    }
}
void viewProc(int pid){
    int z;
    printf("Process %d\n", pid);
    pid--;
    printf("Page\tFrame\n");
    for(z = 0; z < PAGES; z++){
        if(pcb.procs[pid].pages[z] != -1){
            printf("%d\t%d\n", z, pcb.procs[pid].pages[z]);
        }
    }
}
