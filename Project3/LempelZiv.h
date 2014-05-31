#include "Project3.h"

namespace Project3
{
	class LempelZivEncoder : public ICompressionAlg
	{
	public:
		LempelZivEncoder(std::int32_t windowSize);
		virtual ~LempelZivEncoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);

	private:
		std::int32_t windowSize;
	};

	class LempelZivDecoder : public ICompressionAlg
	{
	public:
		LempelZivDecoder(std::int32_t windowSize);
		virtual ~LempelZivDecoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);

	private:
		std::int32_t windowSize;
	};
}