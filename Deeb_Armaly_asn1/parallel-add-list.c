/* 
Deeb Armaly
ECE 4370
Project 1

This file adds the vector elements using parallel computing
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "MyMPI.h"


int main(int argc, char *argv[]) {

    char file_name[30];
    
    if(argc == 3) {
        strcpy(file_name, argv[2]);
    }
    else {
        strcpy(file_name,"default-list-file.dat");
    }
    

    //MPI_Status status;
    MPI_Init(&argc, &argv);

    int size, rank;
    int *v; //sub vector for individual process
    int n; //vector length
    int i;
    long int global_s;
    long int local_sum = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size%2 != 0) {
        printf("Number of processors is not a power of 2\n");
        exit(0);
    }

    read_block_vector(file_name, &v, MPI_INT, &n, MPI_COMM_WORLD);

    for(i = 0; i < BLOCK_SIZE(rank, size, n); i++) {
        local_sum += (long int) *(v+i);
    }
    //printf("Local Sum for process %d is %ld\n", rank, local_sum);
    MPI_Allreduce(&local_sum, &global_s, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if(rank == 1){
        printf("Parallel computed global sum is %ld\n", global_s);
    }
    MPI_Finalize();
}
