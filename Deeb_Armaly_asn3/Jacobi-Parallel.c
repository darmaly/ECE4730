/*
    Deeb Armaly
    ECE 4730
    Assignment 3
    Fall 2022

    This program uses the Jacobi Iterative solver method to perform matrix vector calculations in parallel
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        printf("Error: Usage- ./Jacobi-Parallel -d 100 MatrixA VectorX VectorB\n");
        exit(0);
    }

    if (strcmp(argv[1], "-d") != 0)
    {
        printf("Error: flag following executable must be -d\n");
        exit(0);
    }

    FILE *fpt;
    int iterations = atoi(argv[2]);
    MPI_Status status;
    int n, nLocal, rows, cols, i, j, x, rowGlobal, index;
    int size, rank;
    double *vecPrev, *vecNext, *locX, *recA, *recX;
    double temp;
    double *matrixA, *vectorX, *vectorB;
    double start, end, runTime, global;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        if ((fpt = fopen(argv[3], "rb")) == NULL)
        {
            printf("Error: Unable to open file %s for reading\n", argv[3]);
            exit(0);
        }

        fscanf(fpt, "%d %d ", &rows, &cols);
        if (rows != cols)
        {
            printf("Error: input matrix must be a square\n");
            exit(0);
        }

        matrixA = (double *)calloc(rows * cols, sizeof(double));
        for (i = 0; i < rows * cols; i++)
        {
            fscanf(fpt, "%lf ", &temp);
            matrixA[i] = temp;
        }
        fclose(fpt);

        if ((fpt = fopen(argv[5], "rb")) == NULL)
        {
            printf("Error: Unable to open file %s for reading\n", argv[4]);
            exit(0);
        }

        fscanf(fpt, "%d ", &n);
        if (n != rows)
        {
            printf("Error: input vector must have same size as rows of input matrix\n");
            exit(0);
        }

        vectorX = (double *)calloc(n, sizeof(double));
        for (i = 0; i < n; i++)
        {
            fscanf(fpt, "%lf ", &temp);
            vectorX[i] = temp;
        }
        fclose(fpt);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0)
    {
        MPI_Finalize();
        if (rank == 0)
        {
            printf("Vector size must be divisible by number of processors\n");
        }
        exit(0);
    }

    nLocal = n / size;
    recA = (double *)calloc(n * nLocal, sizeof(double));
    recX = (double *)calloc(nLocal, sizeof(double));

    MPI_Scatter(matrixA, nLocal * n, MPI_DOUBLE, recA, nLocal * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vectorX, nLocal, MPI_DOUBLE, recX, nLocal, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vecPrev = (double *)calloc(n, sizeof(double));
    vecNext = (double *)calloc(n, sizeof(double));
    locX = (double *)calloc(nLocal, sizeof(double));

    for (i = 0; i < nLocal; i++)
    {
        locX[i] = recX[i];
    }

    MPI_Allgather(locX, nLocal, MPI_DOUBLE, vecNext, nLocal, MPI_DOUBLE, MPI_COMM_WORLD);

    start = MPI_Wtime();

    for (x = 0; x < iterations; x++)
    {

        for (i = 0; i < n; i++)
        {
            vecPrev[i] = vecNext[i];
        }

        for (i = 0; i < nLocal; i++)
        {
            index = i * n;
            rowGlobal = BLOCK_LOW(rank, size, n) + i;
            locX[i] = recX[i];

            for (j = 0; j < rowGlobal; j++)
            {
                locX[i] -= vecPrev[j] * recA[index + j];
            }
            for (j = rowGlobal + 1; j < n; j++)
            {
                locX[i] -= vecPrev[j] * recA[index + j];
            }
            locX[i] = locX[i] / recA[i * n + rowGlobal];
        }

        MPI_Allgather(locX, nLocal, MPI_DOUBLE, vecNext, nLocal, MPI_DOUBLE, MPI_COMM_WORLD);
    }

    end = MPI_Wtime();

    runTime = end - start;
    MPI_Reduce(&runTime,&global,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if (rank == 0)
    {
        if ((fpt = fopen(argv[4], "wb")) == NULL)
        {
            printf("Error: Unable to open file %s for writing\n", argv[4]);
            exit(0);
        }

        vectorB = (double *)calloc(n, sizeof(double));
        for (i = 0; i < n; i++)
        {
            vectorB[i] = vecNext[i];
        }

        fprintf(fpt, "%d ", n);
        for (i = 0; i < n; i++)
        {
            fprintf(fpt, "%0.3lf ", vectorB[i]);
        }
        fclose(fpt);
        printf("Time to do parallel matrix multiplication: %0.4lf\n",fabs(global));
    }

    MPI_Finalize();
}