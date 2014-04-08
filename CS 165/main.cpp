#include <cstring>
#include <cstdio>
#include <ctime>

#define MEMOIZED_TYPE char

/* Forward declarations for compare functions. */
int COMPARE(int arg1, int arg2, ...);
double dshrandom(long input);

/* Constants */
const int MaxN = 10000;
const int MaxK = 40;
const int RunsPerCase = 100;

/* Data structures */
// Will hold the indices of the k greatest elements in descending order.
int best[MaxK + 1]; 
// A min heap of indices.
int heap[MaxK]; 
// A record of all comparisons, stored as a 2D array,
// where the value of the array element is:
//	0 if the elements have not been compared.
//	1 if the first index is greater than the second index.
//	2 if the first index is lesser than the second index.
MEMOIZED_TYPE memoizedComparisons[MaxN * MaxN]; 

/* Status variables */
// Total number of elements that are currently being searched.
int n = 0;
// Number of elements that must be found.
int k = 0;
// Number of elements in the heap, equivalent to 
// the first unoccupied heap array index.
int heapSize = 0;
MEMOIZED_TYPE memoizedBase = 0;

/* Operations */
void init_alg(int n, int k);
void doalg(int n, int k);
bool greater_than(int index1, int index2);
void heap_push(int element);
int heap_pop();
int heap_parent(int childSlot);
int heap_left_child(int parentSlot);
int heap_right_child(int parentSlot);
int heap_lesser_child(int parentSlot);
void heap_swap(int& slot1, int& slot2);
void printArray(int * arr, int count);

struct Case
{
	int n, k;
	Case(int n, int k) : n(n), k(k) {}
};

int main()
{
	Case cases[] = { Case(15, 3), Case(100, 10), Case(1000, 20), Case(10000, 40) };

	for (int c = 0; c < 4; c++)
	{
		int maxComparisons = 0,
			totalComparisons = 0;
		memoizedBase = 0;

		for (int i = 0; i < RunsPerCase; i++)
		{
			dshrandom(std::time(nullptr));
			doalg(cases[c].n, cases[c].k);
			memoizedBase += 2;

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

		printf("For n=%d and k=%d, max comparisons is %d and average comparisons is %f.\n\n", cases[c].n, cases[c].k, maxComparisons, totalComparisons / (double)RunsPerCase);
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
	std::memset(heap, 0, k);
	heapSize = 0;
	// Clear memoized comparisons.
	for (int i = 0; i < n * n; i++)
		memoizedComparisons[i] = 0;
	
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

bool greater_than(int index1, int index2)
{
	index1--;
	index2--;
	MEMOIZED_TYPE memoized = memoizedComparisons[index1 + index2 * n];
	// Already computed?
	if (memoized >(MEMOIZED_TYPE)0)
		return memoized == (MEMOIZED_TYPE)1;
	// Otherwise, compute and check return value for error.
	int result = COMPARE(index1 + 1, index2 + 1);
	if (result == -1)
	{
		printf("greater_than() called with out of bounds indices %d and/or %d.", index1, index2);
		return false;
	}
	// Record the result in both possible locations.
	memoizedComparisons[index1 + index2 * n] = (MEMOIZED_TYPE)result;
	memoizedComparisons[index2 + index1 * n] = (MEMOIZED_TYPE)(result == 1 ? 2 : 1);
	return result == 1;
}

void heap_walk_down(int startSlot)
{
	int currentSlot = startSlot;
	int lesserChildSlot = heap_lesser_child(currentSlot);
	// Move the new element down the heap while it has
	// children and the new element is greater than
	// the lesser of its two children.
	while (currentSlot != lesserChildSlot 
		&& greater_than(heap[currentSlot], heap[lesserChildSlot]))
	{
		heap_swap(currentSlot, lesserChildSlot);
		lesserChildSlot = heap_lesser_child(currentSlot);
	}
}

void heap_push(int element)
{
	// If the heap is not full, then we need to add
	// the element to the first available spot then
	// walk it up to its proper place.
	if (heapSize < k)
	{
		heap[heapSize] = element;
		int currentSlot = heapSize;
		int parentSlot = heap_parent(currentSlot);
		heapSize++;
		// Move the new element up the heap while it hasn't
		// reached the root and it is less than its parent.
		while (currentSlot > 0 
			&& !greater_than(heap[currentSlot], heap[parentSlot]))
		{
			heap_swap(currentSlot, parentSlot);
			parentSlot = heap_parent(currentSlot);
		}
		return;
	}
	// Otherwise, the heap is full. Although this
	// is a min-heap, we are only interested in the
	// k greatest elements, so we don't need to add
	// the element to the heap if it is less than
	// the root (minimum) element.
	if (greater_than(heap[0], element))
		return;
	// Finally, we know the heap is full and the
	// new element is greater than the minimum
	// element currently contained in the heap. To
	// add the new element, we replace the existing
	// root (minimum) element with the new element,
	// then walk it down the heap to where it belongs.
	heap[0] = element;
	heap_walk_down(0);
	// Note we replaced an element, so the heap size
	// does not change.
}

int heap_pop()
{
	// Check for empty error case.
	if (heapSize == 0)
	{
		printf("Called heap_pop() but the heap is empty.");
		return -1;
	}
	int minimalElement = heap[0];
	// Replace the root with the last leaf, then walk
	// the last leaf down until it arrives at its proper
	// place.
	heap[0] = heap[heapSize - 1];
	heapSize--;
	heap_walk_down(0);
	return minimalElement;
}

int heap_parent(int childSlot)
{
	return (childSlot - 1) / 2;
}

int heap_left_child(int parentSlot)
{
	return parentSlot * 2 + 1;
}

int heap_right_child(int parentSlot)
{
	return parentSlot * 2 + 2;
}

void heap_swap(int& slot1, int& slot2)
{
	int elementTemp = heap[slot1];
	heap[slot1] = heap[slot2];
	heap[slot2] = elementTemp;
	int slotTemp = slot1;
	slot1 = slot2;
	slot2 = slotTemp;
}

int heap_lesser_child(int parentSlot)
{
	int leftSlot = heap_left_child(parentSlot),
		rightSlot = heap_right_child(parentSlot);
	if (leftSlot >= heapSize)
		return parentSlot;
	if (rightSlot >= heapSize)
		return leftSlot;
	return greater_than(heap[leftSlot], heap[rightSlot]) ? rightSlot : leftSlot;
}