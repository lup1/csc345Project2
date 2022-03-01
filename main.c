#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>//
#include <errno.h>//
#include <string.h>

#define ROW_LENGTH 9
#define COL_LENGTH 9
#define ROW_COL_TOTAL 45

pthread_mutex_t lock;//FIGURE THIS STUFF OUT
//sem_t sem;

/*tracks if any rows, columns, or squares fail to meet criteria*/
int correct = 1;

/*sudoku entries*/
int entries[9][9];

/*for tracking how many times to call runner functions - dependent on # of threads*/
int times_to_run;

/*structure for passing data to threads*/
typedef struct{
	int row;
	int col;
}parameters;

void* vertical(void* param){
	
	/*lock critical section*/
	pthread_mutex_lock(&lock);
	parameters *p = (parameters*) param;
	int row = p -> row;
	int col = p -> col;
	/*unlock critical section*/
	pthread_mutex_unlock(&lock);
	
	int sum = 0;
	
	for(int i = 0; i < times_to_run; i++){
		sum = 0;
		for(int j = row; j < ROW_LENGTH; j++){
			//printf("%d\n", entries[j][col]);//TEST CODE
			sum += entries[j][col];
		}
		col += 1;
		
		if(sum != ROW_COL_TOTAL){
			correct = 0;
		}
	
		//printf("Correct: %d\n", correct);//TEST CODE
	}
	pthread_exit(NULL);
}
void* horizontal(void* param){
	
	/*lock critical section*/
	//pthread_mutex_lock(&lock);
	//don't need lock in veritcal & horizontal bc same data & time before 3x3
	parameters *p = (parameters*) param;
	int row = p -> row;
	int col = p -> col;
	/*unlock critical section*/
	//pthread_mutex_unlock(&lock);
	
	int sum = 0;

	for(int i = 0; i < times_to_run; i++){
		sum = 0;
		for(int j = col; j < COL_LENGTH; j++){
			//printf("%d\n", entries[row][j]);//TEST CODE
			sum += entries[row][j];
		}
		row += 1;
		
		if(sum != ROW_COL_TOTAL){
			correct = 0;
		}
	
		//printf("Correct: %d\n", correct);//TEST CODE
	}
	pthread_exit(NULL);
}
void* _3x3(void* param){
//CHANGE BODY
	
	/*lock critical section*/
	pthread_mutex_lock(&lock);
	parameters *p = (parameters*) param;
	int row = p -> row;//row1
	int col = p -> col;
	
	/*unlock critical section*/
	pthread_mutex_unlock(&lock);
	
	//if(sem_post(&sem) != 0)/*signal*/
	//	printf("%s\n", strerror(errno));
		
	int sum = 0;
	
	printf("3x3:\nr: %d\nc: %d\n",row,col);//TEST CODE
	
	for(int r = row; r < row + 3; r++){
		for(int c = col; c < col + 3; c++){
			sum += entries[r][c];
			printf("%d ", entries[r][c]);
		}
		printf("\n");
	}
	
	if(sum != ROW_COL_TOTAL){
			correct = 0;
	}
		
	
	printf("Correct: %d\n", correct);//TEST CODE
	pthread_exit(NULL);
}
int main(int argc, char* argv[]){
	
	int option = atoi(argv[1]);
	
	
	/*creates input file */
	FILE* input;
	
	input = fopen("input.txt", "r");

	//checks if file failed
	if(!input){
		printf("oopsie\n");
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
	
	/*build parameter struct*/
	parameters *data = (parameters *) malloc(sizeof(parameters));
	
	/*initialize lock*/
	pthread_mutex_init(&lock, NULL);
	
	//if(sem_init(&sem, 0, 1) == -1)/*initialize semaphore*/
		printf("%s\n", strerror(errno));
	
	/*option 1*/
	if(option == 1){
		
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
		
		
		/*check 3x3*/
		pthread_t _3x3Thread[9];
		int current = 0;
		for(int r = 0; r < 9; r++){
			for(int c = 0; c < 9; c++){
				if(r % 3 == 0 && c % 3 == 0){
					//printf("main:\nr: %d\nc: %d\n",r,c);//TEST CODE
					/*set data*/
					data -> row = r;
					data -> col = c;
					
					/*create a thread with data*/
					pthread_create(&_3x3Thread[current], NULL, _3x3, data);
					
					//if(sem_wait(&sem) != 0)/*wait*/
					//	printf("%s\n", strerror(errno));
					
					
					current += 1;

					
					/*controls timing for synchronization*/
					sleep(1);//0.5
					
					
					
					
					
					
				}
			}
		}
		/*join threads*/
		pthread_join(vertThread1, NULL);
		pthread_join(horiThread1, NULL);
		
		for(int i = 0; i < 9; i++){
			pthread_join(_3x3Thread[i], NULL);
		}
		
	}
	/*destroy lock*/
	pthread_mutex_destroy(&lock);
	//if(sem_destroy(&sem) != 0)/*destroy*/
	//	printf("%s\n", strerror(errno));
	return 0;
}
