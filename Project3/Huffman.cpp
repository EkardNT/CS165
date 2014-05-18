#include "Project3.h"
#include "Huffman.h"

namespace Project3
{
	HuffmanEncoder::HuffmanEncoder(std::shared_ptr<std::ostream> outputStream)
		: ICompressionAlg(outputStream)
	{

	}

	HuffmanEncoder::~HuffmanEncoder()
	{

	}

	void HuffmanEncoder::Process(std::uint8_t byte)
	{
		throw std::exception("Not implemented.");
	}

	void HuffmanEncoder::Finish()
	{
		throw std::exception("Not implemented.");
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