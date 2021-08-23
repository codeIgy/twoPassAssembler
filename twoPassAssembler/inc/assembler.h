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
	void assemble(ifstream& inputFile, ofstream& outputFile, ofstream& outputFileBin);

private:
	//fields
	int sectionIndex = 0;
	int locationCounter = 0;
	MySymbolTable table;
	vector<RelocationEntry>  relocTable;
	int outputColumnCnt = 0; //count number of bytes in the current row in the table

	void writeRelocDataToFiles(ofstream & outputFile, ofstream & outputFileBin);
	void writeRelocDataBin(ofstream & outputFileBin);
	void writeRelocData(ofstream& outputFile);
	void writeSectionToFiles(TableEntry & section, ofstream & outputFile, ofstream & outputFileBin);
	void writeSectionBin(TableEntry & section, ofstream & outputFileBin);
	void writeSection(TableEntry& section, ofstream& outputFile);
	void writeSymbol(TableEntry& section, ofstream& outputFile, ofstream& outputFileBin, RelocationEntry::type relocType = RelocationEntry::R_386_16);
	void writeSymbolWord(TableEntry& section, ofstream& outputFile, ofstream& outputFileBin, RelocationEntry::type relocType = RelocationEntry::R_386_16);
	void writeByteToFiles(int value, ofstream & outputFile, ofstream & outputFileBin);
	void writeByteBin(int value, ofstream & outputFileBin);
	void writeByte(int value, ofstream& outputFile);
	void write2BytesWordToFiles(int value, ofstream & outputFile, ofstream & outputFileBin);
	void write2BytesWordBin(int value, ofstream & outputFileBin);
	void write2Bytes(int value, ofstream& outputFile);
	void write2BytesWord(int value, ofstream& outputFile);
	void write2BytesToFiles(int value, ofstream & outputFile, ofstream & outputFileBin);
	void write2BytesBin(int value, ofstream & outputFileBin);
	int extractRegInfo(string s);
	void processAndWriteAddressJmp(string s, ofstream& outputFile, ofstream& outputFileBin);
	void processAndWriteAddress(string s, int reg1, ofstream& outputFile, ofstream& outputFileBin);

	void passFirstTime(ifstream& inputFile);
	void passSecondTime(ifstream& inputFile, ofstream& outputFile,ofstream& outputFileBin);

	
};
#endif
