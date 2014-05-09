using System;
using System.Linq;
using System.Numerics;

namespace PrimalityTest
{
	public static class Program
	{
		private const int NumRounds = 200;

		private static void Main(string[] args)
		{
			Random rand = new Random();
			while (true)
			{
				Console.Write("Enter a decimal number to test for primality: ");
				var input = new string(Console.ReadLine().Trim().Where(char.IsDigit).ToArray());
				BigInteger number;
				if (!BigInteger.TryParse(input, out number))
				{
					Console.WriteLine("Invalid input.");
					continue;
				}
				if (IsPrime(number, rand))
					Console.WriteLine("The number {0} is prime.", number);
				else
				{
					Console.WriteLine("The number {0} is not prime, calculating next prime...", number);
					do
					{
						number++;
					} while (!IsPrime(number, rand));
					Console.WriteLine("The next highest prime is {0}.", number);
				}
			}
		}

		private static bool IsPrime(BigInteger n, Random rand)
		{
			if (n.IsEven)
				return false;
			for(int i = 0; i < NumRounds; i++)
			{
				var b = rand.BigRandom(n - 2) + 1;
				if (BigInteger.GreatestCommonDivisor(b, n) == 1 && (Jacobi(b, n) - UglyTerm(b, n)) % n == 0)
					continue;
				return false;
			}
			return true;
		}

		private static BigInteger BigRandom(this Random rand, BigInteger max)
		{
			var bytes = max.ToByteArray();
			var maxByte = bytes[bytes.Length - 1];
			rand.NextBytes(bytes);
			bytes[bytes.Length - 1] = (byte)rand.Next(maxByte);
			return new BigInteger(bytes);
		}

		private static BigInteger Jacobi(BigInteger x, BigInteger y)
		{
			if (x == 1)
				return 1;
			if (x.IsEven)
				return Jacobi(x / 2, y) * ((((y * y - 1) / 8) % 2 == 0) ? 1 : -1);
			return Jacobi(y % x, x) * ((((x - 1) * (y - 1) / 4) % 2 == 0) ? 1 : -1);
		}

		private static BigInteger UglyTerm(BigInteger b, BigInteger n)
		{
			var x = (n - 1) / 2;
			var y = b;
			var a = new BigInteger(1);
			while(x > 0)
			{
				if (!x.IsEven)
					a = (a * y) % n;
				y = (y * y) % n;
				x /= 2;
			}
			return a;
		}
	}
}