#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRAY_SIZE 10
#define NUMBER_OF_THREAD 4
#define RANDOM_RANGE 10
#define SUPPRESS_PRINT_ARRAY 1
#define SUPPRESS_PRINT_THREAD 1
#define CHUNK_SIZE 10


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

int main()
{
	// Initialize global variable values
	smallest_value = 10;
	largest_value = -1;
	average_value = 0;

	// Set random seed
	srand(time(NULL) + 1);

	// Declare private variables for threads
	int i, tid, nthreads;

	omp_set_num_threads(NUMBER_OF_THREAD);

	#pragma omp parallel shared(smallest_value, largest_value, average_value) \
	private(i, tid, nthreads)
	{
		#pragma omp for schedule(dynamic)
		for(i = 0; i < ARRAY_SIZE; i++){

			tid = omp_get_thread_num();
			if (!SUPPRESS_PRINT_THREAD)
				printf("Thread ID: %d\n", tid);

			// Master thread
			if (tid == 0) {
				nthreads = omp_get_num_threads();
				if (!SUPPRESS_PRINT_THREAD)
					printf("Number of threads = %d\n", nthreads);
			}

			array_a[i] = rand() % RANDOM_RANGE;
			array_b[i] = rand() % RANDOM_RANGE;
			array_c[i] = (array_a[i] + array_b[i]) / 2;

			#pragma omp critical
			{
				if (array_c[i] < smallest_value){
					smallest_value = array_c[i];
				}
				if (array_c[i] > largest_value){
					largest_value = array_c[i];
				}
				average_value += array_c[i];
			}
		}
	}

	print_array(array_a, "Array A");
	print_array(array_b, "Array B");
	print_array(array_c, "Array C");

	average_value /= ARRAY_SIZE;

	printf("Smallest: %d, Largest: %d, Average: %d\n",
		   smallest_value, largest_value, average_value);
	return 0;

}