
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cstdint>


std::string Base10ToBase2(std::string b10);
void Base10DivideBy2(const std::string & dividend, std::string & quotient, std::uint8_t & remainder);

int main()
{
	std::cout << "Enter a base-10 number to test for primality, or type 'exit' to stop." << std::endl;
	std::string input;
	std::getline(std::cin, input);
	while (input != "exit")
	{
		std::string quotient;
		std::uint8_t remainder;
		Base10DivideBy2(input, quotient, remainder);
		std::cout << "Quotient: " << quotient << std::endl << "Remainder: " << (char)(remainder + '0') << std::endl;
		std::cout << input << " in base 2 is " << Base10ToBase2(input) << std::endl;
		std::getline(std::cin, input);
	}
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

