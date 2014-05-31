#pragma once
#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <cstdint>

namespace Project3
{
	class BitWriter
	{
	private:
		std::ostream & output;
		std::uint8_t buffer, bufferSize;

	public:
		BitWriter(std::ostream & output);

		void WriteBits(std::uint64_t bits, int count);
		void Flush();
	};
}

#endif