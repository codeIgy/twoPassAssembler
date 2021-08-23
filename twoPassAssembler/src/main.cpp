
#include <iostream>
#include <string>
#include <fstream>
#include "assembler.h"

using namespace std;

int main(int argc, char** argv) {
	string inputFileName, outputFileName, outputFileNameBin;

	if (argc == 2) {
		inputFileName = argv[1];
		outputFileName = "output.o.txt";
		outputFileNameBin = "output.o";
	}
	else if(argc == 4 && string(argv[1]) ==  "-o"){
		inputFileName = argv[3];
		outputFileName = argv[2];
		outputFileName += ".txt";
		outputFileNameBin = argv[2];
	}
	else {
		cout << "Error: Wrong command line arguments" << endl;
		return 0;
	}

	ifstream inputFile;
	ofstream outputFile;
	ofstream outputFileBin;

	inputFile.open(inputFileName, ios::in);
	if (!inputFile) {
		cout << "Error: Unable to open input file" << endl;
		return 0;
	}

	outputFile.open(outputFileName, ios::out);
	if (!outputFile) {
		cout << "Error: Unable to open text output file" << endl;
		outputFile.close();
		return 0;
	}

	outputFileBin.open(outputFileNameBin, ios::out);
	if (!outputFileBin) {
		cout << "Error: Unable to open binary output file" << endl;
		outputFileBin.close();
		return 0;
	}

	Assembler assembler;

	assembler.assemble(inputFile, outputFile, outputFileBin);

	inputFile.close();
	outputFile.close();
	outputFileBin.close();
	return 0;

}