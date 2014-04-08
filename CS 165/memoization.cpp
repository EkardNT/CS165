#ifndef MEMOIZATION_H
#define MEMOIZATION_H

#include <cstdio>
#include <cstring>
#include "definitions.h"

extern int n;

// A record of all comparisons, stored as a 2D array,
// where the value of the array element is:
//	0 if the elements have not been compared.
//	1 if the first index is greater than the second index.
//	2 if the first index is lesser than the second index.
MEMOIZED_TYPE memoizedComparisons[MAX_N * MAX_N];

MEMOIZED_TYPE memoizedBase = 0;

bool greater_than(int index1, int index2)
{
	index1--;
	index2--;
	MEMOIZED_TYPE memoized = memoizedComparisons[index1 + index2 * n] - memoizedBase;
	// Already computed?
	if (memoized > (MEMOIZED_TYPE)0)
		return memoized == (MEMOIZED_TYPE)1;
	// Otherwise, compute and check return value for error.
	int result = COMPARE(index1 + 1, index2 + 1);
	if (result == -1)
	{
		printf("greater_than() called with out of bounds indices %d and/or %d.", index1, index2);
		return false;
	}
	// Record the result in both possible locations.
	memoizedComparisons[index1 + index2 * n] = (MEMOIZED_TYPE)result + memoizedBase;
	memoizedComparisons[index2 + index1 * n] = (MEMOIZED_TYPE)(result == 1 ? 2 : 1) + memoizedBase;
	return result == 1;
}

void memo_increment()
{
	memoizedBase += 2;
}

void memo_clear()
{
	std::memset(memoizedComparisons, 0, MAX_N * MAX_N);
}

#endif