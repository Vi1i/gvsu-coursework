#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <time.h>

double getResTime (void);

int main(void){
    printf("%.100f\n", getResTime());
    return 0;
}

double getResTime (void) {
    struct timespec t;
    if (0 > clock_getres(CLOCK_REALTIME, &t)) {
        return ((double)-1);
    }
    return (double)(t.tv_sec + 1.0e-9 * t.tv_nsec);  
}
