#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"
#include <time.h>
#include <math.h>
#define EXPECTED_TOTAL 45

// //extern FILE *input_file;
// int horizontal_results; // Results for rows
// int vertical_results;   // Results for columns
// int block_results = 1;      // Results for 3x3
int results = 1;

typedef struct { 
int row;
int col;
int (*entries) [9];
}parameters;

//int entries[9][9] = {0}; /* stores inputs */    

//Function for columns
void* vertical(void* param) {            // entries[] passed in
    //int passed_array[9][9]; 
    //passed_array[9][9] = atol(nums);
    parameters *data = (parameters*) param;
    int total;
    printf("vertical\n");

    for(int i = 0; i < 9; i++) {        // Checks every column for repeating values
        total = 0;
        for(int j = 0; j < 9; j++) {    
            total += (data -> entries[j][i]);     // makes sure the values are in matching columns
        }
        if (total!=EXPECTED_TOTAL) {
            //vertical_results = 1;
            results = 0;
        } 
        // else {
        //     vertical_results = 0;
        //     //results = 0;
        // }
    }
    pthread_exit(NULL);

}

//function for rows
void* horizontal(void* param) {
    // int passed_array[9][9];
    // passed_array[9][9] = (int*)param;
    parameters *data = (parameters*) param;
    int total;
    printf("horizontal\n");
    for(int i = 0; i < 9; i++) {
        total = 0;
        for(int j = 0; j < 9; j++) {
            total += (data -> entries[i][j]);
        }
        if (total!=EXPECTED_TOTAL) {
            //horizontal_results = 1;
            results = 0;
        }
        // } else {
        //     horizontal_results = 0;
            
        // }
    }
    pthread_exit(NULL);
}

//function for 3x3 for 3 threads
void* nine_by_nine(void* param) {
    //int passed_matrix[9][9];

    parameters *data = (parameters*) param;
    int row = data -> row;
    int col = data -> colums;
    int total;
    printf("nine by nine\n");
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if ((i % 3 == 0) && (j % 3== 0)) {
                total = 0;
                for (int k = i; k < 3+i; k++) {
                    for (int p = j; p < 3+j; p++) {
                        total +=passed_matrix[k][p];
                    }
                }
            }
            if (total != EXPECTED_TOTAL) {
                //block_results = 1;
                results = 0;
            } 
            // else {
            //     block_results = 0;
            // }
        }
    }
    printf("exiting nine by nine\n");
    pthread_exit(NULL);
}

//function for 3x3 for 11 threads
void* three_by_three(void* param) {
    //int *passed_matrix[3][3];
    //passed_matrix[3][3] = (int *) nums;
    parameters *data = (parameters*) param;

    int row = data -> row;
    int col = data -> col;
    int total = 0;

    printf("three by three\n");
    for (int i = col; i < col + 3; i++) {
        for (int j = row; j < row + 3; j++){
            total += data -> entries[i][j];
        }
    }
    printf("threebythree something\n");
    if (total != (int *)EXPECTED_TOTAL) {
        //block_results = 1;
        results = 0;
    } 
    printf("%ls\n",total);
    
    pthread_exit(NULL); // <<THIS IS THE PROBLEM CHILD
}

int main(int argc, char* argv[]) {
    
    FILE *input_file;
    int option = atol(argv[1]); /* Takes in user input for options */
    int results = 1;    // variable for whether it works or not
    int val;      /* Variable to contain value currently scanned */
    int i;  /* Iterating for printing out the sequence */
    int iterate = 0;

    //pid_t id;   /***PID FOR THREADS ***/
    int total_threads;

    printf("Option %d\n", option);

    printf("BOARD STATE IN input.txt: \n");
    
    input_file = fopen(argv[argc-1],"r");
    if(!input_file) {
        printf("Couldnt open file\n");
        return 1;
    }    
    /* Scanning input */
    for (int i = 0; i < 9; i++) {
        for (int j=0; j<9; j++) {
            fscanf(input_file,"%d",&entries[i][j]);
                
        }
    }

    
    /* Printing out the board */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ",entries[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    /* Starts clock */
    double total_time;
    printf("hey\n");
    clock_t t;
    printf("woo\n");
    t = clock();
    
    printf("clock started");

    if (option == 1){
            total_threads = 11;
    } else if(option == 2) {
        total_threads = 3;
    } else {
        printf("ERROR: OPTION NOT ACCEPTED\n");
    }
    printf("option selected");
    pthread_t thread[total_threads];    // Determined by option
    if (option == 1) {
        int curr_thread = 0;
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                if((i % 3 == 0) && (j % 3 == 0)) {
                    pthread_create(&thread[curr_thread], NULL, three_by_three, (void *)entries);
                    curr_thread++;
                }
                if((i==0) && (j==0)) {
                    pthread_create(&thread[curr_thread],NULL,vertical, entries);
                    curr_thread++;
                    pthread_create(&thread[curr_thread],NULL,horizontal, entries);
                    curr_thread++;
                }                
            }
        }
    } else if (total_threads == 3) {
        printf("nine by nine\n");
        pthread_create(&thread[0], NULL, nine_by_nine(entries),entries);
        printf("horizontal\n");
        pthread_create(&thread[1], NULL, horizontal(entries),entries);
        printf("vertical\n");
        pthread_create(&thread[2],NULL, vertical(entries),entries);
        }

    //printf("iterate: %d\n", iterate);
    //printf("Last term: %d\n",entries[iterate-1]);

    /***********Threads for calculating sudoku********************/

    
    for(int i = 0; i < total_threads; i++) {
        printf("o.O");
        pthread_join(thread[i], NULL);
    }
    

    //printf("Stop clock?\n");
    /* gets final clock timer */
    t = clock() - t;
    total_time = ((double)t/CLOCKS_PER_SEC);

    if(results == 0) {
        printf("SOLUTION NO (%f seconds)\n", total_time);
    } else {
        printf("SOLUTION: YES (%F seconds)\n", total_time);
    }
    
    // if(horizontal_results && vertical_results && block_results) {
    //     printf("SOLUTION: YES (%f seconds\n", total_time);
    // } else{
    //     printf("SOLUTION: NO (%f seconds)\n", total_time);
    // }

return 0;
}
