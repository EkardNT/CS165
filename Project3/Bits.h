#pragma once
#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>

namespace Project3
{
	class BitWriter
	{
	private:
		std::ostream & output;
		std::uint8_t buffer, writePosition;

	public:
		BitWriter(std::ostream & output);

		void WriteBits(std::uint64_t bits, int count);
		void Flush();
	};

	class BitReader
	{
	private:
		std::shared_ptr<std::vector<std::uint8_t>> inputData;
		int bytePosition, readPosition;

	public:
		BitReader(std::shared_ptr<std::vector<std::uint8_t>> inputData);

		void ReadBits(std::uint64_t & bits, int count, bool advanceReadPosition = true);
	};
}

#endif