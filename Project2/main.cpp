#include "BigIntegerLibrary.hh"
#include "BigNumber.h"
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cstdint>
#include <ctime>
#include <random>

const int NumRounds = 200;

bool TryProcessInput(const std::string & input, std::string & output);
bool IsPrime(const BigNumber & value);

int main()
{
	BigNumber a(10);
	std::cout << "BigNumber a(10): " << a << std::endl;
	BigNumber b(-10);
	std::cout << "BigNumber b(-10): " << b << std::endl;
	BigNumber c("4294967298");
	std::cout << "BigNumber c(\"some string\"): " << c << std::endl;
	BigNumber d(b);
	std::cout << "BigNumber d(b): " << d << std::endl;
	BigNumber shouldBeEven("12349234592345612398523149712538");
	std::cout << shouldBeEven << " should be even, it is: " << (shouldBeEven.IsEven() ? "true" : "false") << std::endl;
	BigNumber shouldBeOdd("123096578459238452348975624390785");
	std::cout << shouldBeOdd << " should be odd, is it: " << (shouldBeOdd.IsEven() ? "false" : "true") << std::endl;
	BigNumber pos0(+0), neg0(-0);
	std::cout << pos0 << " should be zero, is it: " << (pos0.IsZero() ? "true" : "false") << std::endl;
	std::cout << neg0 << " should be zero, is it: " << (neg0.IsZero() ? "true" : "false") << std::endl;
	std::default_random_engine e(std::time(nullptr));
	BigNumber randomizer("76923752934561239857490786123951239876234");
	std::cout << randomizer << " randomized to ";
	randomizer.Randomize(e);
	std::cout << randomizer << std::endl;
	std::cout << "Expected a > b, is it: " << (a > b ? "true" : "false") << std::endl;
	std::cout << "Expected b < a, is it: " << (b < a ? "true" : "false") << std::endl;
	std::cout << "Expected a == a, is it: " << (a == a ? "true" : "false") << std::endl;
	std::cout << "Expected b == b, is it: " << (b == b ? "true" : "false") << std::endl;
	char wait;
	std::cin >> wait;
	return EXIT_SUCCESS;

	while (true)
	{
		std::string input;
		std::cout << "Enter a base-10 number to test for primality, or type 'exit' to stop." << std::endl;
		std::getline(std::cin, input);

		if (input == "exit")
			return EXIT_SUCCESS;

		std::string output;
		if (!TryProcessInput(input, output))
		{
			std::cout << "Bad input." << std::endl << std::endl;
			continue;
		}

		BigNumber num(input);
		if (IsPrime(num))
			std::cout << "The number " << num << " is prime." << std::endl << std::endl;
		else
		{
			std::cout << "The number " << num << " is not prime, calculating next prime..." << std::endl;
			do
			{
				num = num + 1;
			} while (!IsPrime(num));
			std::cout << "The next highest prime is " << num << std::endl << std::endl;
		}
	}
}

bool TryProcessInput(const std::string & input, std::string & output)
{
	std::stringstream stream;
	for (auto iter = input.cbegin(); iter != input.cend(); iter++)
	{
		if (*iter >= '0' && *iter <= '9')
			stream << *iter;
	}
	output = stream.str();
	return output.size() > 0;
}

BigNumber Jacobi(const BigNumber & x, const BigNumber & y)
{
	if (x == 1)
		return 1;
	if (x.IsEven())
		return Jacobi(x / 2, y) * ((((y * y - 1) / 8) % 2 == 0) ? 1 : -1);
	return Jacobi(y % x, x) * ((((x - 1) * (y - 1) / 4) % 2 == 0) ? 1 : -1);
}

BigNumber UglyTerm(const BigNumber & b, const BigNumber & n)
{
	BigNumber
		x = (n - 1) / 2,
		y = b,
		a = 1;
	while (x > 0)
	{
		if (x % 2 == 1)
			a = (a * y) % n;
		y = (y * y) % n;
		x = x / 2;
	}
	return a;
}

bool IsPrime(const BigNumber & n)
{
	static std::default_random_engine e;
	if (n.IsEven() || n < 3)
		return false;
	for (int i = 0; i < NumRounds; i++)
	{
		auto b = n - 2;
		b.Randomize(e);
		b = b + 1;
		if (BigNumber::GreatestCommonDenominator(b, n) == 1 && (Jacobi(b, n) - UglyTerm(b, n)) % n == 0)
			continue;
		else
			return false;
	}
	return true;
}