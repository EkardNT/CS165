#ifndef PROOFS_H
#define PROOFS_H

#include <cstdio>
#include <cstring>
#include "definitions.h"

extern int n;

namespace proofs
{
	// A record of all comparisons, stored as a 2D array,
	// where the value of the array element is:
	//	0 if the elements have not been compared.
	//	1 if the first index is greater than the second index.
	//	2 if the first index is lesser than the second index.
	MEMOIZED_TYPE memoizedComparisons[MAX_N * MAX_N];

	MEMOIZED_TYPE memoizedBase = -2;

	long long totalHits, totalRequests;

	long long getTotalHits() { return totalHits; }
	long long getTotalRequests() { return totalRequests; }

	void write(long long int x, long long int y, bool greater);
	char read(long long x, long long y);

	void case_init()
	{
		totalHits = totalRequests = 0;
	}

	void program_init()
	{
		std::memset(memoizedComparisons, -2, MAX_N * MAX_N);
	}

	void init()
	{
		memoizedBase += 2;
	}

	bool greater(int expectedGreaterIndex, int expectedLesserIndex)
	{
		expectedGreaterIndex--;
		expectedLesserIndex--;
		//MEMOIZED_TYPE memoized = memoizedComparisons[expectedGreaterIndex + expectedLesserIndex * n] - memoizedBase;
		char memoized = read(expectedGreaterIndex, expectedLesserIndex);
		// Already computed?
		totalRequests++;
		if (memoized > 0)
		{
			totalHits++;
			return memoized == 1;
		}
		// Otherwise, compute and check return value for error.
		int result = COMPARE(expectedGreaterIndex + 1, expectedLesserIndex + 1);
		if (result == -1)
		{
			printf("greater_than() called with out of bounds indices %d and/or %d.", expectedGreaterIndex, expectedLesserIndex);
			return false;
		}
		write(expectedGreaterIndex, expectedLesserIndex, result == 1);

		return result == 1;
	}

	bool lesser(int expectedLesserIndex, int expectedGreaterIndex)
	{
		return greater(expectedGreaterIndex, expectedLesserIndex);
	}

	void write(long long x, long long y, bool greater)
	{
		// Record the result in both possible locations.
		memoizedComparisons[x + y * n] = (greater ? 1 : 2) + memoizedBase;
		memoizedComparisons[y + x * n] = (greater ? 2 : 1) + memoizedBase;
	}

	char read(long long x, long long y)
	{
		MEMOIZED_TYPE val = memoizedComparisons[x + y * n] - memoizedBase;
		if (val < 0)
			return 0;
		return val;
	}
}
#endif