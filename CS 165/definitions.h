#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_N 10000
#define MAX_K 500
#define RUNS_PER_CASE 1000

#ifdef __GNUC__
#define nullptr NULL
#endif

/* Instructor-provided random function */
double dshrandom(long input);
/* Instructor-provided comparison function. */
int COMPARE(int arg1, int arg2, ...);

/* Relational proof functions */
namespace proofs
{
	bool greater(int expectedGreaterIndex, int expectedLesserIndex);
	bool lesser(int expectedLesserIndex, int expectedGreaterIndex);
}

/* BST functions */
namespace bst
{
	void init();
	void push(int element);
	void get_descending_order(int* dest, int offset);
}

#endif