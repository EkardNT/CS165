#include "BigNumber.h"
#include <stack>
#include <sstream>

// Holds the results of i / 2, i in [0, 19].
std::uint8_t divTable[] = { '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '8', '8', '9', '9' };
// Holds the results of i % 2, i in [0, 19].
std::uint8_t remTable[] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };

void Base10DivideBy2(const std::string & dividend, std::string & quotient, std::uint8_t & remainder)
{
	std::vector<std::uint8_t> resultBuilder;
	bool carry = false;
	//int tableIndex = 0;
	int digitValue = 0;
	for (unsigned int i = 0; i < dividend.size(); i++)
	{
		digitValue = dividend[i] - '0';
		//tableIndex = dividend[i] - '0';
		if (carry)
		{
			//tableIndex += 10;
			digitValue += 10;
		}
		resultBuilder.push_back(digitValue / 2);
		//resultBuilder.push_back(divTable[tableIndex]);
		carry = digitValue % 2 == 1;
		//carry = remTable[tableIndex] == 1;
	}
	remainder = (digitValue + (carry ? 10 : 0)) % 2;
	//remainder = remTable[tableIndex + (carry ? 10 : 0)];
	std::stringstream stream;
	for (auto iter = resultBuilder.cbegin(); iter != resultBuilder.cend(); iter++)
		stream << (char)(*iter + '0');
	quotient = stream.str();
}

bool Base10IsZero(const std::string & b10)
{
	for (auto iter = b10.cbegin(); iter != b10.cend(); iter++)
		if (*iter != '0')
			return false;
	return true;
}

std::string Base10ToBase2(std::string b10)
{
	std::vector<std::uint8_t> resultBuilder;
	std::string quotient;
	std::uint8_t remainder;
	Base10DivideBy2(b10, quotient, remainder);
	while (!Base10IsZero(b10))
	{
		resultBuilder.push_back(remainder);
		b10 = quotient;
		Base10DivideBy2(b10, quotient, remainder);
		b10 = quotient;
	}
	resultBuilder.push_back(remainder);
	std::stringstream stream;
	for (auto iter = resultBuilder.cbegin(); iter != resultBuilder.cend(); iter++)
		stream << (char)(*iter + '0');
	return stream.str();
}

BigNumber::BigNumber(const std::string & base10)
{
	auto base2 = Base10ToBase2(base10);
	std::uint32_t digitValue = 0;
	std::uint32_t i = 0;
	for (; i < base2.size(); i++)
	{
		std::uint32_t shiftAmount = i % 32;
		digitValue += (base2[i] - '0') << shiftAmount;
		if (shiftAmount == 31)
		{
			digits.setElement(i / 32, digitValue);
			digitValue = 0;
		}
	}
	digits.setElement(i / 32, digitValue);
	sign = Sign::Positive;
}

BigNumber::BigNumber(std::int32_t value)
: sign(value < 0 ? BigNumber::Sign::Negative : BigNumber::Sign::Positive)
{
	digits.setElement(0, value < 0 ? (std::uint32_t)(-value) : (std::uint32_t)value);
}

BigNumber::BigNumber(std::uint32_t value)
: sign(BigNumber::Sign::Positive)
{
	digits.setElement(0, value);
}

BigNumber::BigNumber(const BigNumber & copy)
: sign(copy.sign), digits(copy.digits)
{
}

bool BigNumber::IsZero() const
{
	return digits.getLength() == 1 && digits.getElement(0) == 0;
}

bool BigNumber::IsEven() const
{
	return digits.getElement(0) % 2 == 0;
}

BigNumber::Sign BigNumber::GetSign() const
{
	return sign;
}

const Digits<std::uint32_t> const & const BigNumber::GetDigits() const
{
	return digits;
}

void BigNumber::Randomize(std::default_random_engine & e)
{
	if (digits.getLength() == 0)
		throw "BigNumber::Randomize() - invalid digits length.";
	if (sign == BigNumber::Sign::Negative)
		throw "BigNumber::Randomize() - cannot call Randomize() on a negative BigNumber.";
	static std::uniform_int_distribution<std::uint32_t> d;
	for (std::uint32_t i = 0; i < digits.getLength() - 1; i++)
		digits.setElement(i, d(e));
	digits.setElement(digits.getLength() - 1, d(e) % digits.getElement(digits.getLength() - 1));
}

BigNumber::Comparison CompareUnsigned(const Digits<std::uint32_t> & a, const Digits<std::uint32_t> & b)
{
	if (a.getLength() > b.getLength())
		return BigNumber::Comparison::Greater;
	if (a.getLength() < b.getLength())
		return BigNumber::Comparison::Lesser;
	for (int i = a.getLength() - 1; i >= 0; i--)
	{
		std::uint32_t
			aElem = a.getElement(i),
			bElem = b.getElement(i);
		if (aElem > bElem)
			return BigNumber::Comparison::Greater;
		if (aElem < bElem)
			return BigNumber::Comparison::Lesser;
	}
	return BigNumber::Comparison::Equal;
}

BigNumber::Comparison BigNumber::Compare(const BigNumber & a, const BigNumber & b)
{
	if (a.IsZero() && b.IsZero())
		return Comparison::Equal;
	if (a.GetSign() == Sign::Negative
		&& b.GetSign() == Sign::Positive)
		return Comparison::Lesser;
	if (a.GetSign() == Sign::Positive
		&& b.GetSign() == Sign::Negative)
		return Comparison::Greater;
	if (a.GetSign() == Sign::Positive)
		return CompareUnsigned(a.GetDigits(), b.GetDigits());
	return CompareUnsigned(b.GetDigits(), a.GetDigits());
}

// a - b, where a must be >= b.
void SubtractUnsigned(const Digits<std::uint32_t> & a, const Digits<std::uint32_t> & b, Digits<std::uint32_t> & difference)
{
	auto comparison = CompareUnsigned(a, b);

	if (comparison == BigNumber::Comparison::Equal)
	{
		difference = BigNumber(0).GetDigits();
		return;
	}
	if (comparison == BigNumber::Comparison::Lesser)
	{
		throw "global::SubtractUnsigned() - a was less than b.";
	}

	if (b.isZero())
	{
		difference = a;
		return;
	}

	// Most of the subtraction code slightly modified from 
	// https://mattmccutchen.net/bigint/, which is in the public domain.
	bool borrowIn, borrowOut;
	std::uint32_t temp;
	std::uint32_t i;
	for (i = 0, borrowIn = false; i < b.getLength(); i++) {
		temp = a.getElement(i) - b.getElement(i);
		borrowOut = (temp > a.getElement(i));
		if (borrowIn) {
			borrowOut |= (temp == 0);
			temp--;
		}
		difference.setElement(i, temp);
		borrowIn = borrowOut;
	}
	for (; i < a.getLength() && borrowIn; i++) {
		borrowIn = (a.getElement(i) == 0);
		difference.setElement(i, a.getElement(i) - 1);
	}
	if (borrowIn)
		throw "global::SubtractUnsigned - negative result in unsigned calculation.";
	else
		// Copy over the rest of the blocks
		for (; i < a.getLength(); i++)
			difference.setElement(i, a.getElement(i));
	difference.reduceLength();
}

// a + b
void AddUnsigned(const Digits<std::uint32_t> & a, const Digits<std::uint32_t> & b, Digits<std::uint32_t> & sum)
{
	// 0 + b == b
	if (a.isZero())
		sum = b;
	// a + 0 == a
	else if (b.isZero())
		sum = a;
	else
	{
		std::uint32_t carry = 0;
		const Digits<std::uint32_t> & longer = (a.getLength() >= b.getLength() ? a : b);
		const Digits<std::uint32_t> & shorter = (a.getLength() >= b.getLength() ? b : a);
		for (std::uint32_t i = 0; i < shorter.getLength(); i++)
		{
			std::uint64_t iSum = (std::uint64_t)longer.getElement(i) + (std::uint64_t)shorter.getElement(i) + carry;
			sum.setElement(i, (std::uint32_t)iSum);
			carry = (std::uint32_t)(iSum >> 32);
		}
			sum.setElement(shorter.getLength(), carry + longer.getElement(shorter.getLength()));
			for (std::uint32_t i = shorter.getLength() + 1; i < longer.getLength(); i++)
				sum.setElement(i, longer.getElement(i));
	}
}

BigNumber BigNumber::Subtract(const BigNumber & a, const BigNumber & b)
{
	// 0 - b == -b
	if (a.IsZero())
		return Negate(b);
	// a - 0 == a
	if (b.IsZero())
		return a;
	if (a.sign != b.sign)
	{
		BigNumber diff(0);
		diff.sign = a.sign;
		AddUnsigned(a.digits, b.digits, diff.digits);
		return diff;
	}
	switch (CompareUnsigned(a.digits, b.digits))
	{
	case Comparison::Equal:
		return BigNumber(0);
	case Comparison::Lesser:
	{
		BigNumber diff(0);
		diff.sign = b.sign == Sign::Positive ? Sign::Negative : Sign::Positive;
		SubtractUnsigned(b.digits, a.digits, diff.digits);
		return diff;
	}
	case Comparison::Greater:
	{
		BigNumber diff(0);
		diff.sign = a.sign;
		SubtractUnsigned(a.digits, b.digits, diff.digits);
		return diff;
	}
	}
}

BigNumber BigNumber::Add(const BigNumber & a, const BigNumber & b)
{
	// Shortcut if we are adding 0 to a number.
	if (a.IsZero())
		return b;
	if (b.IsZero())
		return a;
	// When adding two numbers, if the signs are the same
	// then the result will have the same sign and the
	// magnitude will be the two unsigned magnitudes
	// added together.
	if (a.sign == b.sign)
	{
		BigNumber sum(0);
		sum.sign = a.sign;
		AddUnsigned(a.digits, b.digits, sum.digits);
		return sum;
	}
	switch (CompareUnsigned(a.digits, b.digits))
	{
	case Comparison::Equal:
		// x + (-x) == 0
		return BigNumber(0);
	case Comparison::Lesser:
	{
		BigNumber sum(0);
		sum.sign = b.sign;
		SubtractUnsigned(b.digits, a.digits, sum.digits);
		return sum;
	}
	case Comparison::Greater:
	{
		BigNumber sum(0);
		sum.sign = a.sign;
		SubtractUnsigned(a.digits, b.digits, sum.digits);
		return sum;
	}
	}
}

BigNumber BigNumber::Multiply(const BigNumber & a, const BigNumber & b)
{
	// TODO
	return 0;
}

void BigNumber::DivideWithRemainder(const BigNumber & n, const BigNumber & d, BigNumber & q, BigNumber & r)
{
	// TODO
}

BigNumber BigNumber::Negate(const BigNumber & a)
{
	if (a.IsZero())
		return BigNumber(0);
	BigNumber copy(a);
	copy.sign = copy.sign == Sign::Positive ? Sign::Negative : Sign::Positive;
	return copy;
}

BigNumber BigNumber::GreatestCommonDenominator(const BigNumber & a, const BigNumber & b)
{
	// TODO
	return 0;
}

bool BigNumber::operator==(const BigNumber & other) const
{
	return Compare(*this, other) == Comparison::Equal;
}

bool BigNumber::operator!=(const BigNumber & other) const
{
	return Compare(*this, other) != Comparison::Equal;
}

bool BigNumber::operator<(const BigNumber & other) const
{
	return Compare(*this, other) == Comparison::Lesser;
}

bool BigNumber::operator<=(const BigNumber & other) const
{
	return Compare(*this, other) != Comparison::Greater;
}

bool BigNumber::operator>(const BigNumber & other) const
{
	return Compare(*this, other) == Comparison::Greater;
}

bool BigNumber::operator>=(const BigNumber & other) const
{
	return Compare(*this, other) != Comparison::Lesser;
}

BigNumber BigNumber::operator+(const BigNumber & other) const
{
	return BigNumber::Add(*this, other);
}

BigNumber BigNumber::operator-(const BigNumber & other) const
{
	return BigNumber::Subtract(*this, other);
}

BigNumber BigNumber::operator*(const BigNumber & other) const
{
	return BigNumber::Multiply(*this, other);
}

BigNumber BigNumber::operator/(const BigNumber & other) const
{
	BigNumber quotient(0), remainder(0);
	BigNumber::DivideWithRemainder(*this, other, quotient, remainder);
	return quotient;
}

BigNumber BigNumber::operator%(const BigNumber & other) const
{
	BigNumber quotient(0), remainder(0);
	BigNumber::DivideWithRemainder(*this, other, quotient, remainder);
	return remainder;
}

BigNumber BigNumber::operator-() const
{
	return BigNumber::Negate(*this);
}

std::ostream & operator<<(std::ostream & os, const BigNumber & value)
{
	if (value.sign == BigNumber::Sign::Negative)
		os << "-";
	for (int i = value.digits.getLength() - 1; i >= 0; i--)
		os << "[" << value.digits.getElement(i) << "]";
	return os;
}