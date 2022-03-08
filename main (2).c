#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

#define ROW_LENGTH 9
#define COL_LENGTH 9
#define ROW_COL_TOTAL 45

/*tracks if any rows, columns, or squares fail to meet criteria*/
int correct = 1;

/*sudoku entries*/
int entries[9][9];

/*for tracking how many times to call runner functions - dependent on # of threads*/
int times_to_run;
int times_to_run3x3;

/*structure for passing data to threads*/
typedef struct{
	int row;
	int col;
}parameters;

void* vertical(void* param){
	
	/*create parameter pointer to void data pointer*/
	parameters *p = (parameters*) param;
	
	/*instantiate row and column start values*/
	int row = p -> row;
	int col = p -> col;
	int sum = 0;
	
	for(int i = 0; i < times_to_run; i++){
		sum = 0;
		for(int j = row; j < ROW_LENGTH; j++){
			sum += entries[j][col];
		}
		col += 1;
		
		if(sum != ROW_COL_TOTAL){
			correct = 0;
		}
	}
	pthread_exit(NULL);
}
void* horizontal(void* param){
	
	/*create parameter pointer to void data pointer*/
	parameters *p = (parameters*) param;
	
	/*instantiate row and column start values*/
	int row = p -> row;
	int col = p -> col;

	
	int sum = 0;

	for(int i = 0; i < times_to_run; i++){
		sum = 0;
		for(int j = col; j < COL_LENGTH; j++){
			sum += entries[row][j];
		}
		row += 1;
		
		if(sum != ROW_COL_TOTAL){
			correct = 0;
		}
	}
	pthread_exit(NULL);
}
void* _3x3(void* param){
	
	/*create parameter pointer to void data pointer*/
	parameters *p = (parameters*) param;
	
	/*instantiate grid row and column start values*/
	int row = p -> row;
	int col = p -> col;

	/*sum = 0*/		
	int sum = 0;
	
	if(times_to_run3x3 == 1){
	
		/*sums the entries to test if 3x3 grid is valid*/
		for(int r = row; r < row + 3; r++){
			for(int c = col; c < col + 3; c++){
				sum += entries[r][c];
			}
		}
	
		if(sum != ROW_COL_TOTAL){
			correct = 0;
		}
	}
	else if(times_to_run3x3 == 9){
		for(int squareR = row; squareR < 9; squareR++){
			for(int squareC = col; squareC < 9; squareC++){
				/*if statement finds the start of each 3x3 grid*/
				if(squareR % 3 == 0 && squareC % 3 == 0){
					/*sums the entries to test if 3x3 grid is valid*/
					for(int r = squareR; r < squareR + 3; r++){
						for(int c = squareC; c < squareC + 3; c++){
							sum += entries[r][c];
						}
					}
	
					if(sum != ROW_COL_TOTAL){
						correct = 0;
					}
					/*reset sum*/
					sum = 0;
				}
			}
		}
	}
		
	pthread_exit(NULL);
}
int main(int argc, char* argv[]){
	
	int option = atoi(argv[1]);
	
	
	/*creates input file */
	FILE* input;
	
	input = fopen("input.txt", "r");

	/*checks if file failed*/
	if(!input){
		perror("Failed to open file: \n");
	}
	
	/*scans input file int 2d array*/
	for(int i = 0; i < ROW_LENGTH; i++){
		for(int j = 0; j < COL_LENGTH; j++){
			fscanf(input, "%d", &entries[i][j]);
		}
	}
	
	/*prints sudoku board*/
	printf("BOARD STATE IN input.txt:\n");
	for(int i = 0; i < ROW_LENGTH; i++){
		for(int j = 0; j < COL_LENGTH; j++){
			printf("%d ", entries[i][j]);
		}
		printf("\n");
	}
	
	/*start timer*/
	double duration;
	clock_t t;
	t = clock();
	
	/*option 1*/
	if(option == 1){
		/*build parameter struct*/
		parameters *data = (parameters *) malloc(sizeof(parameters));
		
		/*set times to run for row & col runners*/
		times_to_run = 9;
		
		/*check column thread*/
		pthread_t vertThread1;
		
		/*check row thread*/
		pthread_t horiThread1;
		
		/*set data*/
		data -> row = 0;
		data -> col = 0;
		
		/*create threads with data*/
		pthread_create(&vertThread1, NULL, vertical, data);
		pthread_create(&horiThread1, NULL, horizontal, data);
		
		/*set times for 3x3 runner to run*/
		times_to_run3x3 = 1;
		
		/*create thread to check 3x3*/
		pthread_t _3x3Thread[9];
		
		/*build parameter struct for 9 threads*/
		/*need to allocate a pointer to 9 pointers*/
		parameters **data3x3 = malloc(9*sizeof(parameters*));

		/*current is which thread & data3x3 is currently being used*/
		int current = 0;
		for(int r = 0; r < 9; r++){
			for(int c = 0; c < 9; c++){
				/*if statement finds the start of each 3x3 grid*/
				if(r % 3 == 0 && c % 3 == 0){
					
					/*allocate each struct pointer that is pointed to by ** */
					data3x3[current] = (parameters *) malloc(sizeof(parameters));
					
					/*set data3x3 struct for each thread that searches a 3x3*/
					data3x3[current] -> row = r;
					data3x3[current] -> col = c;
					
					/*create a thread with data*/
				pthread_create(&_3x3Thread[current], NULL, _3x3, data3x3[current]);
					
					/*increment current*/
					current += 1;	
					
				}
			}
		}
		/*join threads*/
		pthread_join(vertThread1, NULL);
		pthread_join(horiThread1, NULL);
		
		for(int i = 0; i < 9; i++){
			pthread_join(_3x3Thread[i], NULL);
		}
		
		/*free dynamically allocated data struct*/
		free(data3x3); 
		/*free dynamically allocated data struct*/
		free(data);
	}
	/*option 2*/
	else if(option == 2){
		
		/*set times to run for row & col runners*/
		times_to_run = 1;
		
		/*build column parameter struct for 9 threads*/
		/*need to allocate a pointer to 9 pointers*/
		parameters **colData = malloc(9*sizeof(parameters*));
		
		/*create the check column threads*/
		pthread_t vertThread2[9];
		
		for(int c = 0; c < 9; c++){
			/*allocate each struct pointer that is pointed to by ** */
			colData[c] = (parameters *) malloc(sizeof(parameters));
			
			/*set colData struct for each thread that searches a col*/
			colData[c] -> row = 0;
			colData[c] -> col = c;
			
			/*create a thread with data*/
			pthread_create(&vertThread2[c], NULL, vertical, colData[c]);
		}
		
		/*build column parameter struct for 9 threads*/
		/*need to allocate a pointer to 9 pointers*/
		parameters **rowData = malloc(9*sizeof(parameters*));
		
		/*create the check row threads*/
		pthread_t horiThread2[9];
		
		for(int r = 0; r < 9; r++){
			/*allocate each struct pointer that is pointed to by ** */
			rowData[r] = (parameters *) malloc(sizeof(parameters));
			
			/*set colData struct for each thread that searches a col*/
			rowData[r] -> row = r;
			rowData[r] -> col = 0;
			
			/*create a thread with data*/
			pthread_create(&horiThread2[r], NULL, horizontal, rowData[r]);
		}
		
		/*set times for 3x3 runner to run*/
		times_to_run3x3 = 1;
		
		/*create the check 3x3 threads*/
		pthread_t _3x3Thread[9];
		
		/*build 3x3 parameter struct for 9 threads*/
		/*need to allocate a pointer to 9 pointers*/
		parameters **data3x3 = malloc(9*sizeof(parameters*));

		/*current is which thread & data3x3 is currently being used*/
		int current = 0;
		for(int r = 0; r < 9; r++){
			for(int c = 0; c < 9; c++){
				/*if statement finds the start of each 3x3 grid*/
				if(r % 3 == 0 && c % 3 == 0){
					
					/*allocate each struct pointer that is pointed to by ** */
					data3x3[current] = (parameters *) malloc(sizeof(parameters));
					
					/*set data3x3 struct for each thread that searches a 3x3*/
					data3x3[current] -> row = r;
					data3x3[current] -> col = c;
					
					/*create a thread with data*/
				pthread_create(&_3x3Thread[current], NULL, _3x3, data3x3[current]);
					
					/*increment current*/
					current += 1;	
					
				}
			}
		}
		/*join threads*/
		for(int rc = 0; rc < 9; rc++){
			pthread_join(vertThread2[rc], NULL);
			pthread_join(horiThread2[rc], NULL);
		}
		
		
		for(int i = 0; i < 9; i++){
			pthread_join(_3x3Thread[i], NULL);
		}
		
		/*free dynamically allocated data struct*/
		free(data3x3); 
		free(rowData);
		free(colData);
	}
	/*option 3 - 3 children, 1 thread per child*/
	else if(option == 3){
		/*pids of all three children*/
		pid_t pid1, pid2, pid3;
		
		/*fork child process 1*/
		pid1 = fork();
		
		/*in parent*/
		if(pid1 > 0){
			/*fork child process 2*/
			pid2 = fork();
			
			/*in parent*/
			if(pid2 > 0){
				/*fork child process 3*/
				pid3 = fork();
				
				/*in parent*/
				if(pid3 > 0){					
					/*wait for children*/
					wait(NULL);
					wait(NULL);
					wait(NULL);
				}
				/*failed to fork child 3*/
				else if(pid3 < 0){
					fprintf(stderr, "Fork failed");
				}
				/*child 3 - 3x3*/
				else{
					
					/*build parameter struct*/
				parameters *child3Data = (parameters *) malloc(sizeof(parameters));
					
					/*set times for 3x3 runner to run*/
					times_to_run3x3 = 9;
					
					/*create thread to check 3x3*/
					pthread_t _3x3Thread;
					
					/*set data*/
					child3Data -> row = 0;
					child3Data -> col = 0;
					
					pthread_create(&_3x3Thread, NULL, _3x3, child3Data);
					
					/*join threads*/
					pthread_join(_3x3Thread, NULL);
					
					/*free dynamically allocated data struct*/
					free(child3Data);
					
					/*exits child process*/
					exit(0);
				}
			}
			/*failed to fork child 2*/
			else if(pid2 < 0){
				fprintf(stderr, "Fork failed");
			}
			/*child 2 - columns*/
			else{
				
				/*build parameter struct*/
				parameters *child2Data = (parameters *) malloc(sizeof(parameters));
		
				/*set times to run for row & col runners*/
				times_to_run = 9;
				
				/*check column thread*/
				pthread_t vertThread3;
				
				/*set data*/
				child2Data -> row = 0;
				child2Data -> col = 0;
				
				/*create threads with data*/
				pthread_create(&vertThread3, NULL, vertical, child2Data);
				
				/*join threads*/
				pthread_join(vertThread3, NULL);
				
				/*free dynamically allocated data struct*/ 
				free(child2Data);
			
				/*exits child process*/
				exit(0);
			}
		}
		/*failed to fork child 1*/
		else if(pid1 < 0){
			fprintf(stderr, "Fork failed");
		}
		/*child 1 - rows*/
		else{
			
			/*build parameter struct*/
			parameters *child1Data = (parameters *) malloc(sizeof(parameters));
		
			/*set times to run for row & col runners*/
			times_to_run = 9;
		
			/*check row thread*/
			pthread_t horiThread3;
		
			/*set data*/
			child1Data -> row = 0;
			child1Data -> col = 0;
		
			/*create threads with data*/
			pthread_create(&horiThread3, NULL, horizontal, child1Data);
			
			/*join threads*/
			pthread_join(horiThread3, NULL);
			
			/*free dynamically allocated data struct*/
			free(child1Data);
			
			/*exits child process*/
			exit(0);
		}
		
	}
	
	/*stops timer*/
	t = clock() - t;
	duration = ((double)t/CLOCKS_PER_SEC);
	
	if(correct == 0)
		printf("SOLUTION: NO (%f seconds)\n", duration);
	else
		printf("SOLUTION: YES (%f seconds)\n", duration);

	return 0;
}
