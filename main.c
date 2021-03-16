#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"

//extern FILE *input_file;
const int MAX = 82;

int main(int argc, char *argv[]) {
    FILE *input_file;

    int val;      // Variable to contain value currently scanned
    int entry[MAX];


    int scanTest = 0;

    input_file = fopen(argv[argc-1],"r");
    if(!input_file) {
        printf("Couldnt open file\n");
        return 1;
    }

    while(fscanf(input_file, "%d", &val) == 1) {
        printf("%d\n",val);
        
    } 
    //printf("Opens file?");

return 0;
}
