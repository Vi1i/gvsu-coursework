
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <mpi.h>
#include <sys/time.h>
#define size 8
int totSolutions =0;
bool placable( int col, int board[])
{

    for (int i =0; i<col; i++){
        if(abs(board[i]-board[col]) == (col-i))
            return false;
        if(board[i]== board[col])
            return false;
    }
    return true; 

}
int place_queen (int col,int board[])
{
    
   if(col == size){
       totSolutions++;
    }
   else{
       for(int j = 0; j<size; j++){
           board[col]= j;
           if(placable(col,board) == true){
                place_queen(col+1,board);
            }
        }
    }

    return 0; 
}
int main()
{
    //printf("yes");
    int board[size];
    place_queen(0,board);
    printf("%d\n",totSolutions);
    return 0;
}
