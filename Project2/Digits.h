#pragma once
#ifndef NUMERIC_H
#define NUMERIC_H

#include <array>
#include <cstdint>

// Manages an expandable list of digits with index 0 
// being the least significant digit and padded with 0s
// to infinity.
template<class Element = int>
class Digits
{
public:
	static const std::uint32_t MaxLength = 128;

	Digits() : length(0) {}
	Digits(const Digits & copy) : length(copy.length)
	{
		std::copy(copy.digits.cbegin(), copy.digits.cend(), digits.begin());
	}

protected:
	std::uint8_t getLength() const
	{
		return length;
	}
	Element getElement(std::uint32_t index) const
	{
		return index >= length ? 0 : digits[index];
	}
	void setElement(std::uint32_t index, Element element)
	{
		if (index >= MaxLength)
			throw "Digits::setElement() - 'index' too large.";
		if (index >= length)
			for (std::uint32_t i = length; i < index; i++)
				digits[i] = 0;
		digits[index] = 0;
	}
	bool operator==(const Digits & other) const
	{
		if (length != other.length)
			return false;
		for (std::uint32_t i = 0; i < length; i++)
			if (digits[i] != other.digits[i])
				return false;
		return true;
	}
	bool operator!=(const Digits & other) const
	{
		return !(operator==(other));
	}
private:
	std::array<Element, MaxLength> digits;
	std::uint32_t length;
};

#endif