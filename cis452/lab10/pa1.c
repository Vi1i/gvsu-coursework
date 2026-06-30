#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define I 0
#define S 1


int main(int argc, char *argv[]) {
    struct stat statBuf;
    DIR *dirPtr;
    struct dirent *entryPtr;
    char buf[512];
    int count,
        pos;
    bool flags[2];

    flags[I] = false;
    flags[S] = false;

    if (argc < 2) {
        dirPtr = opendir (".");
    }else{
        if(argc > 1){
            count = 1;
            while(count < argc){
                if(strlen(argv[count]) > 1 && argv[count][0] == '-'){
                    pos = 1;
                    while(pos < strlen(argv[count])){
                        switch(argv[count][pos]){
                            case 'i':
                                flags[I] = true;
                                break;
                            case 's':
                                flags[S] = true;
                                break;
                        }
                        pos++;
                    }
                }else if(argc > 2 && (count -1) < 1){
                    printf("\nInvalid flag");
                    exit(1);
                }
                count++;
            }
        }

        if(argc > 1 && argv[argc - 1][0] == '-'){
            dirPtr = opendir(".");
        }else{
            dirPtr = opendir(argv[argc - 1]);
        }
    }
    while((entryPtr = readdir(dirPtr))){
        if(argc > 1 && argv[argc - 1][0] != '-'){
            sprintf(buf, "%s/%s", argv[argc - 1], entryPtr->d_name);
        }else{
            sprintf(buf, "%s/%s", ".", entryPtr->d_name);
        }
        if (stat(buf, &statBuf) < 0) {
            perror ("huh?  there is ");
            exit(1);
        }
        printf("%s  ", entryPtr->d_name);
        
        if(flags[I]){
            printf("%d  ", (int)statBuf.st_ino);
        }
        if(flags[S]){
            printf("%d\t", (int)(statBuf.st_blocks / 2));
        }
        printf("\n");
    }
    closedir(dirPtr);
    return 0;
}
