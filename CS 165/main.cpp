#include <cstring>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <cerrno>
#include <cstdlib>
#include "definitions.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds millis;

/* Data structures */
// Will hold the indices of the k greatest elements in descending order.
int best[MAX_K + 1];
int shuffledIndices[MAX_N];

/* Status variables */
// Total number of elements that are currently being searched.
int n = 0;
// Number of elements that must be found.
int k = 0;

/* Operations */
void init_alg(int n, int k);
void doalg(int n, int k);
void printArray(int * arr, int count);
long int getSeed(int argc, const char ** argv);

struct Case
{
	int n, k;
	Case(int n, int k) : n(n), k(k) {}
};

int main(int argc, const char ** argv)
{
	Case cases[] = 
	{
		Case(15, 3),
		Case(15, 5),
		Case(15, 10),
		Case(15, 15),
		Case(100, 10),
		Case(100, 50),
		Case(100, 80),
		Case(100, 100),
		Case(1000, 20),
		Case(1000, 40),
		Case(1000, 100),
		Case(1000, 200),
		Case(1000, 500),
		Case(10000, 40), 
		Case(10000, 200),
		Case(10000, 500)
	};
	
	dshrandom(getSeed(argc, argv));
		
	Clock::time_point programStartTime = Clock::now();

	printf("\n%-8s%-5s%-8s%-8s%-13s%-6s\n", "n", "k", "Min.", "Max.", "Avg.", "Time (ms)");
	for (int i = 0; i < 55; i++)
		printf("-");
	printf("\n");

	proofs::program_init();

	for (int c = 0; c < sizeof(cases) / sizeof(cases[0]); c++)
	{
		int maxComparisons = 0,
			minComparisons = 1000000,
			totalComparisons = 0;
		Clock::time_point caseStartTime = Clock::now();
		proofs::case_init();

		for (int i = 0; i < RUNS_PER_CASE; i++)
		{
			doalg(cases[c].n, cases[c].k);

			int result = COMPARE(-1, cases[c].k, best);

			if (result == -1)
				printf("k (%d) is out of range.\n", cases[c].k);
			else if (result == -1000)
				printf("doalg() failed to return the correct indices for the %d greatest elements.\n", cases[c].k);
			else
			{
				if (result > maxComparisons)
					maxComparisons = result;
				if (result < minComparisons)
					minComparisons = result;
				totalComparisons += result;
			}
		}
		
		printf(
			"%-8d%-5d%-8d%-8d%-13.3f%-6d\n", 
			cases[c].n, 
			cases[c].k, 
			minComparisons, 
			maxComparisons, 
			totalComparisons / (double)RUNS_PER_CASE, 
			std::chrono::duration_cast<millis>(Clock::now() - caseStartTime).count());
		long long totalRequests = proofs::getTotalRequests();
		long long totalHits = proofs::getTotalHits();
		double avgHits = totalHits / (double)RUNS_PER_CASE;
		double avgRequests = totalRequests / (double)RUNS_PER_CASE;
		printf("total hits: %d\ntotal requests: %d\navg hits: %f\navg requests: %f\n", (int)totalHits, (int)totalRequests, avgHits, avgRequests);
	}
	printf("\n");
	
	printf(
		"Took %.2f seconds to run all cases %d times each.\n", 
		std::chrono::duration_cast<millis>(Clock::now() - programStartTime).count() / 1000.0,
		RUNS_PER_CASE);

	return 0;
}

long int getSeed(int argc, const char ** argv)
{
	if (argc > 1)
	{
		char * endChar = nullptr;
		const int Radix = 0;
		long int result = strtol(argv[1], &endChar, Radix);
		if (result == 0 || errno == ERANGE)
		{
			result = (long int)std::time(nullptr);
			printf("Invalid seed argument. Substituted seed from std::time() call: %d\n", result);
			return result;
		}
		else
		{
			printf("Using provided seed: %d\n", result);
			return result;
		}
	}
	else
	{
		long int timeVal = (long int)std::time(nullptr);
		printf("Seed from std::time() call: %d\n", timeVal);
		return (long int)timeVal;
	}
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

	// Clear best array and bst.
	std::memset(best, 0, k + 1);
	bst::init();
	proofs::init();

	// Fill shuffled array.
	for (int i = 0; i < n; i++)
		shuffledIndices[i] = i + 1;
	// Do Fisher-Yates shuffle.
	for (int i = n; i >= 1; i--){
		int j = (int)(i*dshrandom(0) + 1);
		if (j != i) {
			int t = shuffledIndices[i - 1];
			shuffledIndices[i - 1] = shuffledIndices[j - 1];
			shuffledIndices[j - 1] = t;
		}
	}
	
	COMPARE(0, n);
}

void doalg(int n, int k)
{
	init_alg(n, k);

	// Add all indices to heap. Remember indices start at 1.
	for (int i = 0; i < n; i++)
		bst::push(shuffledIndices[i]);

	// Remove k elements from heap and put them in descending
	// order in the best array.
	bst::get_descending_order(best, 1);
}