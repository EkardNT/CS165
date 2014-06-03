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
		int value; // Need int for -1 signifier.
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
				throw std::exception();
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
		{
			uint64_t bits = (tree->value << 1) | 1;
			writer.WriteBits(bits, 9);
		}
	}

	// Reverses the order of the least significant 'count' bits in 'bits'.
	// The remaining most significant bits are set to 0.
	uint64_t ReverseBitOrder(uint64_t bits, int count)
	{
		uint64_t result = 0;
		for (int i = 0; i < count; i++)
		{
			uint64_t bit = 1 & (bits >> i);
			result |= bit << (count - 1 - i);
		}
		return result;
	}

	void HuffmanEncoder::Execute(ostream & outputStream, shared_ptr<vector<uint8_t>> inputData)
	{
		// First pass
		int frequencies[256];
		for (int i = 0; i < 256; i++)
			frequencies[i] = 0;

		for (int i = 0; i < inputData->size(); i++)
		{
			frequencies[inputData->at(i)]++;
		}

		Node * huffmanTree = BuildHuffmanTree(frequencies);

		uint64_t bitStrings[256];
		int bitStringLengths[256];
		ExpandBinaryPaths(huffmanTree, vector<bool>(), bitStrings, bitStringLengths);

		BitWriter writer(outputStream);
		WriteHuffmanTree(huffmanTree, writer);

		// Write the number of elements as a 32 bit unsigned number.
		uint64_t size = inputData->size();
		writer.WriteBits(size, 32);

		for (int i = 0; i < inputData->size(); i++)
		{
			uint8_t value = inputData->at(i);
			uint64_t bitString = bitStrings[value];
			int bitLength = bitStringLengths[value];
			uint64_t bitStringReversed = ReverseBitOrder(bitString, bitLength);
			writer.WriteBits(bitStringReversed, bitLength);
		}

		writer.Flush();
	}

	HuffmanDecoder::HuffmanDecoder()
	{

	}

	HuffmanDecoder::~HuffmanDecoder()
	{

	}

	Node * ReconstructHuffmanTree(BitReader & reader)
	{
		Node * node = new Node();
		// We don't need to know the frequency for decoding.
		node->frequency = 0;

		uint64_t bits = 0;
		reader.ReadBits(bits, 1);
		
		if (bits == 0)
		{
			node->left = ReconstructHuffmanTree(reader);
			node->right = ReconstructHuffmanTree(reader);
			node->value = -1;
		}
		else
		{
			node->left = nullptr;
			node->right = nullptr;
			reader.ReadBits(bits, 8);
			node->value = (int)bits;
		}

		return node;
	}

	uint8_t DecodeValueFromTree(Node * root, BitReader & reader)
	{
		uint64_t bits = 0;
		Node * current = root;
		while (root->value == -1)
		{
			reader.ReadBits(bits, 1);
			root = (bits == 1) ? root->right : root->left;
		}
		return (uint8_t)root->value;
	}

	void HuffmanDecoder::Execute(ostream & outputStream, shared_ptr<vector<uint8_t>> inputData)
	{
		BitReader reader(inputData);
		Node * tree = ReconstructHuffmanTree(reader);
		
		// Read the number of encoded elements as a 32 bit number.
		uint64_t size = 0;
		reader.ReadBits(size, 32);

		for (int i = 0; i < size; i++)
		{
			uint8_t value = DecodeValueFromTree(tree, reader);
			char writeValue = value;
			outputStream.write(&writeValue, 1);
		}
	}
}