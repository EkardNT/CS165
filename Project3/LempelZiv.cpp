#include "Project3.h"
#include "LempelZiv.h"

namespace Project3
{
	LempelZivEncoder::LempelZivEncoder(std::int32_t windowSize)
		: windowSize(windowSize)
	{

	}

	LempelZivEncoder::~LempelZivEncoder()
	{

	}

	void LempelZivEncoder::Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData)
	{

	}

	LempelZivDecoder::LempelZivDecoder(std::int32_t windowSize)
		: windowSize(windowSize)
	{

	}

	LempelZivDecoder::~LempelZivDecoder()
	{

	}

	void LempelZivDecoder::Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData)
	{

	}
}