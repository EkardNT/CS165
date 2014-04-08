#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MEMOIZED_TYPE long long
#define MAX_N 10000
#define MAX_K 40
#define RUNS_PER_CASE 1000

/* Compare and random operations */
int COMPARE(int arg1, int arg2, ...);
double dshrandom(long input);

/* Heap operations */
void heap_init();
void heap_push(int element);
int heap_pop();

/* Memoization operations */
bool greater_than(int index1, int index2);
void memo_increment();
void memo_clear();

#endif