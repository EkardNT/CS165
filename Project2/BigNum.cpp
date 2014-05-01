#include "BigNum.h"
#include <sstream>
#include <stack>

namespace Project2
{
	typedef std::uint64_t ulong;
	typedef std::uint32_t uint;
	typedef std::uint8_t ubyte;
	using std::vector;
	using std::string;

	// All powers of 2^n from n = 0 to 31.
	uint TwoToTheN[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648 };

	BigNum::BigNum(const string & base2Number)
	{
		// Group every 32 binary digits into one 32-bit digit.
		std::stack<ubyte> binaryDigits;
		for (auto iter = base2Number.cbegin(); iter != base2Number.cend(); iter++)
			binaryDigits.push(*iter - '0');
		uint digitValue = 0;
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
		if (digitValue != 0 || digits.size() == 0)
			digits.push_back(digitValue);
	}

	BigNum::BigNum(const vector<uint> digits)
		: digits(digits)
	{}

	string BigNum::ToString() const
	{
		if (digits.size() <= 0)
			return "[0]";
		std::stringstream stream;
		for (auto iter = digits.crbegin(); iter != digits.crend(); iter++)
			stream << "[" << *iter << "]";
		return stream.str();
	}

	uint BigNum::Length() const
	{
		return digits.size();
	}

	uint BigNum::operator[](std::uint32_t index) const
	{
		return index >= digits.size() ? 0 : digits[index];
	}
	
	void BigNum::GetCounts(const BigNum & a, const BigNum & b, uint & smallerCount, uint & largerCount)
	{
		if (a.digits.size() > b.digits.size())
		{
			smallerCount = b.digits.size();
			largerCount = a.digits.size();
		}
		else
		{
			smallerCount = a.digits.size();
			largerCount = b.digits.size();
		}
	}

	BigNum BigNum::Add(const BigNum & a, const BigNum & b)
	{
		uint smallerCount = 0, largerCount = 0;
		BigNum::GetCounts(a, b, smallerCount, largerCount);

		vector<uint> sumDigits;
		sumDigits.reserve(largerCount + 1);

		ulong carry = 0, sum = 0;
		for (uint i = 0; i < largerCount; i++)
		{
			sum = (ulong)a[i] + (ulong)b[i] + carry;
			carry = sum >> 32;
			sumDigits.push_back((uint)sum);
		}
		if (carry > 0)
			sumDigits.push_back(carry);

		return BigNum(sumDigits);
	}
}