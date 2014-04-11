#include <cstring>
#include <cstdio>
#include "definitions.h"

extern int n;
extern int k;

namespace treap
{
	struct Node
	{
		Node *parent, *leftChild, *rightChild;
		int element;
		Node(int element) : element(element)
		{
			parent = leftChild = rightChild = nullptr;
		}
	};

	// Treap size.
	int size = 0;
	
	Node * root = nullptr;

	void init()
	{
		size = 0;
		// memory leaks, yay!
		root = nullptr;
	}

	void push(int element)
	{
		// If the treap is not full, add a new node
		// as if this were a standard bst.
		if (size < k)
		{
			Node * parent = nullptr;
			bool wentLeft = false;
			// Find the bst parent.
			expected_bst_parent(element, parent, wentLeft);
			
			Node * newNode = new Node(element);
			newNode->parent = parent;

			// Parent is null if new element should be root.
			if (parent == nullptr)
				root = newNode;
			// Otherwise attach new node as correct child of parent.
			else
			{
				if (wentLeft)
					parent->leftChild = newNode;
				else
					parent->rightChild = newNode;
			}
			
			// We've just added a node, so size increases by 1.
			size++;

			// Lastly, perform a series of tree rotations
			// which preserve the bst ordering to satisfy
			// the min heap property. We only need to
			// do this if the new node is not the root
			// node, otherwise there is only one element in
			// the treap and the heap property is trivially fulfilled.
			if (newNode != root)
				walk_up(newNode);
		}
	}

	void expected_bst_parent(int element, Node *& parent, bool & wentLeft)
	{
		Node * current = root,
			 * parent = nullptr;
		while (current != nullptr)
		{
			parent = current;
			// If the new element is less than the current node's element,
			// go left, otherwise go right.
			wentLeft = greater_than(current->element, element);
			current = wentLeft ? current->leftChild : current->rightChild;
		}
	}

	void walk_up(Node * node)
	{
		Node *current = node;
		// Keep walking the node up while the current node
		// is not the root and while the heap property
		// is inverted.
		while (current->parent != nullptr 
			&& greater_than(current->parent->element, current->element))
		{
			// Perform the appropriate tree rotation to satisfy
			// the min heap property without changing bst order.
			if (current == current->parent->leftChild)
				rotate_left(current);
			else
				rotate_right(current);
			// Advance up the tree. Note current->parent has been
			// reassigned by the rotation function.
			current = current->parent;
		}
	}

	void walk_down(Node * node)
	{
		Node *current = node,
			*lesserChild = get_lesser_child(node);
		while (lesserChild != current
			&& greater_than(current->element, lesserChild->element))
		{
			// Perform the appropriate tree rotation to satisfy
			// the min heap property without changing bst order.
			if (lesserChild == current->leftChild)
				rotate_left(lesserChild);
			else
				rotate_right(lesserChild);
			// Advance down the tree. Note current has been made
			// the child of lesserChild by the rotate function.
			lesserChild = get_lesser_child(current);
		}
	}

	Node * get_lesser_child(Node * parent)
	{
		// If both children are not null, decide between the two.
		if (parent->leftChild != nullptr && parent->rightChild != nullptr)
		{
			// Remember we want the min.
			return greater_than(parent->leftChild->element, parent->rightChild->element)
				? parent->rightChild
				: parent->leftChild;
		}
		// If only one is not null, return that one.
		if (parent->leftChild != nullptr && parent->rightChild == nullptr)
			return parent->leftChild;
		if (parent->leftChild == nullptr && parent->rightChild != nullptr)
			return parent->rightChild;
		// Otherwise both are null.
		return parent;
	}

	void rotate_right(Node * oldLower)
	{
		Node *oldUpper = oldLower->parent,
			*middleSubtree = oldLower->rightChild;
		if (middleSubtree != nullptr) middleSubtree->parent = oldUpper;
		oldUpper->leftChild = middleSubtree;
		if (oldUpper->parent != nullptr)
		{
			if (oldUpper == oldUpper->parent->leftChild)
				oldUpper->parent->leftChild = oldLower;
			else
				oldUpper->parent->rightChild = oldLower;
		}
		oldLower->parent = oldUpper->parent;
		oldLower->rightChild = oldUpper;
		oldUpper->parent = oldLower;
	}

	void rotate_left(Node * oldLower)
	{
		Node * oldUpper = oldLower->parent,
			*middleSubtree = oldLower->leftChild;
		if (middleSubtree != nullptr)
			middleSubtree->parent = oldUpper;
		oldUpper->rightChild = middleSubtree;
		if (oldUpper->parent != nullptr)
		{
			if (oldUpper == oldUpper->parent->leftChild)
				oldUpper->parent->leftChild = oldLower;
			else
				oldUpper->parent->rightChild = oldLower;
		}
		oldLower->parent = oldUpper->parent;
		oldLower->leftChild = oldUpper;
		oldUpper->parent = oldLower;
	}

	void descending_order(int* dest)
	{

	}
}