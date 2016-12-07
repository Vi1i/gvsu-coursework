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

int mpi_place_queen(size_t board_size, int * board, int col, int rank) {
    int solutions = 0;
    if(col == board_size) {
        return 1;
    }else{
        board[0] = rank;
        for(int j = 0; j < board_size; j++) {
            board[col] = j;
            if(valid_move(board, col)) {
                solutions += mpi_place_queen(board_size, board, col + 1, rank);
            }
        }
    }

    return solutions; 
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
    int num_nodes;

    int solutions = 0;
    int * board;
    board = (int *) malloc(board_size * sizeof(int));

    MPI_Init( & argc, & argv);
    MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, & num_nodes);

    if(my_rank == MASTER) {
        for(int z = 0; z < (num_nodes - 1); z++) {
            int result;
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            solutions += result;
        }
        printf("Parallel: %d\n", solutions);
    }else{
        solutions = mpi_place_queen(board_size, board, 1, my_rank - 1);
        printf("Worker-%d: %d\n", my_rank, solutions);
        MPI_Send(&solutions, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return solutions;
}

int seq_k_queens(size_t board_size) {
    int solutions;
    int *board;
    board = (int *) malloc(board_size * sizeof(int));

    solutions = place_queen(board_size, board, 0);
    return solutions;
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
    int solutions = 0;

    board_size = strtol(argv[1], &ptr, 10);
    run_type = strtol(argv[2], &ptr, 10);

    if(run_type) {
        solutions = mpi_k_queens(argc, argv, board_size);
    }else{
        solutions = seq_k_queens(board_size);
        printf("Sequential: %d\n", solutions);
    }


    return 0;
}
