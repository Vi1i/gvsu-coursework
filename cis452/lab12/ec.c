#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char ** argv){
    int file = open("/dev/random", O_RDONLY);
    unsigned int seed;
    long int randNum;
    read(file, &seed, sizeof(seed));
    srandom(seed);

    randNum = random();

    printf("%ld\n", randNum);
    return 0;
}
