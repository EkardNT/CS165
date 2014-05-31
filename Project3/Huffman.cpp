#include "Project3.h"
#include "Huffman.h"
#include <queue>
#include "Bits.h"

using namespace std;

namespace Project3
{
	HuffmanEncoder::HuffmanEncoder()
	{

	}

	HuffmanEncoder::~HuffmanEncoder()
	{

	}

	struct Node
	{
		Node *left, *right;
		int value;
		int frequency;

	};

	struct NodeOrderer
	{
		bool operator() (Node* & a, Node* & b)
		{
			return a->frequency > b->frequency;
		}
	};

	Node* BuildHuffmanTree(int* frequencies)
	{
		// Build huffman encoding tree.
		priority_queue<Node*, vector<Node*>, NodeOrderer> stack;

		for (int i = 0; i < 256; i++)
		{
			if (frequencies[i] == 0)
				continue;
			Node * node = new Node();
			node->left = node->right = nullptr;
			node->frequency = frequencies[i];
			node->value = i;
			stack.push(node);
		}

		while (stack.size() > 1)
		{
			Node * least = stack.top();
			stack.pop();
			Node * secondLeast = stack.top();
			stack.pop();
			Node * internalNode = new Node();
			internalNode->left = least;
			internalNode->right = secondLeast;
			internalNode->frequency = least->frequency + secondLeast->frequency;
			internalNode->value = -1;
			stack.push(internalNode);
		}

		return stack.top();
	}

	void ExpandBinaryPaths(Node* root, vector<bool> code, uint64_t * bitStrings, int * bitStringLengths)
	{
		if (root->value == -1)
		{
			if (code.size() == 64)
				throw std::exception("Huffman code greater than 64 bits.");
			code.push_back(false);
			ExpandBinaryPaths(root->left, code, bitStrings, bitStringLengths);
			code.pop_back();
			code.push_back(true);
			ExpandBinaryPaths(root->right, code, bitStrings, bitStringLengths);
			code.pop_back();
		}
		else
		{
			uint64_t finalCode = 0;
			for (int i = 0; i < code.size(); i++)
			{
				finalCode = (finalCode << 1) | (code[i] ? 1 : 0);
			}
			bitStrings[root->value] = finalCode;
			bitStringLengths[root->value] = code.size();
		}
	}

	void WriteHuffmanTree(Node * tree, BitWriter & writer)
	{
		if (tree->value == -1)
		{
			writer.WriteBits(0, 1);
			WriteHuffmanTree(tree->left, writer);
			WriteHuffmanTree(tree->right, writer);
		}
		else
			writer.WriteBits(tree->value, 8);
	}

	void HuffmanEncoder::Execute(ostream & outputStream, shared_ptr<vector<uint8_t>> inputData)
	{
		// First pass
		int frequencies[256];
		memset(frequencies, 0, 256 * sizeof(int));

		for (int i = 0; i < inputData->size(); i++)
		{
			frequencies[inputData->at(i)]++;
		}

		Node * huffmanTree = BuildHuffmanTree(frequencies);

		std::uint64_t bitStrings[256];
		int bitStringLengths[256];
		ExpandBinaryPaths(huffmanTree, vector<bool>(), bitStrings, bitStringLengths);

		BitWriter writer(outputStream);
		WriteHuffmanTree(huffmanTree, writer);

		for (int i = 0; i < inputData->size(); i++)
		{
			uint8_t value = inputData->at(i);
			uint64_t bitString = bitStrings[value];
			int bitLength = bitStringLengths[value];
			writer.WriteBits(bitString, bitLength);
		}

		writer.Flush();
	}

	HuffmanDecoder::HuffmanDecoder()
	{

	}

	HuffmanDecoder::~HuffmanDecoder()
	{

	}

	void HuffmanDecoder::Execute(ostream & outputStream, shared_ptr<vector<uint8_t>> inputData)
	{

	}
}