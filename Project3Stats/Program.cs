using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;

namespace Project3Stats
{
	public static class Program
	{
		public static void Main()
		{
			var rand = new Random();
			var tasks = new List<Task>();

			var huffmanEncodingTicks = new long[7];
			var huffmanDecodingTicks = new long[7];
			var lz1EncodingTicks = new long[7];
			var lz1DecodingTicks = new long[7];
			var lz2EncodingTicks = new long[7];
			var lz2DecodingTicks = new long[7];

			for (int size = 10; size <= 16; size++)
			{
				tasks.Add(RunTest(size, huffmanEncodingTicks, huffmanDecodingTicks, "HUFF"));
				tasks.Add(RunTest(size, lz1EncodingTicks, lz1DecodingTicks, "LZ1"));
				tasks.Add(RunTest(size, lz2EncodingTicks, lz2DecodingTicks, "LZ2"));
			}

			Task.WaitAll(tasks.ToArray());

			WriteResults("HUFF", huffmanEncodingTicks, huffmanDecodingTicks);
			WriteResults("LZ1", lz1EncodingTicks, lz1DecodingTicks);
			WriteResults("LZ2", lz2EncodingTicks, lz2DecodingTicks);

			Console.Write("Done");
		}

		public static Task RunTest(int size, long[] encodingTicks, long[] decodingTicks, string name)
		{
			int finalSize = 1 << size;
			string inputFile = "size_" + finalSize + ".test";

			return Task.Run(() =>
			{
				Console.WriteLine(name + " started");

				var proc = new Process
				{
					StartInfo = new ProcessStartInfo
					{
						FileName = "Project3",
						Arguments = name + " " + inputFile,
						RedirectStandardOutput = true,
						UseShellExecute = false
					}
				};
				var timer = Stopwatch.StartNew();
				proc.Start();
				var encodeResults = DrainStream(proc.StandardOutput);
				proc.WaitForExit();
				encodingTicks[size - 10] = timer.ElapsedMilliseconds;

				Console.WriteLine(name + " encoded");
				string encodedFilePath = name + "_" + finalSize + "_encoded.txt";

				using (var file = File.Create(encodedFilePath))
				{
					file.Write(encodeResults, 0, encodeResults.Length);
				}

				proc = new Process
				{
					StartInfo = new ProcessStartInfo
					{
						FileName = "Project3",
						Arguments = "EXPAND " + encodedFilePath,
						RedirectStandardOutput = true,
						UseShellExecute = false
					}
				};
				timer = Stopwatch.StartNew();
				proc.Start();
				var decodeResults = DrainStream(proc.StandardOutput);
				proc.WaitForExit();
				decodingTicks[size - 10] = timer.ElapsedMilliseconds;
				Console.WriteLine(name + " finished");
			});
		}

		public static void WriteResults(string name, long[] encodeTicks, long[] decodeTicks)
		{
			Console.WriteLine("-------" + name);
			for (int size = 10; size <= 16; size++)
			{
				Console.WriteLine("\t{0}: Encode {1}, Decode {2}", 1 << size, encodeTicks[size - 10], decodeTicks[size - 10]);
			}
		}

		public static byte[] DrainStream(StreamReader stdout)
		{
			var bytes = new List<byte>();
			var buffer = new byte[4096];
			int amountRead = 0;
			while ((amountRead = stdout.BaseStream.Read(buffer, 0, 4096)) > 0)
			{
				for (int i = 0; i < amountRead; i++)
					bytes.Add(buffer[i]);
			}
			return bytes.ToArray();
		}
	}
}