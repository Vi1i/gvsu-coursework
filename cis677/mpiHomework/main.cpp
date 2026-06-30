#include <iostream>
#include <string>
#include <cstdlib>
#include <mpi.h>

#define MASTER  0
#define WORKER  1
#define RUNS    100
#define TAG     1

int main(int argc, char* argv[]) {
    long int num_steps; 
    int my_rank;
    int num_nodes;
    int source;
    int increment;
    size_t size;
    double t1;
    double t2;
    double total_time;

    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << "[SIZE]\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    size = std::stoi(argv[1]), nullptr, 10;
    if(size < 10 && size % 10 != 0) {
        std::cerr << "Size should be some type of multiple of 10 and positive" << std::endl;
        exit(EXIT_FAILURE);
    }

    increment = size / 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double * send_data;
    double * recv_data;
    send_data = (double *) calloc(size, sizeof(double));

    //Workers
    if(my_rank != MASTER) {
        for(int z = 0; z < 10; z++) {
            size_t temp_size = z * increment;
            recv_data = (double *) malloc(temp_size * sizeof(double));

            for(int y = 0; y < RUNS; y++) {
                MPI_Send(&send_data, temp_size, MPI_DOUBLE, MASTER, TAG, MPI_COMM_WORLD);
                MPI_Recv(&recv_data, temp_size, MPI_DOUBLE, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }else{  //Master
        double bandwidth;
        double avg_time;
        double avg_bandwidth;

        for(int z = 0; z < 10; z++) {
            size_t temp_size = z * increment;
            int nbytes = sizeof(double) * z;
            recv_data = (double *) malloc(temp_size * sizeof(double));

            for(int y = 0; y < RUNS; y++) {
                t1 = MPI_Wtime();
                MPI_Recv(&recv_data, temp_size, MPI_DOUBLE, WORKER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&send_data, temp_size, MPI_DOUBLE, WORKER, TAG, MPI_COMM_WORLD);
                t2 = MPI_Wtime();

                total_time = t2 - t1;
                bandwidth = ((double)(nbytes * 2)) / total_time;

                avg_time += total_time;
                avg_bandwidth += bandwidth;
            }

            avg_time = avg_time / RUNS;
            avg_bandwidth = avg_bandwidth / RUNS;
            printf("%.9f:%.9f:%d\n", avg_time, avg_bandwidth, z);
        }
    }

    MPI_Finalize();
    return 0;
}
