using System;
using System.Diagnostics;
using System.IO;

namespace Project3Stats
{
	public static class Program
	{
		private const int EXIT_FAILURE = 1;

		private static readonly TestCase[] TestCases =
		{
			new TestCase {TestFileName = "news"},
			new TestCase {TestFileName = "book1"},
			new TestCase {TestFileName = "kennedy.xls"}
		};

		public static void Main()
		{
			foreach (var test in TestCases)
			{
				test.ExternalResult = new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "External compression not implemented."
				};
				test.HuffResult = RunCompression("HUFF", test.TestFileName);
				test.Lz1Result = RunCompression("LZ1", test.TestFileName);
				test.Lz2Result = RunCompression("LZ2", test.TestFileName);
				test.PassResult = RunCompression("PASS", test.TestFileName);
			}

			using (var output = File.CreateText("results.csv"))
			{
				output.WriteLine("filename,size,,HUFFsize,%save,t-cmprss,t-expand,,LZ1size,%save,t-cmprss,t-expand,,LZ2size,%save,t-cmprss,t-expand,,PASSsize,%save,t-cmprss,t-expand,,EXTERNALsize,%save,t-cmprss,t-expand");
				foreach (var test in TestCases)
				{
					output.Write(test.TestFileName);
					output.Write(',');
					output.Write(new FileInfo(test.TestFileName).Length);
					output.Write(",,");

					output.WriteResult(test.HuffResult);
					output.Write(",,");
					output.WriteResult(test.Lz1Result);
					output.Write(",,");
					output.WriteResult(test.Lz2Result);
					output.Write(",,");
					output.WriteResult(test.PassResult);
					output.Write(",,");
					output.WriteResult(test.ExternalResult);
					output.WriteLine();
				}
			}
		}

		public static void WriteResult(this StreamWriter output, CompressionResult result)
		{
			if (result.Succeeded)
			{
				output.Write(result.CompressedSize);
				output.Write(',');
				output.Write(result.CompressionFactor);
				output.Write(',');
				output.Write(result.CompressionTime);
				output.Write(',');
				output.Write(result.ExpansionTime);
			}
			else
			{
				output.Write(result.ErrorMessage);
				output.Write(",-,-,-");
			}
		}

		public static CompressionResult RunCompression(string algorithm, string filePath)
		{
			FileInfo fileInfo = null;

			try
			{
				fileInfo = new FileInfo(filePath);
			}
			catch (Exception e)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Unable to open input file."
				};
			}
			
			Process compressionProcess = null;
			try
			{
				compressionProcess = new Process
				{
					StartInfo = new ProcessStartInfo
					{
						FileName = "../../../Debug/Project3.exe",
						Arguments = string.Format("{0} \"{1}\"", algorithm, filePath),
						RedirectStandardError = true,
						RedirectStandardOutput = true,
						UseShellExecute = false,
						ErrorDialog = false,
						CreateNoWindow = true
					}
				};
			}
			catch
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Failed to start compression process."
				};
			}

			long compressionMillis = 0;

			try
			{
				var timer = Stopwatch.StartNew();
				compressionProcess.Start();
				compressionProcess.WaitForExit(10000);
				compressionMillis = timer.ElapsedMilliseconds;
			}
			catch (Exception e)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Exception while waiting for exit: " + e.Message
				};
			}
			// Timeout?
			if (!compressionProcess.HasExited)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Process took too long to exit."
				};
			}
			// Internal error?
			if (compressionProcess.ExitCode == EXIT_FAILURE)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Compression failed: " + compressionProcess.StandardError.ReadToEnd()
				};
			}

			var outStream = compressionProcess.StandardOutput;
			Console.WriteLine("{0} compressed to: {1}", algorithm, outStream.ReadToEnd());
			compressionProcess.StandardOutput.BaseStream.Position = 0;

			Process decompressionProcess = null;

			try
			{
				decompressionProcess = new Process
				{
					StartInfo = new ProcessStartInfo
					{
						FileName = "../../../Debug/Project3.exe",
						Arguments = "EXPAND",
						RedirectStandardError = true,
						RedirectStandardOutput = true,
						UseShellExecute = false,
						ErrorDialog = false,
						CreateNoWindow = true
					}
				};
			}
			catch
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Failed to create decompression process."
				};
			}

			long decompressionMillis = 0;

			// Write all compressed data to the decompression process' standard input
			// stream then wait for it to exit.
			try
			{
				var bytes = new byte[outStream.BaseStream.Length];
				int bytesRead = outStream.BaseStream.Read(bytes, 0, bytes.Length);
				if (bytesRead != bytes.Length)
					return new CompressionResult
					{
						Succeeded = false,
						ErrorMessage = "Unable to read all bytes from compression stream."
					};
				var timer = Stopwatch.StartNew();
				decompressionProcess.Start();
				decompressionProcess.StandardInput.BaseStream.Write(bytes, 0, bytes.Length);
				decompressionProcess.StandardInput.Close();
				decompressionProcess.WaitForExit(10000);
				decompressionMillis = timer.ElapsedMilliseconds;
			}
			catch
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Exception thrown while decompressing."
				};
			}

			// Timeout?
			if (!decompressionProcess.HasExited)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Process took too long to exit."
				};
			}
			// Internal error?
			if (decompressionProcess.ExitCode == EXIT_FAILURE)
			{
				return new CompressionResult
				{
					Succeeded = false,
					ErrorMessage = "Decompression failed: " + decompressionProcess.StandardError.ReadToEnd()
				};
			}

			return new CompressionResult
			{
				Succeeded = true,
				CompressionFactor = fileInfo.Length / (float) outStream.BaseStream.Length,
				CompressionTime = compressionMillis / 1000f,
				ExpansionTime = decompressionMillis / 1000f,
				CompressedSize = (int) outStream.BaseStream.Length
			};
		}
	}

	public class CompressionResult
	{
		public bool Succeeded;
		public string ErrorMessage;
		public float CompressionFactor;
		public float CompressionTime;
		public float ExpansionTime;
		public int CompressedSize;
	}

	public class TestCase
	{
		public string TestFileName;
		public CompressionResult HuffResult;
		public CompressionResult Lz1Result;
		public CompressionResult Lz2Result;
		public CompressionResult PassResult;
		public CompressionResult ExternalResult;
	}
}
