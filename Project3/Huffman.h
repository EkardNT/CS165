#include "Project3.h"

namespace Project3
{
	class HuffmanEncoder : public ICompressionAlg
	{
	public:
		HuffmanEncoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~HuffmanEncoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();
	};

	class HuffmanDecoder : public ICompressionAlg
	{
	public:
		HuffmanDecoder(std::shared_ptr<std::ostream> outputStream);
		virtual ~HuffmanDecoder();

		virtual void Process(std::uint8_t byte);
		virtual void Finish();
	};
}