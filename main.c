#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"

//extern FILE *input_file;
const int MAX = 4;

int main(int argc, char *argv[]) {
    FILE *input_file;

    int val;      /* Variable to contain value currently scanned */
    int i;  /* Iterating for printing out the sequence */


    int entries[81] = {0};   /* stores inputs to be arranged into rows, columns, 
                            and 3x3 sections */
    

    int iterate = 0;

    printf("Last term before scanning: %d\n",entries[iterate]);
    input_file = fopen(argv[argc-1],"r");
    if(!input_file) {
        printf("Couldnt open file\n");
        return 1;
    }

    while(fscanf(input_file, "%d", &val) == 1) {
        
        entries[iterate] = val; /* array will store all values from the input file, then the program will split the inputs up into either rows, columns, or 3x3 sections */
        
        printf("%d ",entries[iterate]);

        if (iterate % 9 ==8){
            printf("\n");
        }
        
        iterate++;
        //iterate = iterate+1;
        //count++;
        /*for (i = 0; i < 9; i++) {
            printf("%d ",val);
        }*/        
    /*printf("Term %d: %d\n", iterate, entries[iterate]);*/
        
    } 
    printf("iterate: %d\n", iterate);
    printf("Last term: %d\n",entries[iterate]);
    /*
    for(int i = 0; i < iterate; i++) {
        printf("%d ",entries[iterate]);

        if (iterate % 9 ==8){
            printf("\n");
        }
    } */
    //printf("Opens file?");

return 0;
}
