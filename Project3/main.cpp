#include "Project3.h"
#include "Huffman.h"
#include "LempelZiv.h"
#include "Passthrough.h"
#include <memory>
#include <fstream>

// Attempts to translate command line arguments into the
// specific compression algorithm to use and the input
// source to read data from. Throws an exception with an
// error message if unsuccessful.
void ParseArgs(
	const char ** args, 
	int argCount, 
	std::shared_ptr<Project3::ICompressionAlg> & compressionAlg, 
	std::shared_ptr<std::istream> & input, 
	int & readBufferSize);

const int FileInputReadBufferSize = 1024;
const int ConsoleInputReadBufferSize = 1;

int main(int argCount, const char ** args)
{
	std::shared_ptr<Project3::ICompressionAlg> compressionAlg = nullptr;
	std::shared_ptr<std::istream> input = nullptr;
	int readBufferSize = 0;
	try
	{
		ParseArgs(args, argCount, compressionAlg, input, readBufferSize);
	}
	catch (std::exception e)
	{
		std::cerr << "Error parsing arguments: " << e.what();
		return EXIT_FAILURE;
	}

	char readBuffer[FileInputReadBufferSize > ConsoleInputReadBufferSize ? FileInputReadBufferSize : ConsoleInputReadBufferSize];

	// Read data from input stream in chunks of size ReadBufferSize.
	// We can't get size of input ahead of time because the input stream
	// might be std::cin, which has no size.
	while (input->read(readBuffer, 1) && input->good())
	{
		try
		{
			compressionAlg->Process((std::uint8_t)readBuffer[0]);
		}
		catch (std::exception e)
		{
			std::cerr << "Exception thrown while processing input: " << e.what();
			return EXIT_FAILURE;
		}
	}
	
	// Finish the encoding or decoding.
	try
	{
		compressionAlg->Finish();
		return EXIT_SUCCESS;
	}
	catch (std::exception e)
	{
		std::cerr << "Exception thrown when compression algorithm finishing: " << e.what();
		return EXIT_FAILURE;
	}
}

void ParseArgs(
	const char ** args, 
	int argCount, 
	std::shared_ptr<Project3::ICompressionAlg> & compressionAlg, 
	std::shared_ptr<std::istream> & input, 
	int & readBufferSize)
{
	// Blank command line arguments is an error, need to
	// specify at least the operation.
	if (argCount < 2)
		throw std::exception("Must specify operation.");

	// If file name not provided, get input from std::cin (note noop deleter).
	if (argCount < 3)
	{
		input = std::shared_ptr<std::istream>(&std::cin, [](std::istream*) -> void {});
		readBufferSize = ConsoleInputReadBufferSize;
	}
	// Otherwise read from file.
	else
	{
		input = std::shared_ptr<std::istream>(new std::ifstream(args[2], std::ios_base::in | std::ios_base::binary));
		readBufferSize = FileInputReadBufferSize;
		if (!input->good())
			throw std::exception("Unable to open file.");
	}

	// Note noop deleter.
	std::shared_ptr<std::ostream> outputStream(&std::cout, [](std::ostream *) -> void{});

	// Now determine the compression action to perform. This
	// needs to come after resolving the input stream so that
	// the first byte can be read in the EXPAND case.
	if (strcmp(args[1],"HUFF") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::HuffmanEncoder(outputStream));
		char idVal = static_cast<char>(Project3::HuffmanIdentifier);
		std::cout.write(&idVal, 1);
	}
	else if (strcmp(args[1], "LZ1") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivEncoder(outputStream, 2048));
		char idVal = static_cast<char>(Project3::LempelZiv1Identifier);
		std::cout.write(&idVal, 1);
	}
	else if (strcmp(args[1], "LZ2") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivEncoder(outputStream, 4096));
		char idVal = static_cast<char>(Project3::LempelZiv2Identifier);
		std::cout.write(&idVal, 1);
	}
	else if (strcmp(args[1], "PASS") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::PassthroughEncoder(outputStream));
		char idVal = static_cast<char>(Project3::PassthroughIdentifier);
		std::cout.write(&idVal, 1);
	}
	else if (strcmp(args[1], "EXPAND") == 0)
	{
		// Read the first byte to determine decode algorithm to use.
		std::uint8_t encodeIdentifier;
		*input >> encodeIdentifier;
		switch (encodeIdentifier)
		{
		case Project3::HuffmanIdentifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::HuffmanDecoder(outputStream));
			break;
		case Project3::LempelZiv1Identifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivDecoder(outputStream, 2048));
			break;
		case Project3::LempelZiv2Identifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivDecoder(outputStream, 4096));
			break;
		case Project3::PassthroughIdentifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::PassthroughDecoder(outputStream));
			break;
		default:
			throw std::exception("Unknown operation identifier, could not decide what decoder to use.");
		}
	}
	else
		throw std::exception("Invalid operation, must be one of HUFF, LZ1, LZ2, PASS, EXPAND.");
}