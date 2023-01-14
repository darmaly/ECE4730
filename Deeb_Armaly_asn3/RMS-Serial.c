/*
    Deeb Armaly
    ECE 4730
    Assignment 3
    Fall 2022

    This program computes the RMS of two vectors and prints the result to stdout
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SQR(x) ((x) * (x))

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Error: Usage- ./RMS-Serial VectorB VectorBprime\n");
        exit(0);
    }

    FILE *fptB, *fptPrime;
    int sizeB, sizePrime;

    if ((fptB = fopen(argv[1], "rb")) == NULL)
    {
        printf("Error: Unable to open file %s for reading\n", argv[1]);
        exit(0);
    }

    if ((fptPrime = fopen(argv[2], "rb")) == NULL)
    {
        printf("Error: Unable to open file %s for reading\n", argv[1]);
        exit(0);
    }

    fscanf(fptB, "%d ", &sizeB);
    fscanf(fptPrime, "%d ", &sizePrime);

    if (sizeB != sizePrime)
    {
        printf("Error: vectors must be of same size\n");
        exit(0);
    }

    int i;
    int n = sizeB;
    double temp1, temp2;
    double *vecB, *vecPrime;

    vecB = (double *)calloc(n, sizeof(double));
    vecPrime = (double *)calloc(n, sizeof(double));

    for (i = 0; i < n; i++)
    {
        fscanf(fptB, "%lf ", &temp1);
        vecB[i] = temp1;
        fscanf(fptPrime, "%lf ", &temp2);
        vecPrime[i] = temp2;
    }

    double *subArray;

    subArray = (double *)calloc(n, sizeof(double));

    for (i = 0; i < n; i++)
    {
        subArray[i] = vecPrime[i] - vecB[i];
    }

    double square = 0.0;
    for (i = 0; i < n; i++)
    {
        square += SQR(subArray[i]);
    }

    double mean = 0.0;
    double root = 0.0;

    mean = square / ((double)n);
    root = sqrt(mean);

    printf("RMS of %s and %s is: %0.3lf\n", argv[1], argv[2], root);

    fclose(fptB);
    fclose(fptPrime);

    free(vecB);
    free(vecPrime);
    free(subArray);
}