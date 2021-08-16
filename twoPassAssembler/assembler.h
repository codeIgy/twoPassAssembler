#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <string>
#include "symbolTable.h"

class Assembler
{
public:
	Assembler();
	~Assembler();
	void assemble(ifstream& inputFile, ofstream& outputFile);

private:
	//fields
	int sectionIndex = 0;
	int locationCounter = 0;
	MySymbolTable table;
	void passFirstTime(ifstream& inputFile);
	void passSecondTime();
};
#endif
