/* 
Deeb Armaly
ECE 4370
Project 1

This file creates the list to be summed using parallel programming
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fpt;
    int n = 100;
    int i, num;
    
    if(argc == 5) {
        fpt = fopen(argv[4], "wb");
        n = atoi(argv[2]);
    }
    else if(argc == 3) {
        if(strcmp(argv[1],"-n") == 0) {
            n = atoi(argv[2]);
            fpt = fopen("default-list-file.dat","wb");
        }
        else if(strcmp(argv[1],"-o") == 0) {
            fpt = fopen(argv[2],"wb");
        }
    }
    else {
        fpt = fopen("default-list-file.dat","wb");
    }
    
    fwrite(&n,sizeof(int),1,fpt);
    for(i = 0; i < n; i++) {
        num = rand()%100 + 1;  // generate randome number between 1-100 and insert into data file
        fwrite(&num,sizeof(int), 1,fpt);
    }

    fclose(fpt);

}