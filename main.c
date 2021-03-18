#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"
#include <pthread.h>

//extern FILE *input_file;
const int MAX = 4;


//Function for vertical
void* vertical(void* nums) {
    int cont_values[9] = {0};

    for(int i=0; i < 9; i++) {
        for(int j = 0; i < 9; j++) {
            
        }
    }

}


//function for horizontal

//function for 3x3

int main(int argc, char *argv[]) {
    FILE *input_file;

    int val;      /* Variable to contain value currently scanned */
    int i;  /* Iterating for printing out the sequence */
    int entries[81] = {0};   /* stores inputs to be arranged into rows, columns, 
                            and 3x3 sections */    
    int iterate = 0;

    int option = atol(argv[1]);
    printf("Option %d\n", option);

    printf("BOARD STATE IN input.txt: \n");
    
    input_file = fopen(argv[argc-1],"r");
    if(!input_file) {
        printf("Couldnt open file\n");
        return 1;
    }    

    while(fscanf(input_file, "%d", &val) == 1) {    
        entries[iterate] = val; /* array will store all values from the input file,
                                * then the program will split the inputs up into 
                                * either rows, columns, or 3x3 sections */
        iterate++;
    } 

    //printf(" %d\n", entries[80]);
    
    for (int i = 0; i < iterate; i++) {
        printf("%d ",entries[i]);

         if (i % 9 == 8) {
             printf("\n");
         }
    }
    printf("\n");
    //printf("iterate: %d\n", iterate);
    //printf("Last term: %d\n",entries[iterate-1]);

    /***********Threads for calculating sudoku********************/

return 0;
}
