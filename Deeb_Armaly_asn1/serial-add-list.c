/* 
Deeb Armaly
ECE 4370
Project 1

This file serially adds up all the elements in a vector
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fpt;
    long int total_sum = 0;
    int m;
    int num;
    
    if(argc == 3) {
        fpt = fopen(argv[2],"rb");
    }
    else {
        fpt = fopen("default-list-file.dat", "rb");
    }
    if(fpt == NULL) {
        printf("Unable to open data file for reading\n");
        exit(0);
    }

    m = fread(&num,sizeof(int),1,fpt); //get first number which indicates total number of elements
    printf("Number of elements in array is %d\n", num);
    while(1) {
        m = fread(&num,sizeof(int),1,fpt);
        if(m != 1) {
            break;
        }
        total_sum += num;
    }
    fclose(fpt);
    printf("Global Sum determined serially is: %ld\n", total_sum);
}