#pragma once
#ifndef PROJECT3_H
#define PROJECT3_H

#include <iostream>
#include <cstdint>
#include <memory>

namespace Project3
{
	const std::uint8_t HuffmanIdentifier = 13;
	const std::uint8_t LempelZiv1Identifier = 17;
	const std::uint8_t LempelZiv2Identifier = 19;
	const std::uint8_t PassthroughIdentifier = 23;

	// Encodes or decodes input one byte at a time.
	class ICompressionAlg
	{
	public:
		ICompressionAlg(std::shared_ptr<std::ostream> outputStream) : outputStream(outputStream) {}
		virtual ~ICompressionAlg() {}
		virtual void Process(std::uint8_t byte) = 0;
		virtual void Finish() = 0;

	protected:
		void Write(std::uint8_t byte)
		{
			char val = (char)byte;
			outputStream->write(&val, 1);
		}

	private:
		std::shared_ptr<std::ostream> outputStream;
	};
}

#endif