#include "BigNum.h"
#include <sstream>
#include <stack>

namespace Project2
{
	std::uint32_t TwoToTheN[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648 };

	BigNum::BigNum(const std::string & base2Number)
	{
		std::stack<std::uint8_t> binaryDigits;
		for (auto iter = base2Number.cbegin(); iter != base2Number.cend(); iter++)
			binaryDigits.push(*iter - '0');
		std::uint32_t digitValue = 0;
		int exponent = 0;
		while (!binaryDigits.empty())
		{
			digitValue += binaryDigits.top() * TwoToTheN[exponent];
			binaryDigits.pop();
			if (++exponent == 32)
			{
				exponent = 0;
				digits.push_back(digitValue);
				digitValue = 0;
			}
		}
		digits.push_back(digitValue);
	}

	std::string BigNum::ToString() const
	{
		if (digits.size() <= 0)
			return "[0]";
		std::stringstream stream;
		for (auto iter = digits.crbegin(); iter != digits.crend(); iter++)
		{
			stream << "[" << *iter << "]";
		}
		return stream.str();
	}
}