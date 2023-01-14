/*
    Deeb Armaly
    ECE 4730
    Assignment 2
    Fall 2022

    This program multiplies two vectors serially
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>



int main(int argc, char *argv[]) {
    double a, b;
    int rowsA, colsA, rowsB, colsB;
    FILE *fpt1, *fpt2, *output;
    double *A, *B, *C;

    if(argc != 4) {
        printf("Error, usage: ./mm-serial input_file1 input_file2 output_file\n");
        exit(0);
    }

    fpt1 = fopen(argv[1], "rb");
    if(fpt1 == NULL) {
        printf("Error opening file %s for reading\n", argv[1]);
        exit(0);
    }

    fpt2 = fopen(argv[2], "rb");
    if(fpt2 == NULL) {
        printf("Error opening file %s for reading\n", argv[2]);
        fclose(fpt1);
        exit(0);
    }

    fscanf(fpt1,"%d %d ", &rowsA, &colsA);
    fscanf(fpt2,"%d %d ", &rowsB, &colsB);

    if(colsA != rowsB) {
        printf("Error: Columns of matrix A should match the number of rows in matrix B\n");
        fclose(fpt1);
        fclose(fpt2);
        exit(0);
    }

    output = fopen(argv[3], "wb");
    if(output == NULL) {
        printf("Could not open file %s for writing\n", argv[3]);
        fclose(fpt1);
        fclose(fpt2);
        exit(0);
    }

    A = (double *)calloc(rowsA*colsA, sizeof(double));
    B = (double *)calloc(rowsB*colsB, sizeof(double));
    C = (double *)calloc(colsA*rowsB, sizeof(double));

    int i, j, n;
    for(i = 0; i < rowsA; i++) {
        for(j = 0; j < colsA; j++) {
            fscanf(fpt1, "%lf", &a);
            fscanf(fpt2, "%lf", &b);
            A[i*colsA+j] = a;
            B[i*colsB+j] = b;
        }
    }

    clock_t start, end;
    start = clock();

    for(i = 0; i < rowsA; i++) {
        for(j = 0; j < colsA; j++) {
            for(n = 0; n < rowsA; n++) {
                C[i*colsA+j] += (A[i*colsA+n] * B[n*colsA+j]);
            }
        }
    }

    end = clock();
    printf("Serial Matrix multiplication time is %lf\n", fabs(start-end)/CLOCKS_PER_SEC);

    fprintf(output,"%d %d\n", rowsA, colsA);
    for(i = 0; i < rowsA; i++) {
        for(j = 0; j < colsA; j++) {
            fprintf(output,"%f ",C[i*colsA+j]);
        }
        fprintf(output,"\n");
    }

    free(A);
    free(B);
    free(C);
    fclose(fpt1);
    fclose(fpt2);
    fclose(output);
}