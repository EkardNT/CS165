#include "Bits.h"

using namespace std;

namespace Project3
{
	BitWriter::BitWriter(ostream & output) : output(output), buffer(0), bufferSize(0)
	{

	}

	void BitWriter::WriteBits(uint64_t bits, int count)
	{
		for (int i = count - 1; i >= 0; i--)
		{
			buffer = buffer << 1;
			buffer = buffer | (1 & (bits >> i));
			bufferSize++;
			if (bufferSize == 8)
			{
				Flush();
			}
		}
	}

	void BitWriter::Flush()
	{
		if (bufferSize == 0)
			return;
		buffer = buffer << (8 - bufferSize);
		char writeBuffer = buffer;
		output.write(&writeBuffer, 1);
		buffer = 0;
		bufferSize = 0;
	}
}