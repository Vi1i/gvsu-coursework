#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if (argc == 3){
        link(argv[1], argv[2]);
    }else if(argc == 4){
        symlink(argv[2], argv[3]);
    }else{
        printf("Invalid command");
        exit(1);
    }
    return 0;
}
