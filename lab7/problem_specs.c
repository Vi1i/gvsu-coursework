#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
int main(void){
    struct rlimit limit;
    struct sysinfo sinfo;
    //size of file
    getrlimit(RLIMIT_FSIZE, &limit);
    printf("File Size:\n");
    printf("\tsoft: %lld\n", (long long int)limit.rlim_cur);
    printf("\thard: %lld\n", (long long int)limit.rlim_max);
    for(;;){
        limit.rlim_cur += 1;
        limit.rlim_max += 1;
        setrlimit(RLIMIT_FSIZE, &limit);
        printf("\tsoft: %lld\n", (long long int)limit.rlim_cur);
        printf("\thard: %lld\n", (long long int)limit.rlim_max);
    }
    //sysinfo(sinfo);
    return 0;
}
