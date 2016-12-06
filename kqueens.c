#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool valid_move(int col, int board[]) {
    for(int i = 0; i < col; i++) {
        if(abs(board[i] - board[col]) == (col - i)) {
            return false;
        }
        if(board[i] == board[col]) {
            return false;
        }
    }
    return true; 
}

int place_queen(size_t size, int col, int board[]) {
    int solutions = 0;
    if(col == size) {
        return 1;
    }else{
        for(int j = 0; j < size; j++) {
            board[col] = j;
            if(valid_move(col, board)) {
                solutions += place_queen(size, col + 1, board);
            }
        }
    }

    return solutions; 
}

int main(int argc, char * argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [BOARDSIZE]\n", argv[0]);
        return 1;
    }

    char *ptr;
    size_t size;

    size = strtol(argv[1], &ptr, 10);

    int board[size];
    int solutions;
    solutions = place_queen(size, 0, board);
    printf("%d\n", solutions);
    return 0;
}
