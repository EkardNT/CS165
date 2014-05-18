#include "Project3.h"
#include "LempelZiv.h"

namespace Project3
{
	LempelZivEncoder::LempelZivEncoder(std::shared_ptr<std::ostream> outputStream, std::int32_t windowSize)
		: ICompressionAlg(outputStream), windowSize(windowSize)
	{

	}

	LempelZivEncoder::~LempelZivEncoder()
	{

	}

	void LempelZivEncoder::Process(std::uint8_t byte)
	{
		throw std::exception("Not implemented.");
	}

	void LempelZivEncoder::Finish()
	{
		throw std::exception("Not implemented.");
	}

	LempelZivDecoder::LempelZivDecoder(std::shared_ptr<std::ostream> outputStream, std::int32_t windowSize)
		: ICompressionAlg(outputStream), windowSize(windowSize)
	{

	}

	LempelZivDecoder::~LempelZivDecoder()
	{

	}

	void LempelZivDecoder::Process(std::uint8_t byte)
	{
		throw std::exception("Not implemented.");
	}

	void LempelZivDecoder::Finish()
	{
		throw std::exception("Not implemented.");
	}
}