/*
    Deeb Armaly
    ECE 4730
    Assignment 2
    Fall 2022

    This program prints a square matrix stored in the input_file argument
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char line[1000000];
    FILE *fpt;

    if(argc != 2) {
        printf("Error, usage: ./print-matrix input_file\n");
        exit(0);
    }

    fpt = fopen(argv[1], "rb");
    if(fpt == NULL) {
        printf("Error opening file %s for reading\n",argv[1]);
        exit(0);
    }

    // first line stores row and col counts
    while(fgets(line, sizeof(line), fpt) != NULL) {
        printf("%s", line);
    }

    fclose(fpt);
}