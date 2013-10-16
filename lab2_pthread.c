#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRAY_SIZE 10000
#define NUMBER_OF_THREADS 4
#define RANDOM_RANGE 10
#define SUPPRESS_PRINT_ARRAY 1
#define SUPPRESS_PRINT_THREAD 1


/* Calculation mutex */
pthread_mutex_t cmutex;

int array_a[ARRAY_SIZE], array_b[ARRAY_SIZE], array_c[ARRAY_SIZE];
int smallest_value, largest_value, average_value;


void print_array(int array_x[ARRAY_SIZE], char *name)
{
	if (!SUPPRESS_PRINT_ARRAY){
		printf("%s: ", name);
		int i;
		for(i = 0; i < ARRAY_SIZE; i++){
			printf("%d ", array_x[i]);
		}
		printf("\n");
	}
}

void *thread_worker(void *thread_id)
/*
 *	Worker thread
 */
{
	int i;
	long tid;
	tid = (long)thread_id;
	int chunk_size = ARRAY_SIZE / NUMBER_OF_THREADS;
	int start_index = tid * chunk_size;

	int end_index = (tid + 1) * chunk_size - 1;

	if (tid == NUMBER_OF_THREADS - 1)
		end_index = ARRAY_SIZE - 1;

	if (!SUPPRESS_PRINT_THREAD)
		printf("Thread ID: %ld, start_index: %d, end_index: %d\n",
	           tid, start_index, end_index);

	// Populating Array A and B with random numbers
	for(i = start_index; i <= end_index; i++){
		array_a[i] = rand() % RANDOM_RANGE;
		array_b[i] = rand() % RANDOM_RANGE;
	}

	// Populating Array C that contains the average of A and B
	for(i = start_index; i <= end_index; i++){
		array_c[i] = (array_a[i] + array_b[i]) / 2;
	}

	// Initialize local values for the batch
	int local_smallest_value = 10;
	int local_largest_value = -1;
	int local_sum_value = 0;

	// Calculate local values (smallest, largest and sum for the chunk)
	for(i = start_index; i <= end_index; i++){
		if (array_c[i] < local_smallest_value){
			local_smallest_value = array_c[i];
		}
		if (array_c[i] > local_largest_value){
			local_largest_value = array_c[i];
		}
		local_sum_value += array_c[i];
	}

	// Update global values using mutex
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
	// Initialize global variable values
	smallest_value = 10;
	largest_value = -1;
	average_value = 0;

	// Set random seed
	srand(time(NULL) + 1);

	pthread_t threads[NUMBER_OF_THREADS];
	int rc;
	long t;

	pthread_mutex_init(&cmutex, NULL);

	// Create worker threads
	for(t = 0; t < NUMBER_OF_THREADS; t++){
		//printf("Creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, thread_worker, (void *)t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	// Join worker threads
	for(t = 0; t < NUMBER_OF_THREADS; t++){
		rc = pthread_join(threads[t], NULL);
		if (rc){
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_destroy(&cmutex);

	print_array(array_a, "Array A");
	print_array(array_b, "Array B");
	print_array(array_c, "Array C");

	average_value /= ARRAY_SIZE;

	printf("Smallest: %d, Largest: %d, Average: %d\n",
		   smallest_value, largest_value, average_value);

	pthread_exit(NULL);
	return 0;
}
