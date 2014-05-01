#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Project2
{
	class BigNum
	{
	public:
		BigNum(const std::string & base2Number);

		std::string ToString() const;

	private:
		// Digits in base 2^32, where the number
		// at index 0 is the least significant digit.
		std::vector<std::uint32_t> digits;
	};
}