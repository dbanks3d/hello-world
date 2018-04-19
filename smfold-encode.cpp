#include <iostream>
#include <fstream>

#define DEBUG 1 // verbose debug messages

/////////////////////////////////////////////////////
// Read a file; encode it; write the encoded result.
 
int smfold_encode (char *inputData, int numBytes, FILE *outputFile)
{
	fwrite (inputData, numBytes, 1, outputFile); // trivial encoding
	fflush(outputFile);
	return (0);
}

/////////////////////////////////////////////////////
// main()
//
// Usage:
// <program-name>  --input filename  --output filename

int main(int argc, char** argv) {
	int  command_flag_error = 0;
	FILE *inputFile         = NULL, *outputFile     = NULL;
	char *inputFilename     = NULL, *outputFilename = NULL;

	// Parse the command line

	if (argc > 1) {
		for (int i = 1; (i < argc) && (!command_flag_error); i++) {
			if (std::string(argv[i]) == "--input") {
				command_flag_error |= !(i+1 < argc);
				inputFilename = argv[++i];
				}
			else if (std::string(argv[i]) == "--output") {
				command_flag_error |= !(i+1 < argc);
				outputFilename = argv[++i];
				}
			else {
				command_flag_error = 1;
				std::cout << "Couldn't parse flag number " << i << ": " << argv[i] << "\n";
			}
		}
	}
	else command_flag_error = 1;
	if (command_flag_error || !inputFilename || !outputFilename) {
		std::cout << "Usage: " << argv[0] << "  --input filename  --output filename \n";
		return (1);
	}

	errno_t err;
	// Read the input file into an array
	
	err = fopen_s(&inputFile, inputFilename, "r"); // Microsoft's "safe" fopen
		if (err) {
		std::cout << "Unable to read  file " << inputFilename << std::endl;
		return (1);
	}
	std::ifstream  fl(inputFilename, std::ios::in | std::ios::binary); // Read from disk into data array
	fl.seekg(0, std::ios::end);
	size_t len = fl.tellg();
	char *inputData = new char[len];
	fl.seekg(0, std::ios::beg);
	fl.read(inputData, len);
	fl.close();
	if (DEBUG) std::cout << "Length of input data: " << len << std::endl;
	if (DEBUG) std::cout << "Input file contains: " << inputData << std::endl;
	if (DEBUG) for (int i = 0; i < len; i++) std::cout << i << "\'" << inputData[i] << "\'" << std::endl;

	err = fopen_s(&outputFile, outputFilename, "w");
	if (err) {
		std::cout << "Unable to write file " << outputFilename << std::endl;
		return (1);
	}

	// Write the encoded data to a file

	int encode_error = smfold_encode(inputData, len, outputFile);
	fclose(outputFile);
}