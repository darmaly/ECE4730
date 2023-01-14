/*
    Deeb Armaly
    ECE 4730
    Assignment 2
    Fall 2022

    This program creates a square matrix to be used to implement Cannon's Algorithm
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int upper_bound;
    int lower_bound;
    int rows, cols;
    char output_file[25];
    double v;
    FILE *fpt;

    if (argc != 11)
    {
        printf("Usage example: ./make-matrix -r 100 -c 100 -l 5 -u 50 -o output_file\n");
        printf("Where\n-r <rows>  number of rows in the matrix\n-c <cols> number of columns in the matrix\n -l <low> lower bound on the values in the matrix\n-u <up> upper bound of the values in the matrix\n-o <fname> output file name\n");
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        rows = atoi(argv[2]);
    }
    else
    {
        printf("Error: first argument must be -r\n");
        exit(0);
    }

    if (strcmp(argv[3], "-c") == 0)
    {
        cols = atoi(argv[4]);
    }
    else
    {
        printf("Error: second argument must be -c\n");
        exit(0);
    }

    if (rows != cols)
    {
        printf("Error: matrix must be a square (-r and -c arguments must be equal)\n");
    }

    if (strcmp(argv[5], "-l") == 0)
    {
        lower_bound = atoi(argv[6]);
    }
    else
    {
        printf("Error: third argument must be -l\n");
        exit(0);
    }

    if (strcmp(argv[7], "-u") == 0)
    {
        upper_bound = atoi(argv[8]);
    }
    else
    {
        printf("Error: fourth argument must be -u\n");
        exit(0);
    }

    if (strcmp(argv[9], "-o") == 0)
    {
        strcpy(output_file, argv[10]);
    }
    else
    {
        printf("Error: fifth argument must be -o\n");
        exit(0);
    }

    fpt = fopen(output_file, "wb");

    srand(time(0));
    int i, j;

    fprintf(fpt, "%d %d\n", rows, cols);
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            v = ((double)rand() * (upper_bound - lower_bound)) / (double)RAND_MAX + lower_bound;
            if (i == j)
            {
                v *= 100;
            }
            fprintf(fpt, "%0.3f ", v);
        }
        fprintf(fpt, "\n");
    }

    fclose(fpt);
}