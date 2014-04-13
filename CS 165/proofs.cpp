#ifndef PROOFS_H
#define PROOFS_H

#include <cstdio>
#include <cstring>
#include "definitions.h"

extern int n;

namespace proofs
{
	void write(long long int x, long long int y, bool greater);
	char read(long long x, long long y);
	
	bool greater(int expectedGreaterIndex, int expectedLesserIndex)
	{
		int result = COMPARE(expectedGreaterIndex, expectedLesserIndex);
		if (result == -1)
			printf("greater_than() called with out of bounds indices %d and/or %d.", expectedGreaterIndex, expectedLesserIndex);
		return result == 1;
	}

	bool lesser(int expectedLesserIndex, int expectedGreaterIndex)
	{
		return greater(expectedGreaterIndex, expectedLesserIndex);
	}
}
#endif