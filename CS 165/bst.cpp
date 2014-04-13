#include <cstdio>
#include "definitions.h"

extern int k;

namespace bst
{
	struct Node
	{
		Node *parent, *left, *right;
		int element;
		Node(int element) : element(element) 
		{
			parent = left = right = nullptr;
		}
	};

	Node *root = nullptr, *least = nullptr;
	int size;

	void delete_tree(Node *& treeRoot);
	void find_bst_parent(int element, Node *& parent, bool & leftChild);
	Node * delete_least();
	int reverse_order_traverse(Node * current, int * dest, int i);

	void init()
	{
		delete_tree(bst::root);
		bst::root = bst::least = nullptr;
		bst::size = 0;
	}

	void delete_tree(Node *& treeRoot)
	{
		if (treeRoot == nullptr)
			return;
		delete_tree(treeRoot->left);
		delete_tree(treeRoot->right);
		treeRoot->parent = nullptr;
		delete treeRoot;
		treeRoot = nullptr;
	}

	void push(int element)
	{
		// If the tree is empty, record the new node
		// as the root and the least node.
		if (bst::size == 0)
		{
			bst::root = bst::least = new Node(element);
			bst::size = 1;
			return;
		}

		// Declare newNode here to possibly recycle
		// the old least node.
		Node * newNode = nullptr;

		// If the tree is full, remove the least element
		// before adding the new one.
		if (bst::size == k)
		{
			// We want only the greatest elements in the
			// bst, so if the new element is less than
			// the least element, reject it.
			if (proofs::lesser(element, bst::least->element))
				return;
			newNode = delete_least();
			newNode->element = element;
			bst::size--;
		}
		else
			newNode = new Node(element);
		Node * parent = nullptr;
		bool leftChild = false;
		find_bst_parent(element, parent, leftChild);
		// Add new node as child of parent.
		newNode->parent = parent;
		if (leftChild)
		{
			if (parent->left != nullptr)
				printf("Error: new node is left child of parent but parent->left != nullptr.\n");
			parent->left = newNode;
		}
		else
		{
			if (parent->right != nullptr)
				printf("Error: new node is right child of parent but parent->right != nullptr.\n");
			parent->right = newNode;
		}
		// Also update the least pointer if necessary.
		if (leftChild && parent == bst::least)
			bst::least = newNode;
		// Finally, increase size.
		bst::size++;
	}

	void find_bst_parent(int element, Node *& parent, bool & leftChild)
	{
		Node *current = bst::root;
		parent = nullptr;
		while (current != nullptr)
		{
			parent = current;
			leftChild = proofs::greater(current->element, element);
			current = leftChild ? current->left : current->right;
		}
	}

	Node * delete_least()
	{
		Node * oldLeast = bst::least;
		if (oldLeast->left != nullptr)
			printf("ERROR: oldLeast->left != nullptr\n");

		bool hasParent = bst::least->parent != nullptr;
		bool hasRight = bst::least->right != nullptr;

		if (hasParent && hasRight)
		{
			bst::least->parent->left = bst::least->right;
			bst::least->right->parent = bst::least->parent;
			bst::least = bst::least->right;
			while (bst::least->left != nullptr)
				bst::least = bst::least->left;
		}
		else if (hasParent && !hasRight)
		{
			bst::least->parent->left = nullptr;
			bst::least = bst::least->parent;
		}
		else if (!hasParent && hasRight)
		{
			bst::least->right->parent = nullptr;
			bst::root = bst::least = bst::least->right;
			while (bst::least->left != nullptr)
				bst::least = bst::least->left;
		}
		else if (!hasParent && !hasRight)
		{
			printf("bst::delete_least() reached the !hasParent && !hasRight case, which should never happen because we don't want to call delete_least until size == k.");
			bst::root = bst::least = nullptr;
		}

		// Detach the old least node from the tree.
		oldLeast->parent = oldLeast->left = oldLeast->right = nullptr;
		return oldLeast;
	}

	void get_descending_order(int* dest, int offset)
	{
		int i = offset;
		i = reverse_order_traverse(root, dest, i);
		if (i - offset != k)
			printf("bst::get_descending_order did not visit k (%d) elements, rather %d.\n", k, i);
	}

	int reverse_order_traverse(Node * current, int * dest, int i)
	{
		if (current->right != nullptr)
			i = reverse_order_traverse(current->right, dest, i);
		dest[i++] = current->element;
		if (current->left != nullptr)
			i = reverse_order_traverse(current->left, dest, i);
		return i;
	}
}