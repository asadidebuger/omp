#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include "omp.h"

void quickSort_parallel(int* array, int lenArray, int numThreads);
void quickSort_parallel_internal(int* array, int left, int right, int cutoff);
void quickSort(int* arr, int left, int right);
int cmpfunc (const void * a, const void * b);
int lenArr = 10000000*4;
int numthreads = 8;

int main(){
	int minMum = 1;
	int maxNum = lenArr;
	int maxNumbersDisplayed = 30;

	double startTime, stopTime;

	int* arr1;
	int* arr2; 
	int* arr3;
	arr1 = (int*) malloc(lenArr*sizeof(int));
	arr2 = (int*) malloc(lenArr*sizeof(int));
	arr3 = (int*) malloc(lenArr*sizeof(int));
	

	printf("Muhamadreza Asadi 983624004 OMP Quicksort:\n" );
	printf("\nSize of the array \n", lenArr );
	// Initialise the array with random numbers
	int i;
	srand(5); // seed
	printf("fill the arrays with random numbers...\n");
	FILE *f = fopen("data.txt", "w");
	for (i=0; i<lenArr; i++){
		// RAND_MAX = 2147483647 = INT_MAX 
		// printf("RAND_MAX %u \n", RAND_MAX );
		int num=minMum+(rand()%maxNum);
		arr1[i] = num;
		arr2[i] = num;
		arr3[i] = num;
		char snum[8]="";

		// convert 123 to string [buf]
		itoa(num, snum, 10);
		fwrite("\n", 1, sizeof(char), f);
		fwrite(snum, 1, sizeof(snum), f);
		fwrite("\r", 1, sizeof(char), f);
		//printf("%d \n", arr1[i] ); 
	}
	
	fclose(f);
	printf("Array Inited!....\n");



	
	if( lenArr <= maxNumbersDisplayed ){
		printf("\n\nArray BEFORE sorting: \n");
		for( i = 0 ; i < lenArr; i++ ) 
		{
			printf("%d ", arr1[i]);
		}
		printf("\n\n\n");
	}


	printf("\nSorting with serial standard 'qsort' function of  ..."); fflush(stdout);
	startTime = clock();
	qsort(arr1, lenArr, sizeof(int), cmpfunc);
	stopTime = clock();
	
	printf("\nSorted in: %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with my serial QuickSort...");
	fflush(stdout);
	startTime = clock();
	quickSort(arr2, 0, lenArr-1);
	stopTime = clock();

	printf("\nSorted in : %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);



	printf("\nSorting with custom PARALLEL QuickSort... "); fflush(stdout);
	startTime = omp_get_wtime();
	quickSort_parallel(arr3, lenArr, numthreads);
	stopTime = omp_get_wtime();

	FILE *fparalel = fopen("result.txt", "wb");

	for( i = 0 ; i < lenArr; i++ ) 
	{
		char snum[8]="";

		// convert 123 to string [buf]
		itoa(arr3[i], snum, 10);
		fwrite("\n", 1, sizeof(char), fparalel);
		fwrite(snum, 1, sizeof(snum), fparalel);
		fwrite("\r", 1, sizeof(char), fparalel);
	}
	printf("\nSorted in (aprox.): %f seconds \n\n", (stopTime-startTime));

	free(arr1);
	free(arr2);
	free(arr3);

	return 0;
}

void quickSort_parallel(int* array, int lenArray, int numThreads){

	int cutoff = 1000;

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp single nowait
		{
			quickSort_parallel_internal(array, 0, lenArray-1, cutoff);
		}
	}

}



void quickSort_parallel_internal(int* array, int left, int right, int cutoff)
{

	int i = left, j = right;
	int tmp;
	int pivot = array[(left + right) / 2];


	{
		/* PARTITION PART */
		while (i <= j) {
			while (array[i] < pivot)
				i++;
			while (array[j] > pivot)
				j--;
			if (i <= j) {
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				i++;
				j--;
			}
		}

	}


	if ( ((right-left)<cutoff) ){
		if (left < j){ quickSort_parallel_internal(array, left, j, cutoff); }
		if (i < right){ quickSort_parallel_internal(array, i, right, cutoff); }

	}else{
#pragma omp task
		{ quickSort_parallel_internal(array, left, j, cutoff); }
#pragma omp task
		{ quickSort_parallel_internal(array, i, right, cutoff); }
	}

}

void quickSort(int* arr, int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];


	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j){ quickSort(arr, left, j);  }
	if (i< right){ quickSort(arr, i, right); }
}

int cmpfunc (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}
