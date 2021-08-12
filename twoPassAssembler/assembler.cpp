#include <iostream>

#include "assembler.h"
#include "assemblerException.h"
Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}

void Assembler::assemble(ifstream& inputFile, ofstream& outputFile) {
	try
	{
		passFirstTime(inputFile);
		passSecondTime();
	}
	catch (AssemblerException e) {
		cout << e.getMsg() << endl;
	}
}

void Assembler::passFirstTime(ifstream& inputFile) {
	bool endReached = false;
	string line;
	while (!endReached) {
		getline(inputFile, line);
	}
}