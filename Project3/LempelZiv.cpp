#include "Project3.h"
#include "LempelZiv.h"
#include "Bits.h"
#include <map>
#include <algorithm>

using namespace std;

namespace Project3
{
	const int LookaheadSize = 16;

	struct MapElement
	{
		uint8_t elements[LookaheadSize];
		int count;
		int position;
	};

	struct MapElementComparer
	{
		// Defines a strict weak ordering on MapElements which is:
		// - Irreflexive: f(x, x) is false
		// - Antisymmetric: f(x, y) implies !f(y, x)
		// - Transitive: f(x, y) and f(y, z) implies f(x, z)
		bool operator() (const MapElement & a, const MapElement & b)
		{
			for (int i = 0; i < LookaheadSize; i++)
			{
				if (a.elements[i] < b.elements[i])
					return true;
				if (a.elements[i] > b.elements[i])
					return false;
				// Otherwise a[i] == b[i], continue examining.
			}
			return false;
		}
	};

	LempelZivEncoder::LempelZivEncoder(std::int32_t windowSize)
		: windowSize(windowSize)
	{

	}

	LempelZivEncoder::~LempelZivEncoder()
	{

	}

	bool Matches(std::shared_ptr<std::vector<std::uint8_t>> inputData, uint64_t offset, int lbPosition, int searchLength)
	{
		if (offset > lbPosition)
			return false;
		for (int i = 0; i < searchLength; i++)
		{
			if (inputData->operator[](lbPosition - offset + i) != inputData->operator[](lbPosition + i))
				return false;
		}
		return true;
	}

	void LempelZivEncoder::Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData)
	{
		BitWriter writer(outputStream);

		int offsetBitCount = 0;
		if (windowSize == 4096)
			offsetBitCount = 12;
		else if (windowSize == 2048)
			offsetBitCount = 11;
		else
			throw std::exception();

		map<MapElement, MapElement, MapElementComparer> lvMap;
		MapElement element;
		
		for (int lbPosition = 0; lbPosition < inputData->size(); /* noop */)
		{
			int initialLength = 16;
			if (inputData->size() - lbPosition < 16)
				initialLength = inputData->size() - lbPosition;

			for (int elementIndex = 0; elementIndex < initialLength; elementIndex++)
				element.elements[elementIndex] = inputData->at(elementIndex + lbPosition);
			for (int elementIndex = initialLength; elementIndex < 16; elementIndex++)
				element.elements[elementIndex] = 0;
			element.position = lbPosition;

			for (int searchLength = initialLength; searchLength >= 2; searchLength--)
			{
				element.count = searchLength;

				for (uint64_t offset = 2; offset <= windowSize / 4; offset++)
				{
					if (Matches(inputData, offset, lbPosition, searchLength))
					{
						// Error checking
						if (searchLength < 2 || searchLength > 16)
							throw exception();

						writer.WriteBits(searchLength - 1, 4);
						writer.WriteBits(offset, offsetBitCount);
						
						lbPosition += searchLength;
						goto EndOfILoop;
					}
				}
			}
			
			element.count = initialLength;
			lvMap.insert(make_pair(element, element));

			writer.WriteBits(0, 4);
			writer.WriteBits(initialLength - 1, 4);
			for (int i = 0; i < initialLength; i++)
			{
				uint64_t val = element.elements[i];
				writer.WriteBits(val, 8);
			}
			
			lbPosition += initialLength;

		EndOfILoop:
			continue;
		}

		// EOF
		writer.WriteBits(0, 8);
		writer.Flush();
	}

	LempelZivDecoder::LempelZivDecoder(int32_t windowSize)
		: windowSize(windowSize)
	{

	}

	LempelZivDecoder::~LempelZivDecoder()
	{

	}

	void LempelZivDecoder::Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData)
	{
		BitReader reader(inputData);

		vector<uint8_t> outputBuffer;
		outputBuffer.reserve(inputData->size() * 2);

		uint64_t windowStart = 0;

		while (true)
		{
			uint64_t length = 0;
			reader.ReadBits(length, 4);

			if (length == 0)
			{
				uint64_t strLen = 0;
				reader.ReadBits(strLen, 4);
				
				// The (0, 0) token represents EOF.
				if (strLen == 0)
					break;

				strLen++;

				for (int i = 0; i < strLen; i++)
				{
					uint64_t charValue = 0;
					reader.ReadBits(charValue, 8);
					outputBuffer.push_back((uint8_t)charValue);
				}

				windowStart += strLen;
			}
			else
			{
				length++;

				int offsetBitCount = 0;
				if (windowSize == 4096)
					offsetBitCount = 12;
				else if (windowSize == 2048)
					offsetBitCount = 11;
				else
					throw std::exception();

				uint64_t offset = 0;
				reader.ReadBits(offset, offsetBitCount);

				if (offset > windowStart)
					throw exception();

				for (uint64_t i = 0; i < length; i++)
				{
					uint64_t readPosition = windowStart - offset + i;
					outputBuffer.push_back(outputBuffer.at(readPosition));
				}

				windowStart += length;
			}
		}

		for (int i = 0; i < outputBuffer.size(); i++)
		{
			char charVal = (char)outputBuffer.at(i);
			outputStream.write(&charVal, 1);
		}
	}
}