#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"
#include <time.h>
#include <math.h>
#include <sys/types.h>


#define EXPECTED_TOTAL 45
int results = 1;

typedef struct { 
int row;
int column;
int (*entries) [9];
}parameters;


//Function for columns
void *vertical(void* param){            // entries[] passed in

    parameters *data = (parameters *) param;
    int total;

    for(int i = 0; i < 9; i++){        // Checks every column for repeating values
        total = 0;
        for(int j = 0; j < 9; j++){    
            total += data -> entries[j][i];     // makes sure the values are in matching columns
        }
        if (total != EXPECTED_TOTAL){
            results = 0;
        } 
    }
    pthread_exit(NULL);
}

//function for rows
void *horizontal(void* param){
    parameters *data = (parameters*) param;
    int total;

    for(int i = 0; i < 9; i++){
        total = 0;
        for(int j = 0; j < 9; j++){
            total += data -> entries[i][j];
        }  
        if (total != EXPECTED_TOTAL){
            results = 0;
        }
    }
    pthread_exit(NULL);
}


//function for 3x3 for 3 threads
void *nine_by_nine(void* param) {
    parameters *data = (parameters *) param;
    int row = data -> row;
    int col = data -> column;
    int total;
  
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if ((i % 3 == 0) && (j % 3== 0)) {
                total = 0;
                for (int k = i; k < i + 3; k++) {
                    for (int p = j; p < j + 3; p++) {
                        total += data -> entries[k][p];
                    }
                }
            }
            if (total != EXPECTED_TOTAL) {
                results = 0;
            } 
        }
    }
    pthread_exit(NULL);
}

//function for 3x3 for 11 threads
void *three_by_three(void *param) {

    parameters *data = (parameters*) param;
    int row = data -> row;
    int col = data -> column;
    int total = 0;

    for (int i = col; i < col + 3; i++) {
        for (int j = row; j < row + 3; j++){
            total += data -> entries[i][j];
        }
    }
    if (total != EXPECTED_TOTAL) {
        results = 0;
    } 
    
    pthread_exit(NULL); // <<THIS IS THE PROBLEM CHILD
}

int main(int argc, char* argv[]) {
    
    FILE *input_file;
    int option = atol(argv[1]); /* Takes in user input for options */
    int total_threads;
    int entries[9][9];

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
    clock_t t;
    t = clock();
    

    if (option == 1)
    {
        total_threads = 11;
    } 
    else if(option == 2) 
    {
        total_threads = 3;
    } 
    else if(option == 3)
    {
        printf("ERROR: OPTION NOT READY!!\n");
    }
    else
    {
        printf("ERROR: OPTION NOT ACCEPTED\n");
    }

    printf("option selected");
    pthread_t thread[total_threads];    // Determined by option

    if (total_threads == 11){
        int curr_thread = 0;
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                if((i % 3 == 0) && (j % 3 == 0)) {
                    parameters *specialParams = (parameters *) malloc(sizeof(parameters));
                    specialParams -> row = i;
                    specialParams -> column = j;
                    specialParams -> entries = entries;

                    pthread_create(&thread[curr_thread], NULL, three_by_three, specialParams);
                    curr_thread++;
                }
                if((i == 0) && (j == 0)) {
                    
                    parameters *params = (parameters *) malloc(sizeof(parameters));
                    params -> row = i;
                    params -> column = j;
                    params -> entries = entries;

                    pthread_create(&thread[curr_thread],NULL,vertical, params);
                    curr_thread++;
                    pthread_create(&thread[curr_thread],NULL,horizontal, params);
                    curr_thread++;
                }                
            }
        }
    } 
    
    else if (total_threads == 3) {

        parameters *params = (parameters *) malloc(sizeof(parameters));
        params -> row = 0;
        params -> column = 0;
        params -> entries = entries;

        pthread_create(&thread[0], NULL, nine_by_nine, params);
        printf("horizontal\n");
        pthread_create(&thread[1], NULL, horizontal, params);
        printf("vertical\n");
        pthread_create(&thread[2],NULL, vertical, params);
        }

    
    for(int i = 0; i < total_threads; i++) {
        pthread_join(thread[i], NULL);
    }
    

    /* gets final clock timer */
    t = clock() - t;
    total_time = ((double)t/CLOCKS_PER_SEC);

    if(results == 0) {
        printf("SOLUTION NO (%f seconds)\n", total_time);
    } else {
        printf("SOLUTION: YES (%F seconds)\n", total_time);
    }

return 0;
}