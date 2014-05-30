#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Project3.h"
#include <array>

namespace Project3
{
	class HuffmanEncoder : public ICompressionAlg
	{
	public:
		HuffmanEncoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~HuffmanEncoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();

	private:
		// Frequency table for all 256 possible byte values.
		std::array<std::uint32_t, 256> frequencies;
		std::vector<std::uint8_t> storedBytes;
	};

	class HuffmanDecoder : public ICompressionAlg
	{
	public:
		HuffmanDecoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~HuffmanDecoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();
	};
}

#endif