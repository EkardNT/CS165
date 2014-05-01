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
		BigNum(const std::vector<std::uint32_t> digits);

		std::string ToString() const;
		std::uint32_t Length() const;		
		std::uint32_t operator[](std::uint32_t index) const;

		static BigNum Add(const BigNum & a, const BigNum & b);
		
		
	private:
		// Digits in base 2^32, where the number
		// at index 0 is the least significant digit.
		std::vector<std::uint32_t> digits;

		static void GetCounts(const BigNum & a, const BigNum & b, std::uint32_t & smallerCount, std::uint32_t & largerCount);
	};
}