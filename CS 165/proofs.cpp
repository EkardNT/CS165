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
		MEMOIZED_TYPE memoized = memoizedComparisons[expectedGreaterIndex + expectedLesserIndex * n] - memoizedBase;
		// Already computed?
		if (memoized > (MEMOIZED_TYPE)0)
			return memoized == (MEMOIZED_TYPE)1;
		// Otherwise, compute and check return value for error.
		int result = COMPARE(expectedGreaterIndex + 1, expectedLesserIndex + 1);
		if (result == -1)
		{
			printf("greater_than() called with out of bounds indices %d and/or %d.", expectedGreaterIndex, expectedLesserIndex);
			return false;
		}
		// Record the result in both possible locations.
		memoizedComparisons[expectedGreaterIndex + expectedLesserIndex * n] = (MEMOIZED_TYPE)result + memoizedBase;
		memoizedComparisons[expectedLesserIndex + expectedGreaterIndex * n] = (MEMOIZED_TYPE)(result == 1 ? 2 : 1) + memoizedBase;
		return result == 1;
	}

	bool lesser(int expectedLesserIndex, int expectedGreaterIndex)
	{
		return greater(expectedGreaterIndex, expectedLesserIndex);
	}
}
#endif