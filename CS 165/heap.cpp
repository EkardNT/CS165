#include <cstring>
#include <cstdio>
#include "definitions.h"

extern int n;
extern int k;

int heap_parent(int childSlot);
int heap_left_child(int parentSlot);
int heap_right_child(int parentSlot);
int heap_lesser_child(int parentSlot);
void heap_swap(int& slot1, int& slot2);

// A min heap of indices.
int heap[MAX_K]; 
// Number of elements in the heap, equivalent to 
// the first unoccupied heap array index.
int heapSize = 0;

void heap_init()
{
	std::memset(heap, 0, k);
	heapSize = 0;
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