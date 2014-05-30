#include "Project3.h"
#include "Huffman.h"
#include <queue>
#include "BitBuilder.h"

namespace Project3
{
	struct Node
	{
		// Nodes own their child nodes if they are not null and
		// thus delete them in their constructor.
		Node *left, *right;
		// Either the value in [0, 256) of this leaf node,
		// or -1 to indicate this is not a leaf node.
		std::int32_t value;
		// How often the value was encountered in the input data if
		// this is a leaf node, or the sum of the two child nodes if this
		// is not a leaf node.
		std::uint32_t frequency;

		Node(std::int32_t value, std::uint32_t frequency) : left(nullptr), right(nullptr), value(value), frequency(frequency) {}
		~Node()
		{
			if (left != nullptr)
				delete left;
			if (right != nullptr)
				delete right;
		}
	};

	struct NodeComparer
	{
		bool operator() (const Node * a, const Node * b) 
		{
			return a->frequency < b->frequency;
		}
	};

	void OutputHuffmanCodeBits(std::uint8_t value, Node * codeTree, BitBuilder & builder)
	{

	}

	HuffmanEncoder::HuffmanEncoder(std::shared_ptr<std::ostream> outputStream)
		: ICompressionAlg(outputStream)
	{
		for (int i = 0; i < 256; i++)
			frequencies[i] = 0;
	}

	HuffmanEncoder::~HuffmanEncoder()
	{

	}

	void HuffmanEncoder::Process(std::uint8_t byte)
	{
		frequencies[byte]++;
		storedBytes.push_back(byte);
	}

	void HuffmanEncoder::Finish()
	{
		// All frequencies are recorded, now we have to build
		// the Huffman code tree.

		// This priority queue stores value-frequency pairs in increasing magnitude of frequency.
		std::priority_queue<Node*, std::vector<Node*>, NodeComparer> pq;
		for (int i = 0; i < 256; i++)
			pq.push(new Node(i, frequencies[i]));

		// While the queue has more than one node still in it, take the two
		// nodes with least frequency and create a parent node for them
		// which is the sum of their two frequencies, then add that node
		// back into the queue.
		while (pq.size() > 1)
		{
			Node* least = pq.top();
			pq.pop();
			Node* secondLeast = pq.top();
			pq.pop();
			Node* newParent = new Node(-1, least->frequency + secondLeast->frequency);
			pq.push(newParent);
		}

		// Our Huffman code tree.
		Node* root = pq.top();

		// Now go over the input data in a second pass
		for (auto iter = storedBytes.cbegin(); iter != storedBytes.cend(); iter++)
		{

		}
	}

	HuffmanDecoder::HuffmanDecoder(std::shared_ptr<std::ostream> outputStream)
		: ICompressionAlg(outputStream)
	{

	}

	HuffmanDecoder::~HuffmanDecoder()
	{

	}

	void HuffmanDecoder::Process(std::uint8_t byte)
	{
		throw std::exception("Not implemented.");
	}

	void HuffmanDecoder::Finish()
	{
		throw std::exception("Not implemented.");
	}
}