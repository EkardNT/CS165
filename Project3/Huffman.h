#include "Project3.h"

namespace Project3
{
	class HuffmanEncoder : public ICompressionAlg
	{
	public:
		HuffmanEncoder();
		virtual ~HuffmanEncoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);
	};

	class HuffmanDecoder : public ICompressionAlg
	{
	public:
		HuffmanDecoder();
		virtual ~HuffmanDecoder();

		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData);
	};
}