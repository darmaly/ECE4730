/*
    Deeb Armaly
    ECE 4730
    Assignment 2
    Fall 2022

    This program multiplies two vectors using parallel computing with MPI
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>


#define MIN(a,b)           ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) \
                     (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(j,p,n) (((p)*((j)+1)-1)/(n))
#define PTR_SIZE           (sizeof(void*))
#define CEILING(i,j)       (((i)+(j)-1)/(j))


int main(int argc, char *argv[]) {

    if(argc != 4) {
        printf("Error, usage: mpirun -np __ ./mm-parallel input_file1 input_file2 output_file\n");
        exit(0);
    }

    MPI_Comm cartComm;
    MPI_Status status;
    FILE *fptA, *fptB, *fptC;
    int size, rank;
    double *A, *B, *C;
    double temp1, temp2;
    int rowsA, colsA, rowsB, colsB;
    int i, j, k;
    double *a, *b, *c, *buffer, *tmp;
    int n;
    int up, down, left, right;
    int dims[2];
    int coords[2];
    int periods[2];
    int cartRank;
    int nlocal;
    int shift;
    double start, end, runTime, global;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    dims[0] = dims[1] = 0;
    periods[0] = periods[1] = 1;

        /* Create dimensions of matrices to be sent to processes */
    MPI_Dims_create(size,2,dims);

        /* Create cartesian topology */
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cartComm);

        /* get rank and coords for new topology */
    MPI_Comm_rank(cartComm, &cartRank);
    MPI_Cart_coords(cartComm, cartRank, 2, coords);

    MPI_Barrier(MPI_COMM_WORLD);

    if(dims[0] != dims[1]) {
        if(cartRank == 0) {
            printf("Number of processors must be square e.g 4, 9, 16, 25...etc");
            MPI_Finalize();
            return 0;
        }
    }

    if(cartRank == 0) {
        printf("Number of processors: %d\n", size);
        fptA = fopen(argv[1],"rb");
        fptB = fopen(argv[2],"rb");
        if(fptA == NULL || fptB == NULL) {
            printf("Unable to open file for reading\n");
            MPI_Finalize();
            return 0;
        }

        fscanf(fptA,"%d %d ", &rowsA, &colsA);
        fscanf(fptB,"%d %d ", &rowsB, &colsB);

        if(rowsA != colsA || rowsB != colsB) {
            printf("Error: both matrices must be square matrices\n");
            MPI_Finalize();
            return 0;
        }
        if(colsA != rowsB){
            printf("Error: rows of first matrix must match columns of secon matrix\n");
            MPI_Finalize();
            return 0;
        }
        n = rowsA;
            /* Send count information to each process */
        for(i = 0; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 1, cartComm);
        }

        printf("Local dimensions %dx%d\n", n/dims[0], n/dims[1]);

        A = (double *)calloc(n*n, sizeof(double));
        B = (double *)calloc(n*n, sizeof(double));
        C = (double *) calloc(n*n, sizeof(double));

        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++) {
                fscanf(fptA,"%lf",&temp1);
                fscanf(fptB,"%lf",&temp2);
                A[i*n+j] = temp1;
                B[j*n+i] = temp2;
            }
        }

        fclose(fptA);
        fclose(fptB);
        
    }
        /* Recieve count information */
    MPI_Recv(&n, 1, MPI_INT, 0, 1, cartComm, MPI_STATUS_IGNORE);

        /* local dimension of matrix for each process */
    nlocal = n / dims[0];

    MPI_Barrier(cartComm);

        /* local matrices storage location */
    a = (double *)calloc(nlocal*nlocal, sizeof(double));
    b = (double *)calloc(nlocal*nlocal, sizeof(double));
    c = (double *)calloc(nlocal*nlocal, sizeof(double));
    buffer = (double *)calloc(nlocal*nlocal, sizeof(double));

        /* Distributes matrices A and B equally amongst processes */
    MPI_Scatter(A,nlocal*nlocal,MPI_DOUBLE,a,nlocal*nlocal,MPI_DOUBLE,0,cartComm);
    MPI_Scatter(B,nlocal*nlocal,MPI_DOUBLE,b,nlocal*nlocal,MPI_DOUBLE,0,cartComm);

    MPI_Barrier(cartComm);

        /* get ranks of up down left and right shifts */
    MPI_Cart_shift(cartComm, 0, 1, &left, &right);    
    MPI_Cart_shift(cartComm, 1, 1, &down, &up);

    start = MPI_Wtime();
    
        /* Main computation loop */
    for (shift = 0; shift < dims[0]; shift++) {
        for(i = 0; i < nlocal; i++) {
            for(j = 0; j < nlocal; j++) {
                for(k = 0; k < nlocal; k++) {
                    c[i*nlocal+j] += a[i*nlocal+k]*b[i*nlocal+k];
                }
            }
        }
       
       MPI_Sendrecv(a,nlocal*nlocal,MPI_DOUBLE,left,1,buffer,nlocal*nlocal,MPI_DOUBLE,right,1,cartComm,&status);
       tmp = buffer; 
       buffer = a; 
       a = tmp;

       MPI_Sendrecv(b,nlocal*nlocal,MPI_DOUBLE,up,2,buffer,nlocal*nlocal,MPI_DOUBLE,down,2,cartComm,&status);
       tmp = buffer;
       buffer = b;
       b = tmp;
       
    }

        /* wait for all processes to reach this state */
    MPI_Barrier(cartComm);

    MPI_Gather(c, nlocal*nlocal, MPI_DOUBLE, C, nlocal*nlocal, MPI_DOUBLE, 0, cartComm);
    end = MPI_Wtime();

    runTime = end - start;
    MPI_Reduce(&runTime,&global,1,MPI_DOUBLE,MPI_MAX,0,cartComm);


    if (cartRank == 0) {
        
        fptC = fopen(argv[3], "wb");
        if(fptC == NULL) {
            printf("Error: unable to open %s for writing\n", argv[3]);
            MPI_Finalize();
            return 0;
        }

        n = 0;
        for(i = 0; i < rowsA; i++) {
            for(j = 0; j < colsA; j++) {
                fprintf(fptC,"%lf ",C[n]);
                n++;
            }
            fprintf(fptC,"\n");
        }
        fclose(fptC);
        
        printf("Time to do parallel matrix multiplication: %0.4lf\n",fabs(global));
    }

    MPI_Barrier(cartComm);

    MPI_Finalize();

}