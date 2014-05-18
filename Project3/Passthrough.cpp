#include "Project3.h"
#include "Passthrough.h"

namespace Project3
{
	PassthroughEncoder::PassthroughEncoder(std::shared_ptr<std::ostream> outputStream)
		: ICompressionAlg(outputStream)
	{

	}

	PassthroughEncoder::~PassthroughEncoder()
	{

	}

	void PassthroughEncoder::Process(std::uint8_t byte)
	{
		Write(byte);
	}

	void PassthroughEncoder::Finish()
	{

	}

	PassthroughDecoder::PassthroughDecoder(std::shared_ptr<std::ostream> outputStream)
		: ICompressionAlg(outputStream)
	{

	}

	PassthroughDecoder::~PassthroughDecoder()
	{

	}

	void PassthroughDecoder::Process(std::uint8_t byte)
	{
		Write(byte);
	}

	void PassthroughDecoder::Finish()
	{

	}
}