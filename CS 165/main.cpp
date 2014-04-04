#include "compare.c"

const int MAX_N = 1000000;
const int MAX_K = 1000000;
const int K_OUT_OF_RANGE = -1;
const int WRONG_INDICES = -1000;
const int RUNS_PER_CASE = 10;

int indices[MAX_N];
int best[MAX_K + 1];

bool greaterThan(int index1, int index2);

void selectionSort(int count);
void quickSort(int count);

void doalg(int n, int k)
{
	if (n > MAX_N || k > MAX_K)
	{
		printf("N or K is too large, max K is %d, max N is %d.", MAX_N, MAX_K);
		return;
	}

	// Initialize elements and indices.
	COMPARE(0, n);
	for (int i = 0; i < n; i++)
		indices[i] = i + 1; // Elements indexed starting from 1, not 0.
	
	// Sort the indices in descending order, then copy
	// the highest k indices into best.
	selectionSort(n);
	for (int i = 0; i < k; i++)
		best[i + 1] = indices[i];
}

struct Case
{
	int n, k;

	Case(int n, int k) : n(n), k(k) {}
};

void printArray(int * arr, int count)
{
	printf("[");
	for (int i = 0; i < count - 1; i++)
		printf("%d, ", arr[i]);
	printf("%d]", arr[count - 1]);
}

int main()
{
	Case cases[4] = { Case(15, 3), Case(100, 10), Case(1000, 20), Case(10000, 40) };

	for (int c = 0; c < 4; c++)
	{
		int maxComparisons = 0,
			totalComparisons = 0;

		for (int i = 0; i < RUNS_PER_CASE; i++)
		{
			doalg(cases[c].n, cases[c].k);

			int result = COMPARE(-1, cases[c].k, best);

			if (result == K_OUT_OF_RANGE)
				printf("k (%d) is out of range.\n", cases[c].k);
			else if (result == WRONG_INDICES)
				printf("doalg() failed to return the correct indices for the %d greatest elements.\n", cases[c].k);
			else
			{
				if (result > maxComparisons)
					maxComparisons = result;
				totalComparisons += result;
			}
		}

		printf("For n=%d and k=%d, max comparisons is %d and average comparisons is %f.\n\n", cases[c].n, cases[c].k, maxComparisons, totalComparisons / (double)RUNS_PER_CASE);
	}

	getchar();

	return 0;
}

bool greaterThan(int index1, int index2)
{
	return COMPARE(index1, index2) == 1;
}

void selectionSort(int* indices, int count)
{
	// i is the first unsorted element.
	for (int i = 0; i < count; i++)
	{
		int max = i;
		for (int j = i + 1; j < count; j++)
		{
			if (greaterThan(indices[j], indices[max]))
				max = j;
		}
		int temp = indices[max];
		indices[max] = indices[i];
		indices[i] = temp;
	}
}

int getQuicksortPivot(int first, int last)
{
	// Median of first, middle, last.
	int middle = first + (last - first) / 2;
	if (greaterThan(indices[first], indices[last]))
	{
		if (greaterThan(indices[first], indices[middle]))
			return greaterThan(indices[last], indices[middle]) ? last : middle;
		else
			return first;
	}
	else
	{
		if (greaterThan(indices[last], indices[middle]))
			return greaterThan(indices[first], indices[middle]) ? first : middle;
		else
			return last;
	}
}

void quickSortRange(int first, int last)
{
	if (first >= last)
		return;
	
}

void quickSort(int count)
{
	quickSortRange(0, count - 1);
}