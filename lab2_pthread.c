#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRAY_SIZE 100000
#define THREAD_NUM 4
#define RANDOM_RANGE 10

/* Calculation mutex */
pthread_mutex_t cmutex;

int array_a[ARRAY_SIZE], array_b[ARRAY_SIZE], array_c[ARRAY_SIZE];
int smallest_value, largest_value, average_value;


void populate_random_numbers(int array_x[ARRAY_SIZE], int seed)
{
	int i;
	srand(time(NULL) + seed);
	for(i = 0; i < ARRAY_SIZE; i++){
		array_x[i] = rand() % RANDOM_RANGE;
	}
}

void print_array(int array_x[ARRAY_SIZE], char *name)
{
	printf("%s: ", name);
	int i;
	for(i = 0; i < ARRAY_SIZE; i++){
		printf("%d ", array_x[i]);
	}
	printf("\n");
}


void *thread_pan(void *thread_id)
/*
 *	Populate average numbers
 */
{
	int i;
	long tid;
	tid = (long)thread_id;
	int batch_size = ARRAY_SIZE / THREAD_NUM;
	int start_index = tid * batch_size;

	int end_index = (tid + 1) * batch_size - 1;
	if (tid == THREAD_NUM - 1)
		end_index = ARRAY_SIZE - 1;
	//printf("Thread ID: %ld, start_index: %d, end_index: %d\n", tid, start_index, end_index);
	for(i = start_index; i <= end_index; i++){
		array_c[i] = (array_a[i] + array_b[i]) / 2;
	}
	pthread_exit(NULL);
}

void *thead_cal(void *thread_id)
/*
 *	Calculate smallest, largest, and average value
 */
{
	int i;
	long tid;
	tid = (long)thread_id;
	int batch_size = ARRAY_SIZE / THREAD_NUM;
	int start_index = tid * batch_size;

	int end_index = (tid + 1) * batch_size - 1;
	if (tid == THREAD_NUM - 1)
		end_index = ARRAY_SIZE - 1;
	//printf("Thread ID: %ld, start_index: %d, end_index: %d\n", tid, start_index, end_index);


	int local_smallest_value = 10;
	int local_largest_value = -1;
	int local_sum_value = 0;
	for(i = start_index; i <= end_index; i++){
		if (array_c[i] < smallest_value){
			local_smallest_value = array_c[i];
		}
		if (array_c[i] > largest_value){
			local_largest_value = array_c[i];
		}
		local_sum_value += array_c[i];
	}
	pthread_mutex_lock(&cmutex);
	if (local_smallest_value < smallest_value)
		smallest_value = local_smallest_value;
	if (local_largest_value > largest_value)
		largest_value = local_largest_value;
	average_value += local_sum_value;
	pthread_mutex_unlock(&cmutex);

	pthread_exit(NULL);
}


int main()
{
	pthread_t threads[THREAD_NUM];
	int rc;
	long t;
	populate_random_numbers(array_a, 1);
	populate_random_numbers(array_b, 2);
	print_array(array_a, "Array A");
	print_array(array_b, "Array B");

	// Creating thread for populating Array C
	for(t = 0; t < THREAD_NUM; t++){
		printf("Creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, thread_pan, (void *)t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}

	}
	for(t = 0; t < THREAD_NUM; t++){
		rc = pthread_join(threads[t], NULL);
		if (rc){
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	print_array(array_c, "Array C");
	pthread_mutex_init(&cmutex, NULL);

	// Initialize global variable
	smallest_value = 10;
	largest_value = -1;
	average_value = 0;

	// Creating threads for calculation
	for(t = 0; t < THREAD_NUM; t++){
		printf("Creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, thead_cal, (void *)t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}

	}

	for(t = 0; t < THREAD_NUM; t++){
		rc = pthread_join(threads[t], NULL);
		if (rc){
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	average_value /= ARRAY_SIZE;
	printf("Smallest: %d, Largest: %d, Average: %d\n",
		   smallest_value, largest_value, average_value);
	pthread_mutex_destroy(&cmutex);
	pthread_exit(NULL);
}
