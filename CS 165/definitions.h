#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MEMOIZED_TYPE long long
#define MAX_N 10000
#define MAX_K 500
#define RUNS_PER_CASE 1000

#ifdef __GNUC__
#define nullptr NULL
#endif

/* Instructor-provided comparison and random functions */
int COMPARE(int arg1, int arg2, ...);
double dshrandom(long input);

/* Memoization functions */
bool greater_than(int index1, int index2);
void memo_increment();
void memo_clear();

/* BST functions */
namespace bst
{
	void init();
	void push(int element);
	void get_descending_order(int* dest, int offset);
}

#endif