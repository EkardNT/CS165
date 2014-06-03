#include "Bits.h"

using namespace std;

namespace Project3
{
	BitWriter::BitWriter(ostream & output) : output(output), buffer(0), writePosition(0)
	{

	}

	// Writes from lsb to msb.
	void BitWriter::WriteBits(uint64_t bits, int count)
	{
		// i is the bit position in the bits field to take the bit from
		// writePosition is the bit position in the buffer to write the bit to.
		for (int i = 0; i < count; i++)
		{
			uint8_t bit = 1 & (bits >> i);
			buffer |= bit << writePosition;
			writePosition++;
			if (writePosition == 8)
				Flush();
		}
	}

	void BitWriter::Flush()
	{
		if (writePosition == 0)
			return;
		char writeBuffer = buffer;
		output.write(&writeBuffer, 1);
		buffer = writePosition = 0;
	}

	BitReader::BitReader(shared_ptr<vector<uint8_t>> inputData)
		: inputData(inputData), bytePosition(0), readPosition(0)
	{

	}

	// Reads from lsb of the current byte to msb.
	// Adds the read bits to the bits parameter starting at the lsb
	// and moving toward the msb.
	void BitReader::ReadBits(uint64_t & bits, int count, bool advanceReadPosition)
	{
		bits = 0;
		int initialBytePosition = bytePosition, initialReadPosition = readPosition;
		// i is the bit position where the read bit will be written to,
		// readPosition is the bit position where the bit will be read
		// from in the current byte.
		for (int i = 0; i < count; i++)
		{
			uint8_t containingByte = inputData->at(bytePosition);
			uint8_t bit = 1 & (containingByte >> readPosition);
			readPosition++;
			bits |= bit << i;
			if (readPosition == 8)
			{
				bytePosition++;
				readPosition = 0;
			}
		}
		if (!advanceReadPosition)
		{
			bytePosition = initialBytePosition;
			readPosition = initialReadPosition;
		}
	}
}