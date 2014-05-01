#include "BigNum.h"
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cstdint>

bool ContainsOnlyDigits(const std::string & input);
std::string Base10ToBase2(std::string b10);
void Base10DivideBy2(const std::string & dividend, std::string & quotient, std::uint8_t & remainder);

using Project2::BigNum;

int main()
{
	do
	{
		std::string input;
		std::cout << "Enter a base-10 number to test for primality, or type 'exit' to stop." << std::endl;
		std::getline(std::cin, input);

		if (input == "exit")
			return EXIT_SUCCESS;

		if (input.size() <= 0)
		{
			std::cout << "Bad input, no characters found." << std::endl << std::endl;
			continue;
		}
		else if (!ContainsOnlyDigits(input))
		{
			std::cout << "Bad input, non-digit character found." << std::endl << std::endl;
			continue;
		}
		BigNum num = BigNum(Base10ToBase2(input));
		std::cout << "num.ToString() == " << num.ToString() << std::endl;
		std::cout << "num.Length() == " << num.Length() << std::endl;

		BigNum _123(Base10ToBase2("18446744082299486211")); // [1][2][3]	
		BigNum _321(Base10ToBase2("55340232229718589441")); // [3][2][1]
		BigNum _999(Base10ToBase2("166020696702040670217")); // [9][9][9]
		BigNum _111(Base10ToBase2("18446744078004518913")); // [1][1][1]
		BigNum _M(Base10ToBase2("4294967295")); // [2^32-1]
		BigNum _1(Base10ToBase2("1")); // [1]

		BigNum 
			a = _M,
			b = _1;

		std::cout << a.ToString() << " + " << b.ToString() << " == " << BigNum::Add(a, b).ToString() << std::endl;
	} while (true);
}

bool ContainsOnlyDigits(const std::string & input)
{
	for (auto iter = input.cbegin(); iter != input.cend(); iter++)
		if (*iter < '0' || *iter > '9')
			return false;
	return true;
}

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
	std::stack<std::uint8_t> resultBuilder;
	std::string quotient;
	std::uint8_t remainder;
	Base10DivideBy2(b10, quotient, remainder);
	while (!Base10IsZero(b10))
	{
		resultBuilder.push(remainder);
		b10 = quotient;
		Base10DivideBy2(b10, quotient, remainder);
		b10 = quotient;
	}
	resultBuilder.push(remainder);
	std::stringstream stream;
	while (!resultBuilder.empty())
	{
		stream << (char)(resultBuilder.top() + '0');
		resultBuilder.pop();
	}
	return stream.str();
}

