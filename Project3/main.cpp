#include "Project3.h"
#include "Huffman.h"
#include "LempelZiv.h"
#include <memory>
#include <fstream>
#include <stdexcept>
#include <cstring>

#ifdef WIN32
#include <fcntl.h>
#include <io.h>
#endif

// Attempts to translate command line arguments into the
// specific compression algorithm to use and the input
// source to read data from. Throws an exception with an
// error message if unsuccessful.
void ParseArgs(
	const char ** args,
	int argCount,
	std::shared_ptr<Project3::ICompressionAlg> & compressionAlg,
	std::shared_ptr<std::istream> & input);


int main(int argCount, const char ** args)
{
#ifdef WIN32
	// Suppress newline translation in Windows.
	_setmode(_fileno(stdout), _O_BINARY);
#endif
	
	std::shared_ptr<Project3::ICompressionAlg> compressionAlg;
	std::shared_ptr<std::istream> input;
	try
	{
		ParseArgs(args, argCount, compressionAlg, input);
	}
	catch (std::logic_error e)
	{
		std::cerr << "Logic error parsing arguments: " << e.what();
		return EXIT_FAILURE;
	}
	catch (std::exception e)
	{
		std::cerr << "Error parsing arguments: " << e.what();
		return EXIT_FAILURE;
	}

	char readBuffer[1024];
	std::shared_ptr<std::vector<std::uint8_t>> data(new std::vector<std::uint8_t>);

	// Read data from input stream in chunks of size ReadBufferSize.
	// We can't get size of input ahead of time because the input stream
	// might be std::cin, which has no size.
	while (!input->eof())
	{
		input->read(readBuffer, 1024);
		int amountRead = input->gcount();
		for (int i = 0; i < amountRead; i++)
			data->push_back(readBuffer[i]);
	}
	
	// Perform the encoding or decoding.
	try
	{
		compressionAlg->Execute(std::cout, data);
		std::cout.flush();
		return EXIT_SUCCESS;
	}
	catch (std::logic_error e)
	{
		std::cerr << "Logic exception thrown when compression algorithm finishing: " << e.what();
		return EXIT_FAILURE;
	}
	//catch (std::exception e)
	//{
	//	std::cerr << "Exception thrown when compression algorithm finishing: " << e.what();
	//	return EXIT_FAILURE;
	//}
}

struct NoopDeleter
{
	void operator() (std::istream * unused) {}
};

void ParseArgs(
	const char ** args, 
	int argCount, 
	std::shared_ptr<Project3::ICompressionAlg> & compressionAlg, 
	std::shared_ptr<std::istream> & input)
{
	// Blank command line arguments is an error, need to
	// specify at least the operation.
	if (argCount < 2)
		throw std::logic_error("Must specify operation.");

	// If file name not provided, get input from std::cin (note noop deleter).
	if (argCount < 3)
	{
		input = std::shared_ptr<std::istream>(&std::cin, NoopDeleter());
	}
	// Otherwise read from file.
	else
	{
		input = std::shared_ptr<std::istream>(new std::ifstream(args[2], std::ios_base::in | std::ios_base::binary));
		if (!input->good())
			throw std::logic_error("Unable to open file.");
	}

	// Now determine the compression action to perform. This
	// needs to come after resolving the input stream so that
	// the first byte can be read in the EXPAND case.
	if (std::strcmp(args[1],"HUFF") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::HuffmanEncoder());
		char idVal = static_cast<char>(Project3::HuffmanIdentifier);
		std::cout.write(&idVal, 1);
	}
	else if (std::strcmp(args[1], "LZ1") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivEncoder(2048));
		char idVal = static_cast<char>(Project3::LempelZiv1Identifier);
		std::cout.write(&idVal, 1);
	}
	else if (std::strcmp(args[1], "LZ2") == 0)
	{
		compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivEncoder(4096));
		char idVal = static_cast<char>(Project3::LempelZiv2Identifier);
		std::cout.write(&idVal, 1);
	}
	else if (std::strcmp(args[1], "EXPAND") == 0)
	{
		// Read the first byte to determine decode algorithm to use.
		char encodeIdentifier;
		input->read(&encodeIdentifier, 1);
		switch (encodeIdentifier)
		{
		case Project3::HuffmanIdentifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::HuffmanDecoder());
			break;
		case Project3::LempelZiv1Identifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivDecoder(2048));
			break;
		case Project3::LempelZiv2Identifier:
			compressionAlg = std::shared_ptr<Project3::ICompressionAlg>(new Project3::LempelZivDecoder(4096));
			break;
		default:
			throw std::logic_error("Unknown operation identifier, could not decide what decoder to use.");
		}
	}
	else
		throw std::logic_error("Invalid operation, must be one of HUFF, LZ1, LZ2, PASS, EXPAND.");
}