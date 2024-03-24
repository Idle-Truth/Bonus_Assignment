#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t *ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void *ptr)
{
	size_t *pSz = (size_t *)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t *)ptr - 1);
}

size_t Size(void *ptr)
{
	return ((size_t *)ptr)[-1];
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void heapify(int arr[], int n, int i)
{
	int largest = i;	   // Initialize largest as root
	int left = 2 * i + 1;  // left = 2*i + 1
	int right = 2 * i + 2; // right = 2*i + 2

	// If left child is larger than root
	if (left < n && arr[left] > arr[largest])
		largest = left;

	// If right child is larger than largest so far
	if (right < n && arr[right] > arr[largest])
		largest = right;

	// If largest is not root
	if (largest != i)
	{
		swap(&arr[i], &arr[largest]);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	// Build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap
	for (int i = n - 1; i > 0; i--)
	{
		// Move current root to end
		swap(&arr[0], &arr[i]);

		// call max heapify on the reduced heap
		heapify(arr, i, 0);
	}
}

void merge(int pData[], int *L, int *R, int n1, int n2, int l)
{

	int i = 0; // Initial index of first subarray
	int j = 0; // Initial index of second subarray
	int k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			pData[k] = L[i];
			i++;
		}
		else
		{
			pData[k] = R[j];
			j++;
		}
		k++;
	}
	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		pData[k] = L[i];
		i++;
		k++;
	}
	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		pData[k] = R[j];
		j++;
		k++;
	}
	DeAlloc(L);
	DeAlloc(R);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	int m = (l + r) / 2;
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	if (r <= l)
	{
		return;
	}

	/* create temp arrays */
	int *L = (int *)Alloc(n1 * sizeof(int));
	int *R = (int *)Alloc(n2 * sizeof(int));

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
	{
		L[i] = pData[l + i];
	}

	for (j = 0; j < n2; j++)
	{
		R[j] = pData[m + 1 + j];
	}

	mergeSort(L, 0, n1 - 1);
	mergeSort(R, 0, n2 - 1);

	merge(pData, L, R, n1, n2, l);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int *pData, int n)
{
	int i, j, item;

	for (i = 1; i < n; i++)
	{
		item = pData[i];

		for (j = i - 1; j >= 0; j--)
		{
			if (pData[j] > item)
			{
				pData[j + 1] = pData[j];
			}
			else
			{
				break;
			}
		}

		pData[j + 1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int *pData, int n)
{
	int i, j, p;

	for (i = 0; i < n - 1; i++)
	{
		// last i element are already in place, so stop j before n-i
		// as we will compare j with j+1 we will stop j before n-i-1
		for (j = 0; j < n - i - 1; j++)
		{
			if (pData[j] > pData[j + 1])
			{
				swap(&pData[j], &pData[j + 1]);
				// use the value of the number being swapped to count how many times it's been swapped
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int *pData, int n)
{
	int i, j, min_index, temp;

	// One by one move boundary of unsorted subarray
	for (i = 0; i < n; i++)
	{
		min_index = i;

		// find the minimum element in unsorted array
		for (j = i + 1; j < n; j++)
		{
			if (pData[j] < pData[min_index])
			{
				min_index = j;
			}
		}

		// swap the found minimum element with the first element
		if (i != min_index)
		{
			temp = pData[i];
			pData[i] = pData[min_index];
			pData[min_index] = temp;
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);

		for (int i = 0; i < dataSz; i++)
		{
			fscanf(inFile, "%d", &(*ppData)[i]);
		}
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

	for (i = 0; i < 3; ++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)Alloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
}
