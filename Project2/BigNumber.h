#pragma once
#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <iostream>
#include <string>
#include <random>
#include "Digits.h"

class BigNumber
{
public:
	enum class Sign 
	{
		Positive = 1,
		Negative = -1
	};
	enum class Comparison
	{
		Greater,
		Lesser,
		Equal
	};

	// Constructors
	BigNumber(const std::string & base10);
	BigNumber(std::int32_t value);
	BigNumber(std::uint32_t value);
	BigNumber(const BigNumber & copy);

	// Accessors
	bool IsZero() const;
	bool IsEven() const;
	
	// Mutates the current value into a random value in
	// the range [0, value - 1]. Throws if current value
	// is negative or zero.
	void Randomize(std::default_random_engine & e);

	// Algebraic operations
	// Returns a + b
	static BigNumber Add(const BigNumber & a, const BigNumber & b); 
	// Returns a - b
	static BigNumber Subtract(const BigNumber & a, const BigNumber & b);
	// Returns a * b
	static BigNumber Multiply(const BigNumber & a, const BigNumber & b);
	// Computes:
	//	q = n / d (quotient)
	//	r = n % d (remainder)
	static void DivideWithRemainder(const BigNumber & n, const BigNumber & d, BigNumber & q, BigNumber & r);
	// Returns -a
	static BigNumber Negate(const BigNumber & a);
	// Returns the greatest common denominator of a and b.
	static BigNumber GreatestCommonDenominator(const BigNumber & a, const BigNumber & b);
	// Compares a to b.
	static Comparison Compare(const BigNumber & a, const BigNumber & b);

	// Logical operators
	bool operator ==(const BigNumber & other) const;
	bool operator !=(const BigNumber & other) const;
	bool operator < (const BigNumber & other) const;
	bool operator <=(const BigNumber & other) const;
	bool operator >=(const BigNumber & other) const;
	bool operator > (const BigNumber & other) const;
	// Arithmetic operators
	BigNumber operator +(const BigNumber & other) const;
	BigNumber operator -(const BigNumber & other) const;
	BigNumber operator *(const BigNumber & other) const;
	BigNumber operator /(const BigNumber & other) const;
	BigNumber operator %(const BigNumber & other) const;
	BigNumber operator -() const;
	void operator ++();
	void operator ++(std::int32_t amount);
	void operator --();
	void operator --(std::int32_t amount);
	// Conversions
	friend std::ostream & operator<<(std::ostream & os, const BigNumber & value);
private:
	Sign sign;
	Digits<std::uint32_t> digits;
};

#endif