/*
    Deeb Armaly
    ECE 4730
    Assignment 3
    Fall 2022

    This program performs sequential matrix vector multiplication
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Error: Usage- ./MatrixVector-Serial MatrixA VectorX VectorBprime\n");
        exit(0);
    }

    FILE *fptA, *fptX, *fptB;
    int m, n, n2;

    if ((fptA = fopen(argv[1], "rb")) == NULL)
    {
        printf("Error: Unable to open file %s for reading\n", argv[1]);
        exit(0);
    }

    if ((fptX = fopen(argv[2], "rb")) == NULL)
    {
        printf("Error: Unable to open file %s for reading\n", argv[2]);
        exit(0);
    }

    fscanf(fptA, "%d %d ", &n, &n2);
    fscanf(fptX, "%d ", &m);

    if (m != n)
    {
        printf("Error: MatrixA must have same row/col demension as VectorX\n");
        exit(0);
    }

    double *matrixA, *vectorX, *result;
    matrixA = (double *)calloc(n * n, sizeof(double));
    vectorX = (double *)calloc(m, sizeof(double));
    result = (double *)calloc(m, sizeof(double));

    int i, j;
    double temp;

    for (i = 0; i < n * n; i++)
    {
        fscanf(fptA, "%lf", &temp);
        matrixA[i] = temp;
    }

    for (i = 0; i < m; i++)
    {
        fscanf(fptX, "%lf", &temp);
        vectorX[i] = temp;
    }

    clock_t start, end;
    start = clock();

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            result[i] += matrixA[i * n + j] * vectorX[j];
        }
    }

    end = clock();

    if ((fptB = fopen(argv[3], "wb")) == NULL)
    {
        printf("Error: Unable to open file %s for reading\n", argv[3]);
        exit(0);
    }

    fprintf(fptB, "%d ", n);
    for (i = 0; i < n; i++)
    {
        fprintf(fptB, "%0.3lf ", result[i]);
    }

    fclose(fptA);
    fclose(fptX);
    fclose(fptB);

    free(matrixA);
    free(vectorX);
    free(result);
}