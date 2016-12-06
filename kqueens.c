#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

#define MASTER 0

bool valid_move(int * board, int col) {
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

int place_queen(size_t board_size, int * board, int col) {
    int solutions = 0;
    if(col == board_size) {
        return 1;
    }else{
        for(int j = 0; j < board_size; j++) {
            board[col] = j;
            if(valid_move(board, col)) {
                solutions += place_queen(board_size, board, col + 1);
            }
        }
    }

    return solutions; 
}

int mpi_k_queens(int argc, char * argv[], size_t board_size) {
    int my_rank;
    int source;
    int num_nodes;

    int solutions;
    int * board;
    board = (int *) malloc(board_size * sizeof(int));

    MPI_Init( & argc, & argv);
    MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, & num_nodes);

    if(my_rank != MASTER) {
    printf("Master has %d workers.\n", num_nodes - 1);
    }else{
    printf("Worker-%d\n", my_rank);
    }

    MPI_Finalize();
    return 0;
}

int seq_k_queens(size_t board_size) {
    int solutions;
    int *board;
    board = (int *) malloc(board_size * sizeof(int));

    solutions = place_queen(board_size, board, 0);
}

int main(int argc, char * argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [BOARDSIZE] [0/1]\n", argv[0]);
        fprintf(stderr, "\t[BOARDSIZE]\n");
        fprintf(stderr, "\t\tThis is the size of the board\n");
        fprintf(stderr, "\t[0/1]\n");
        fprintf(stderr, "\t\t0: For sequential\n");
        fprintf(stderr, "\t\t1: For MPI\n");
        return 1;
    }

    char *ptr;
    size_t board_size;
    int run_type;
    int solutions;

    board_size = strtol(argv[1], &ptr, 10);
    run_type = strtol(argv[2], &ptr, 10);

    if(run_type) {
        solutions = mpi_k_queens(argc, argv, board_size);
    }else{
        solutions = seq_k_queens(board_size);
        printf("Sequetial: %d\n", solutions);
    }

    return 0;
}
