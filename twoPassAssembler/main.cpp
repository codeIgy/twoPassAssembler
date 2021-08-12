
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
	string inputFileName, outputFileName;

	if (argc == 2) {
		inputFileName = argv[1];
		outputFileName = "output.o";
	}
	else if(argc == 4 && argv[1] == "-o"){
		inputFileName = argv[3];
		outputFileName = argv[2];
	}
	else {
		cout << "Error: Wrong command line arguments" << endl;
		return 0;
	}

	ifstream inputFile;
	ofstream outputFile;

	inputFile.open(inputFileName, ios::in);
	if (!inputFile) {
		cout << "Error: Unable to open input file" << endl;
		return 0;
	}

	outputFile.open(outputFileName, ios::out);
	if (!outputFile) {
		cout << "Error: Unable to open output file" << endl;
		return 0;
	}


	return 0;

}