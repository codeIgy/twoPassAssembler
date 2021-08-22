#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <string>
#include "symbolTable.h"
#include "relocationEntry.h"

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
	vector<RelocationEntry>  relocTable;
	int outputColumnCnt = 0; //count number of bytes in the current row in the table

	void writeRelocData(ofstream& outputFile);
	void writeSection(TableEntry& section, ofstream& outputFile);
	void writeSymbol(TableEntry& section, ofstream& outputFile, RelocationEntry::type relocType = RelocationEntry::R_386_16);
	void writeByte(int value, ofstream& outputFile);
	void write2Bytes(int value, ofstream& outputFile);
	int extractRegInfo(string s);
	void processAndWriteAddressJmp(string s, ofstream& outputFile);
	void processAndWriteAddress(string s, int reg1, ofstream& outputFile);

	void passFirstTime(ifstream& inputFile);
	void passSecondTime(ifstream& inputFile, ofstream& outputFile);

	
};
#endif
