#include <cstring>
#include <cstdio>
#include <ctime>
#include "definitions.h"

/* Data structures */
// Will hold the indices of the k greatest elements in descending order.
int best[MAX_K + 1]; 



/* Status variables */
// Total number of elements that are currently being searched.
int n = 0;
// Number of elements that must be found.
int k = 0;



/* Operations */
void init_alg(int n, int k);
void doalg(int n, int k);
void printArray(int * arr, int count);

struct Case
{
	int n, k;
	Case(int n, int k) : n(n), k(k) {}
};

int main()
{
	Case cases[] = { Case(15, 3), Case(100, 10), Case(1000, 20), Case(10000, 40) };
	
	time_t seed = std::time(nullptr);
	dshrandom(std::time(nullptr));
	printf("Seed from std::time() call is: %d.\n", seed);

	for (int j = 0; j < 1; j++)
	{
		printf("%-8s%-5s%-8s%-8s\n", "n", "k", "Max.", "Avg.");
		for (int i = 0; i < 8 + 5 + 8 + 8; i++)
			printf("-");
		printf("\n");

		memo_clear();

		for (int c = 0; c < 4; c++)
		{
			int maxComparisons = 0,
				totalComparisons = 0;

			for (int i = 0; i < RUNS_PER_CASE; i++)
			{
				doalg(cases[c].n, cases[c].k);
				memo_increment();

				int result = COMPARE(-1, cases[c].k, best);

				if (result == -1)
					printf("k (%d) is out of range.\n", cases[c].k);
				else if (result == -1000)
					printf("doalg() failed to return the correct indices for the %d greatest elements.\n", cases[c].k);
				else
				{
					if (result > maxComparisons)
						maxComparisons = result;
					totalComparisons += result;
				}
			}

			printf("%-8d%-5d%-8d%-8.2f\n", cases[c].n, cases[c].k, maxComparisons, totalComparisons / (double)RUNS_PER_CASE);
		}
		printf("\n");
	}
	
	getchar();

	return 0;
}

void printArray(int * arr, int count)
{
	printf("[");
	for (int i = 0; i < count - 1; i++)
		printf("%d, ", arr[i]);
	printf("%d]", arr[count - 1]);
}

void init_alg(int n, int k)
{
	::n = n;
	::k = k;

	// Clear best array and heap.
	std::memset(best, 0, k + 1);
	heap_init();
	
	COMPARE(0, n);
}

void doalg(int n, int k)
{
	init_alg(n, k);

	// Add all indices to heap. Remember indices start at 1.
	for (int i = 1; i <= n; i++)
		heap_push(i);

	// Remove k elements from heap and put them in descending
	// order in the best array.
	for (int i = 0; i < k; i++)
		best[k - i] = heap_pop();
}