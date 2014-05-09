#include "BigNumber.h"

BigNumber::BigNumber(const std::string & base10)
{
	// TODO
}

BigNumber::BigNumber(std::int32_t value)
{
	// TODO
}

BigNumber::BigNumber(std::uint32_t value)
{
	// TODO
}

BigNumber::BigNumber(const BigNumber & copy)
{
	// TODO
}

bool BigNumber::IsZero() const
{
	// TODO
}

bool BigNumber::IsEven() const
{
	// TODO
}

void Randomize(std::default_random_engine & e)
{
	// TODO
}

BigNumber BigNumber::Add(const BigNumber & a, const BigNumber & b)
{
	// TODO
}

BigNumber BigNumber::Subtract(const BigNumber & a, const BigNumber & b)
{
	// TODO
}

BigNumber BigNumber::Multiply(const BigNumber & a, const BigNumber & b)
{
	// TODO
}

void BigNumber::DivideWithRemainder(const BigNumber & n, const BigNumber & d, BigNumber & q, BigNumber & r)
{
	// TODO
}

BigNumber BigNumber::Negate(const BigNumber & a)
{
	// TODO
}

BigNumber BigNumber::GreatestCommonDenominator(const BigNumber & a, const BigNumber & b)
{
	// TODO
}

BigNumber::Comparison Compare(const BigNumber & a, const BigNumber & b)
{
	// TODO
}

bool BigNumber::operator==(const BigNumber & other) const
{
	// TODO
}

bool BigNumber::operator!=(const BigNumber & other) const
{
	// TODO
}

bool BigNumber::operator<(const BigNumber & other) const
{
	// TODO
}

bool BigNumber::operator<=(const BigNumber & other) const
{
	// TODO
}

bool BigNumber::operator>(const BigNumber & other) const
{
	// TODO
}

bool BigNumber::operator>=(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator+(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator-(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator*(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator/(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator%(const BigNumber & other) const
{
	// TODO
}

BigNumber BigNumber::operator-() const
{
	// TODO
}

void BigNumber::operator++()
{
	// TODO
}

void BigNumber::operator++(std::int32_t amount)
{
	// TODO
}

void BigNumber::operator--()
{
	// TODO
}

void BigNumber::operator--(std::int32_t amount)
{
	// TODO
}

std::ostream & operator<<(std::ostream & os, const BigNumber & value)
{
	// TODO
}