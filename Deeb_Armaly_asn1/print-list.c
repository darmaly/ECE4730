/* 
Deeb Armaly
ECE 4370
Project 1

This file prints the elements of the data file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fpt;
    int num;
    int m;

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

    while(1) {
        m = fscanf(fpt,"%d ", &num);
        if(m != 1) {
            break;
        }
        printf("%d\n",num);
    }
    fclose(fpt);

}