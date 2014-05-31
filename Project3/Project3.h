#pragma once
#ifndef PROJECT3_H
#define PROJECT3_H

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>

namespace Project3
{
	const std::uint8_t HuffmanIdentifier = 13;
	const std::uint8_t LempelZiv1Identifier = 17;
	const std::uint8_t LempelZiv2Identifier = 19;
	const std::uint8_t PassthroughIdentifier = 23;

	class ICompressionAlg
	{
	public:
		ICompressionAlg() {}
		virtual ~ICompressionAlg() {}
		virtual void Execute(std::ostream & outputStream, std::shared_ptr<std::vector<std::uint8_t>> inputData) = 0;
	};
}

#endif