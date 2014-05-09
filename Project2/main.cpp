#include "BigIntegerLibrary.hh"
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cstdint>
#include <ctime>
#include <random>

const int NumRounds = 200;

bool TryProcessInput(const std::string & input, std::string & output);
bool IsPrime(const BigInteger & value);

int main()
{
	srand(time(nullptr));

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

		BigInteger num = stringToBigInteger(input);
		if (IsPrime(num))
			std::cout << "The number " << num << " is prime." << std::endl << std::endl;
		else
		{
			std::cout << "The number " << num << " is not prime, calculating next prime..." << std::endl;
			do
			{
				num++;
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

BigInteger Jacobi(const BigInteger & x, const BigInteger & y)
{
	if (x == 1)
		return 1;
	if (x.isEven())
		return Jacobi(x / 2, y) * ((((y * y - 1) / 8) % 2 == 0) ? 1 : -1);
	return Jacobi(y % x, x) * ((((x - 1) * (y - 1) / 4) % 2 == 0) ? 1 : -1);
}

BigInteger UglyTerm(const BigInteger & b, const BigInteger & n)
{
	BigInteger
		x = (n - 1) / 2,
		y = b,
		a = 1;
	while (x > 0)
	{
		if (x % 2 == 1)
			a = (a * y) % n;
		y = (y * y) % n;
		x /= 2;
	}
	return a;
}

bool IsPrime(const BigInteger & n)
{
	static std::default_random_engine e;
	if (n.isEven() || n < 3)
		return false;
	for (int i = 0; i < NumRounds; i++)
	{
		auto b = n - 2;
		b.randomize(e);
		b++;
		if (gcd(b.getMagnitude(), n.getMagnitude()) == 1 && (Jacobi(b, n) - UglyTerm(b, n)) % n == 0)
			continue;
		else
			return false;
	}
	return true;
}