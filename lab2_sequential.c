#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRAY_SIZE 10
#define THREAD_NUM 4
#define RANDOM_RANGE 10


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

void populate_average_numbers(
		int array_x[ARRAY_SIZE],
		int array_y[ARRAY_SIZE],
		int array_output[ARRAY_SIZE])
{
	int i;
	for(i = 0; i < ARRAY_SIZE; i++){
		array_output[i] = (array_x[i] + array_y[i]) / 2;
	}
}

int get_smallest_number(int array_x[ARRAY_SIZE])
{
	int smallest_value = 10;
	int i;
	for(i = 0; i < ARRAY_SIZE; i++){
		if (array_x[i] < smallest_value){
			smallest_value = array_x[i];
		}
	}
	return smallest_value;

}

int get_largest_number(int array_x[ARRAY_SIZE])
{
	int largest_value = -1;
	int i;
	for(i = 0; i < ARRAY_SIZE; i++){
		if (array_x[i] > largest_value){
			largest_value = array_x[i];
		}
	}
	return largest_value;
	
}

int get_average_number(int array_x[ARRAY_SIZE])
{
	int sum_value, average_value;
	int i;
	for(i = 0; i < ARRAY_SIZE; i++){
		sum_value += array_x[i];
	}
	average_value = sum_value / ARRAY_SIZE;
	return average_value;

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

int main()
{

	populate_random_numbers(array_a, 1);
	populate_random_numbers(array_b, 2);
	populate_average_numbers(array_a, array_b, array_c);
	print_array(array_a, "Array A");
	print_array(array_b, "Array B");
	print_array(array_c, "Array C");
	smallest_value = get_smallest_number(array_c);
	largest_value = get_largest_number(array_c);
	average_value = get_average_number(array_c);
	printf("Smallest: %d, Largest: %d, Average: %d\n",
		   smallest_value, largest_value, average_value);
	return 0;

}

