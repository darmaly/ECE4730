/*
Deeb Armaly
ECE 4370
Project 3

This file creates a vector of doubles to be used in the Jacobi Iterative Solver
Usage: ./make-vector [size] output_file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{

    FILE *fpt;
    int size;
    int i;
    double num;
    char output_file[25];

    if (argc != 3)
    {
        printf("Error: Usage - ./make-vector [size] output_file\n");
        exit(0);
    }

    size = atoi(argv[1]);
    strcpy(output_file, argv[2]);

    if ((fpt = fopen(output_file, "wb")) == NULL)
    {
        printf("Error: Unable to open %s for writing\n", argv[2]);
        exit(0);
    }

    srand(time(0));

    fprintf(fpt, "%d ", size);
    for (i = 0; i < size; i++)
    {
        num = ((double)rand() * (99.0)) / (double)RAND_MAX + 1.0; // Generates random number between 1-100
        fprintf(fpt, "%0.3lf ", num);
    }

    fclose(fpt);
}