#include "Project3.h"

namespace Project3
{
	class LempelZivEncoder : public ICompressionAlg
	{
	public:
		LempelZivEncoder(std::shared_ptr<std::ostream> outputStream, std::int32_t windowSize);
		virtual ~LempelZivEncoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();

	private:
		std::int32_t windowSize;
	};

	class LempelZivDecoder : public ICompressionAlg
	{
	public:
		LempelZivDecoder(std::shared_ptr<std::ostream> outputStream, std::int32_t windowSize);
		virtual ~LempelZivDecoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();

	private:
		std::int32_t windowSize;
	};
}