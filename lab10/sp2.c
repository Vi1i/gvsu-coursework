#include <stdio.h>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>

int main(){ 
   DIR *dirPtr; 
   struct dirent *entryPtr;
   struct stat statbuf;

   dirPtr = opendir (".");


   while ((entryPtr = readdir (dirPtr))){
        stat(entryPtr->d_name, &statbuf);
        printf ("%s\t\t%u\n", entryPtr->d_name, statbuf.st_size);
   }
   closedir (dirPtr); 
   return 0; 
}
