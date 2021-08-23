#include <iostream>
#include <unordered_set>
#include <iomanip>

#include "assembler.h"
#include "assemblerException.h"
#include "lex.h"


Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}

void Assembler::assemble(ifstream& inputFile, ofstream& outputFile, ofstream& outputFileBin) {
	try
	{
		table.clearTable();

		passFirstTime(inputFile);

		table.setOrdinals();

		passSecondTime(inputFile, outputFile, outputFileBin);
	}
	catch (AssemblerException e) {
		cout << e.getMsg() << endl;
	}
}

void Assembler::writeRelocDataToFiles(ofstream& outputFile, ofstream& outputFileBin) {
	writeRelocData(outputFile);
	writeRelocDataBin(outputFileBin);

	relocTable.clear();
}

void Assembler::writeRelocDataBin(ofstream& outputFileBin) {

}

void Assembler::writeRelocData(ofstream & outputFile)
{
	outputFile << endl << "$relocation data" << endl << left << hex << setw(10) << setfill(' ') << "offset" << setw(12) << "type" << setw(10) << "ordinal" << endl;

	for (auto r : relocTable) {
		outputFile << setw(10) << r.offset << setw(12) << (RelocationEntry::R_386_16 == r.relType ? "R_386_16" : "R_386_PC16") << r.ordinal << endl;
	}
}

void Assembler::writeSectionToFiles(TableEntry & section, ofstream & outputFile, ofstream & outputFileBin) {
	writeSection(section, outputFile);
	writeSectionBin(section, outputFileBin);
}

void Assembler::writeSectionBin(TableEntry & section, ofstream & outputFileBin) {

}

void Assembler::writeSection(TableEntry & section, ofstream & outputFile)
{
	outputFile << "$." << section.label << ":" << hex << section.size << endl;
	outputColumnCnt = 0;
}

void Assembler::writeSymbol(TableEntry & symbol, ofstream & outputFile, ofstream&  outputFileBin, RelocationEntry::type relocType)
{
	//dodati dio ako je ista sekcija zbog pc rel i vidjeti sta sa apsolutnim simbolom
	RelocationEntry entry;

	if (symbol.section == 1) { //absolute symbol
		if (relocType == RelocationEntry::R_386_16) {
			write2BytesToFiles(symbol.value, outputFile, outputFileBin);
		}
		else {//not covered
			int offset = symbol.value - 2;
			write2BytesToFiles(symbol.value, outputFile, outputFileBin);
		}
	}
	else {
		if (relocType == RelocationEntry::R_386_16) {
			entry.relType = RelocationEntry::R_386_16;
			if (symbol.isExt || symbol.visibility == 'g') { //global symbol
				entry.offset = locationCounter;
				entry.ordinal = symbol.id;
				write2BytesToFiles(0, outputFile, outputFileBin);
			}
			else { //local symbol
				entry.offset = locationCounter;
				entry.ordinal = symbol.section;
				write2BytesToFiles(symbol.value, outputFile, outputFileBin);
			}
			relocTable.push_back(entry);
		}
		else {
			if (sectionIndex == symbol.section) {
				write2BytesToFiles(symbol.value - locationCounter - 2, outputFile, outputFileBin);
			}
			else {
				entry.relType = RelocationEntry::R_386_PC16;
				if (symbol.isExt || symbol.visibility == 'g') { //global symbol
					entry.offset = locationCounter;
					entry.ordinal = symbol.id;
					write2BytesToFiles(0 - 2, outputFile, outputFileBin);
				}
				else { //local symbol
					entry.offset = locationCounter;
					entry.ordinal = symbol.section;
					write2BytesToFiles(symbol.value - 2, outputFile, outputFileBin);
				}
				relocTable.push_back(entry);
			}
		}
	}
}

void Assembler::writeSymbolWord(TableEntry & symbol, ofstream & outputFile, ofstream & outputFileBin, RelocationEntry::type relocType)
{
	//dodati dio ako je ista sekcija zbog pc rel i vidjeti sta sa apsolutnim simbolom
	RelocationEntry entry;

	if (symbol.section == 1) { //absolute symbol
		write2BytesWordToFiles(symbol.value, outputFile, outputFileBin);
	}
	else {
		entry.relType = RelocationEntry::R_386_16;
		if (symbol.isExt || symbol.visibility == 'g') { //global symbol
			entry.offset = locationCounter;
			entry.ordinal = symbol.id;
			write2BytesWordToFiles(0, outputFile, outputFileBin);
		}
		else { //local symbol
			entry.offset = locationCounter;
			entry.ordinal = symbol.section;
			write2BytesWordToFiles(symbol.value, outputFile, outputFileBin);
		}
		relocTable.push_back(entry);
	}
}

void Assembler::writeByteToFiles(int value, ofstream & outputFile, ofstream & outputFileBin) {
	writeByte(value, outputFile);
	writeByteBin(value, outputFileBin);
}

void Assembler::writeByteBin(int value,  ofstream & outputFileBin) {

}

void Assembler::writeByte(int value, ofstream & outputFile)
{
	int mask = 0xFF;

	if (outputColumnCnt == 15) {
		outputColumnCnt = 0;
		outputFile << endl;
	}

	outputFile << setw(2) << right << setfill('0') << hex << (value & mask);

	if (++outputColumnCnt < 15) {
		outputFile << " ";
	}
}

void Assembler::write2BytesWordToFiles(int value, ofstream & outputFile, ofstream & outputFileBin) {
	write2BytesWord(value, outputFile);
	write2BytesWordBin(value, outputFileBin);
}

void Assembler::write2BytesWordBin(int value, ofstream & outputFileBin) {

}

void Assembler::write2BytesWord(int value, ofstream & outputFile)
{
	int lowerByte = 0xFF;
	int upperByte = lowerByte << 8;

	if (outputColumnCnt == 15) {
		outputColumnCnt = 0;
		outputFile << endl;
	}

	outputFile << setw(2) << setfill('0') << right << hex << (value & lowerByte); //lower byte

	if (++outputColumnCnt == 15) {
		outputColumnCnt = 0;
		outputFile << endl;
	}
	else
		outputFile << " ";

	outputFile << setw(2) << setfill('0') << right << hex << ((value & upperByte) >> 8); //upper byte

	if (++outputColumnCnt < 15) {
		outputFile << " ";
	}
}

void Assembler::write2BytesToFiles(int value, ofstream & outputFile, ofstream & outputFileBin) {
	write2Bytes(value, outputFile);
	write2BytesBin(value, outputFileBin);
}

void Assembler::write2BytesBin(int value, ofstream & outputFileBin) {

}

void Assembler::write2Bytes(int value, ofstream & outputFile)
{
	int lowerByte = 0xFF;
	int upperByte = lowerByte << 8;

	if (outputColumnCnt == 15) {
		outputColumnCnt = 0;
		outputFile << endl;
	}

	outputFile << setw(2) << setfill('0') << right << hex << ((value & upperByte) >> 8); //upper byte

	if (++outputColumnCnt == 15) {
		outputColumnCnt = 0;
		outputFile << endl;
	}
	else
		outputFile << " ";

	outputFile << setw(2) << setfill('0') << right << hex << (value & lowerByte); //lower byte

	if (++outputColumnCnt < 15) {
		outputFile << " ";
	}
}

int Assembler::extractRegInfo(string s)
{
	int num = 0;

	if (s == "pc") {
		num = 7;
	}
	else if (s == "psw") {
		num = 8;
	}
	else if (s == "sp") {
		num = 6;
	}
	else {
		smatch symbol;
		regex_search(s, symbol, RegExpr::value);
		num = stoi(symbol[0]);
	}

	return num;
}

void Assembler::processAndWriteAddress(string s, int reg1, ofstream & outputFile, ofstream & outputFileBin)
{
	smatch symbols;
	int mask = 0x0F;
	if (regex_search(s, symbols, RegExpr::indirectReg)) {
		int reg2 = extractRegInfo(symbols[1]);
		writeByteToFiles(((mask & reg1) << 4) | (mask & reg2), outputFile, outputFileBin);
		writeByteToFiles(0x02, outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::directReg)) {
		int reg2 = extractRegInfo(symbols[1]);
		writeByteToFiles(((mask & reg1) << 4) | (mask & reg2), outputFile, outputFileBin);
		writeByteToFiles(0x01, outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::immLiteral)) {
		writeByteToFiles(((mask & reg1) << 4) | 0x0F, outputFile, outputFileBin);
		writeByteToFiles(0x00, outputFile, outputFileBin);

		int value;
		string numStr = symbols[1];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::addressLiteral)) {
		writeByteToFiles(((mask & reg1) << 4) | 0x0F, outputFile, outputFileBin);
		writeByteToFiles(0x04, outputFile, outputFileBin);

		int value;
		string numStr = symbols[1];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::indirectSumLiteral)) {
		int reg2 = extractRegInfo(symbols[1]);
		writeByteToFiles(((mask & reg1) << 4) | (mask & reg2), outputFile, outputFileBin);
		writeByteToFiles(0x03, outputFile, outputFileBin);

		int value;
		string numStr = symbols[2];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::immSymbol)) {
		writeByteToFiles(((mask & reg1) << 4) | 0x0F, outputFile, outputFileBin);
		writeByteToFiles(0x00, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin);

		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::pcRel)) {
		int reg2 = 0x07;
		writeByteToFiles(((mask & reg1) << 4) | (mask & reg2), outputFile, outputFileBin);
		writeByteToFiles(0x03, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin, RelocationEntry::R_386_PC16);

		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::addressSymbol)) {
		writeByteToFiles(((mask & reg1) << 4) | 0x0F, outputFile, outputFileBin);
		writeByteToFiles(0x04, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin);

		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::indirectSumSymbol)) {
		int reg2 = extractRegInfo(symbols[1]);
		writeByteToFiles(((mask & reg1) << 4) | (mask & reg2), outputFile, outputFileBin);
		writeByteToFiles(0x03, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[2]), outputFile, outputFileBin);

		locationCounter += 2;
	}
}

void Assembler::processAndWriteAddressJmp(string s, ofstream & outputFile, ofstream & outputFileBin)
{
	smatch symbols;
	int mask = 0x0F;
	if (regex_search(s, symbols, RegExpr::indirectRegJmp)) {
		int reg = extractRegInfo(symbols[1]);
		writeByteToFiles(0xF0 | (mask & reg), outputFile, outputFileBin);
		writeByteToFiles(0x02, outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::directRegJmp)) {
		int reg = extractRegInfo(symbols[1]);
		writeByteToFiles(0xF0 | (mask & reg), outputFile, outputFileBin);
		writeByteToFiles(0x01, outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::immLiteralJmp)) {
		writeByteToFiles(0xFF, outputFile, outputFileBin);
		writeByteToFiles(0x00, outputFile, outputFileBin);

		int value;
		string numStr = symbols[1];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::addressLiteralJmp)) {
		writeByteToFiles(0xFF, outputFile, outputFileBin);
		writeByteToFiles(0x04, outputFile, outputFileBin);

		int value;
		string numStr = symbols[1];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::indirectSumLiteralJmp)) {
		int reg = extractRegInfo(symbols[1]);
		writeByteToFiles(0xF0 | (mask & reg), outputFile, outputFileBin);
		writeByteToFiles(0x03, outputFile, outputFileBin);

		int value;
		string numStr = symbols[2];
		if (regex_search(numStr, RegExpr::hex)) {
			value = stoi(numStr, nullptr, 16);
		}
		else {
			value = stoi(numStr);
		}

		write2BytesToFiles(value, outputFile, outputFileBin);
		locationCounter += 4;
	}
	else if (regex_search(s, symbols, RegExpr::immSymbolJmp)) {
		writeByteToFiles(0xFF, outputFile, outputFileBin);
		writeByteToFiles(0x00, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::pcRelJmp)) {
		int reg = 0x07;
		writeByteToFiles(0xF0 | (mask & reg), outputFile, outputFileBin);
		writeByteToFiles(0x05, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin, RelocationEntry::R_386_PC16);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::addressSymbolJmp)) {
		writeByteToFiles(0xFF, outputFile, outputFileBin);
		writeByteToFiles(0x04, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[1]), outputFile, outputFileBin);
		locationCounter += 2;
	}
	else if (regex_search(s, symbols, RegExpr::indirectSumSymbolJmp)) {
		int reg = extractRegInfo(symbols[1]);
		writeByteToFiles(0xF0 | (mask & reg), outputFile, outputFileBin);
		writeByteToFiles(0x03, outputFile, outputFileBin);
		locationCounter += 2;

		writeSymbol(table.getSymbol(symbols[2]), outputFile, outputFileBin);
		locationCounter += 2;
	}
}

void Assembler::passFirstTime(ifstream& inputFile) {
	bool endReached = false;
	int lineNumber = 0;
	string line;

	locationCounter = 0;
	sectionIndex = 0;
	try {
		while (!endReached) {
			getline(inputFile, line);

			lineNumber++;

			//remove line comment if exists
			size_t commentStart = line.find('#');
			if (commentStart != string::npos) {
				line.erase(commentStart, line.size() - commentStart);
			}

			//check for label
			smatch labelName;
			if (regex_search(line, labelName, RegExpr::label)) {
				string newLabel = labelName[1];
				line.erase(0, line.find(':') + 1);

				if (sectionIndex == 0) throw AssemblerException("Label must be inside a section!");

				table.insertNonSection(newLabel, sectionIndex, locationCounter);
			}

			if (line.empty()) continue; //nothing left for us to analyze

			smatch symbols;//used for extraction of names of symbols

			if (regex_search(line, symbols, RegExpr::directiveGlobal)) {
				string symbol = symbols[1];
				table.changeVisibilityToGlobal(symbol);
				if (symbols.size() > 1) {
					string labels = symbols[2];
					string::const_iterator searchStart(labels.cbegin());
					while (regex_search(searchStart, labels.cend(), symbols, RegExpr::symbol)) {
						symbol = symbols[0];
						table.changeVisibilityToGlobal(symbol);

						searchStart = symbols.suffix().first;
					}
				}
			}
			else if (regex_search(line, symbols, RegExpr::directiveExtern)) {
				string symbol = symbols[1];
				table.insertNonSection(symbol, 0, 0, 'g', true);//declare symbol as extern
				if (symbols.size() > 1) {
					string labels = symbols[2];
					string::const_iterator searchStart(labels.cbegin());
					while (regex_search(searchStart, labels.cend(), symbols, RegExpr::symbol)) {
						symbol = symbols[0];
						table.insertNonSection(symbol, 0, 0, 'g', true);//declare symbol as extern

						searchStart = symbols.suffix().first;
					}
				}
			}
			else if (regex_search(line, symbols, RegExpr::directiveSection)) {
				if (sectionIndex != 0)
					table.updateSectionSize(sectionIndex, locationCounter);
				sectionIndex = table.insertSection(symbols[1], 0);
				locationCounter = 0;
			}
			else if (sectionIndex == 0) { //section has not been defined!
				throw AssemblerException("Section must be declared first!");
			}
			else if (regex_search(line, symbols, RegExpr::directiveWord)) {
				string arg = symbols[1];
				if (regex_match(arg, RegExpr::symbol)) {
					table.markAsUsed(arg);
				}
				locationCounter += 2;

				if (symbols.size() > 1) {
					//used symbols
					string arguments = symbols[2];
					string::const_iterator searchStartSymbol(arguments.cbegin());
					while (regex_search(searchStartSymbol, arguments.cend(), symbols, RegExpr::symbol)) {
						arg = symbols[0];
						table.markAsUsed(arg);
						locationCounter += 2;

						searchStartSymbol = symbols.suffix().first;//iterate
					}

					//values - needed for location counter
					string::const_iterator searchStartValue(arguments.cbegin());
					while (regex_search(searchStartValue, arguments.cend(), symbols, RegExpr::value)) {
						locationCounter += 2;

						searchStartValue = symbols.suffix().first;//iterate
					}
				}
			}
			else if (regex_search(line, symbols, RegExpr::directiveSkip)) {
				string value = symbols[1];
				int base = 10;
				if (regex_search(value, RegExpr::hex)) {
					base = 16;
				}
				int num = stoi(value, nullptr, base);
				locationCounter += num;
			}
			else if (regex_search(line, symbols, RegExpr::directiveEqu)) {
				string label = symbols[1];
				string value = symbols[2];
				int base = 10;
				if (regex_search(value, RegExpr::hex)) {
					base = 16;
				}
				int num = stoi(value, nullptr, base);

				table.insertNonSection(label, 1, num, 'l', false, true);
			}
			else if (regex_search(line, RegExpr::directiveEnd)) {
				endReached = true;
				if (sectionIndex) table.updateSectionSize(sectionIndex, locationCounter);
			}
			else if (regex_search(line, RegExpr::halt)) {
				locationCounter += 1;
			}
			else if (regex_search(line, symbols, RegExpr::interrupt)) {
				locationCounter += 2;
			}
			else if (regex_search(line, RegExpr::iret)) {
				locationCounter += 1;
			}
			else if (regex_search(line, RegExpr::callFixed)) {
				line = regex_replace(line, RegExpr::callFixed, "");

				if (regex_search(line, RegExpr::indirectRegJmp) || regex_search(line, RegExpr::directRegJmp)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteralJmp) || regex_search(line, RegExpr::addressLiteralJmp)
					|| regex_search(line, RegExpr::indirectSumLiteralJmp)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbolJmp) || regex_search(line, symbols, RegExpr::pcRelJmp) || regex_search(line, symbols, RegExpr::addressSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, RegExpr::ret)) {
				locationCounter += 1;
			}
			else if (regex_search(line, RegExpr::jmpFixed)) {
				line = regex_replace(line, RegExpr::jmpFixed, "");

				if (regex_search(line, RegExpr::indirectRegJmp) || regex_search(line, RegExpr::directRegJmp)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteralJmp) || regex_search(line, RegExpr::immSymbolJmp) || regex_search(line, RegExpr::pcRelJmp) || regex_search(line, RegExpr::addressLiteralJmp) || regex_search(line, RegExpr::addressSymbolJmp)
					|| regex_search(line, RegExpr::indirectSumLiteralJmp) || regex_search(line, RegExpr::indirectSumSymbolJmp)) {
					locationCounter += 5;
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, RegExpr::jeqFixed)) {
				line = regex_replace(line, RegExpr::jeqFixed, "");

				if (regex_search(line, RegExpr::indirectRegJmp) || regex_search(line, RegExpr::directRegJmp)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteralJmp) || regex_search(line, RegExpr::addressLiteralJmp)
					|| regex_search(line, RegExpr::indirectSumLiteralJmp)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbolJmp) || regex_search(line, symbols, RegExpr::pcRelJmp) || regex_search(line, symbols, RegExpr::addressSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, RegExpr::jneFixed)) {
				line = regex_replace(line, RegExpr::jneFixed, "");

				if (regex_search(line, RegExpr::indirectRegJmp) || regex_search(line, RegExpr::directRegJmp)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteralJmp) || regex_search(line, RegExpr::addressLiteralJmp)
					|| regex_search(line, RegExpr::indirectSumLiteralJmp)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbolJmp) || regex_search(line, symbols, RegExpr::pcRelJmp) || regex_search(line, symbols, RegExpr::addressSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, RegExpr::jgtFixed)) {
				line = regex_replace(line, RegExpr::jgtFixed, "");

				if (regex_search(line, RegExpr::indirectRegJmp) || regex_search(line, RegExpr::directRegJmp)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteralJmp) || regex_search(line, RegExpr::addressLiteralJmp)
					|| regex_search(line, RegExpr::indirectSumLiteralJmp)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbolJmp) || regex_search(line, symbols, RegExpr::pcRelJmp) || regex_search(line, symbols, RegExpr::addressSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbolJmp)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, symbols, RegExpr::push)) {
				locationCounter += 3;
			}
			else if (regex_search(line, symbols, RegExpr::pop)) {
				locationCounter += 3;
			}
			else if (regex_search(line, symbols, RegExpr::xchg)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::add)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::sub)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::mul)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::div)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::cmp)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicNot)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicAnd)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicOr)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicXor)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::test)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::shl)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::shr)) {
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::ldrFixed)) {
				line = regex_replace(line, RegExpr::ldrFixed, "");

				if (regex_search(line, RegExpr::indirectReg) || regex_search(line, RegExpr::directReg)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteral) || regex_search(line, RegExpr::addressLiteral)
					|| regex_search(line, RegExpr::indirectSumLiteral)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbol) || regex_search(line, symbols, RegExpr::pcRel) || regex_search(line, symbols, RegExpr::addressSymbol)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbol)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else if (regex_search(line, symbols, RegExpr::strFixed)) {
				line = regex_replace(line, RegExpr::strFixed, "");

				if (regex_search(line, RegExpr::indirectReg) || regex_search(line, RegExpr::directReg)) {
					locationCounter += 3;
				}
				else if (regex_search(line, RegExpr::immLiteral) || regex_search(line, RegExpr::addressLiteral)
					|| regex_search(line, RegExpr::indirectSumLiteral)) {
					locationCounter += 5;
				}
				else if (regex_search(line, symbols, RegExpr::immSymbol) || regex_search(line, symbols, RegExpr::pcRel) || regex_search(line, symbols, RegExpr::addressSymbol)) {
					locationCounter += 5;
					table.markAsUsed(symbols[1]);
				}
				else if (regex_search(line, symbols, RegExpr::indirectSumSymbol)) {
					locationCounter += 5;
					table.markAsUsed(symbols[2]);
				}
				else {
					throw AssemblerException("Syntax error!");
				}
			}
			else {
				throw AssemblerException("Syntax error!");
			}

		}

		if (!table.areAllSymbolsKnown()) {
			string msg = "Undefined symbols:";
			for (auto s : table.getUnknownUsedSymbols()) {
				msg += " ";
				msg += s;
				msg += ",";
			}
			msg.erase(msg.end() - 1);

			throw AssemblerException(msg);
		}
	}
	catch (AssemblerException e) {
		string message = "Error";
		if (table.areAllSymbolsKnown() || string(e.getMsg()) == "Syntax error!")
		{
			message += " on line ";
			message += to_string(lineNumber);
		}
		message += ": ";
		message += e.getMsg();

		throw AssemblerException(message);
	}

}

void Assembler::passSecondTime(ifstream& inputFile, ofstream& outputFile, ofstream& outputFileBin)
{
	bool endReached = false;
	int lineNumber = 0;
	string line;

	inputFile.seekg(0);

	table.printSymbolTable(outputFile, outputFile);

	locationCounter = 0;
	sectionIndex = 0;
	try {
		while (!endReached) {
			getline(inputFile, line);

			lineNumber++;

			//remove line comment if exists
			size_t commentStart = line.find('#');
			if (commentStart != string::npos) {
				line.erase(commentStart, line.size() - commentStart);
			}

			//check for label
			if (regex_search(line, RegExpr::label)) {
				line.erase(0, line.find(':') + 1);
			}

			if (line.empty()) continue; //nothing left for us to analyze

			smatch symbols;//used for extraction of names of symbols

			if (regex_search(line, symbols, RegExpr::directiveSection)) {
				if (sectionIndex != 0) {
					writeRelocDataToFiles(outputFile, outputFileBin);
				}

				TableEntry& section = table.getSymbol(symbols[1]);
				sectionIndex = section.id;
				locationCounter = 0;

				writeSectionToFiles(section, outputFile, outputFileBin);
			}
			else if (regex_search(line, symbols, RegExpr::directiveWord)) {
				string arg = symbols[1];

				if (regex_match(arg, RegExpr::symbol)) {
					writeSymbolWord(table.getSymbol(arg), outputFile, outputFileBin);
				}
				else if (regex_match(arg, RegExpr::hex)) {
					int val = stoi(arg, nullptr, 16);
					write2BytesWordToFiles(val, outputFile, outputFileBin);
				}
				else {
					int val = stoi(arg);
					write2BytesWordToFiles(val, outputFile, outputFileBin);
				}

				locationCounter += 2;

				if (symbols.size() > 1) {
					//symbols
					string arguments = symbols[2];
					string::const_iterator searchStartSymbol(arguments.cbegin());
					while (regex_search(searchStartSymbol, arguments.cend(), symbols, RegExpr::symbol)) {
						arg = symbols[0];

						if (regex_match(arg, RegExpr::symbol)) {
							writeSymbol(table.getSymbol(arg), outputFile, outputFileBin);
						}

						locationCounter += 2;

						searchStartSymbol = symbols.suffix().first;//iterate
					}

					//values
					string::const_iterator searchStartValue(arguments.cbegin());
					while (regex_search(searchStartValue, arguments.cend(), symbols, RegExpr::value)) {
						arg = symbols[0];

						if (regex_match(arg, RegExpr::hex)) {
							int val = stoi(arg, nullptr, 16);
							write2BytesWordToFiles(val, outputFile, outputFileBin);
						}
						else {
							int val = stoi(arg);
							write2BytesWordToFiles(val, outputFile, outputFileBin);
						}

						locationCounter += 2;

						searchStartValue = symbols.suffix().first;//iterate
					}
				}
			}
			else if (regex_search(line, symbols, RegExpr::directiveSkip)) {
				string value = symbols[1];
				int base = 10;
				if (regex_search(value, RegExpr::hex)) {
					base = 16;
				}

				int num = stoi(value, nullptr, base);

				for (int i = 0; i < num; i++) {
					writeByteToFiles(0, outputFile, outputFileBin);
				}

				locationCounter += num;
			}
			else if (regex_search(line, RegExpr::directiveEnd)) {
				endReached = true;
				writeRelocDataToFiles(outputFile, outputFileBin);
			}
			else if (regex_search(line, RegExpr::halt)) {
				writeByteToFiles(0, outputFile, outputFileBin);
				locationCounter += 1;
			}
			else if (regex_search(line, symbols, RegExpr::interrupt)) {
				int reg = extractRegInfo(symbols[1]);
				writeByteToFiles(0x10, outputFile, outputFileBin);
				writeByteToFiles((reg << 4) | 0x0F, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, RegExpr::iret)) {
				writeByteToFiles(0x20, outputFile, outputFileBin);
				locationCounter++;
			}
			else if (regex_search(line, RegExpr::callFixed)) {
				line = regex_replace(line, RegExpr::callFixed, "");

				writeByteToFiles(0x30, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddressJmp(line, outputFile, outputFileBin);
			}
			else if (regex_search(line, RegExpr::ret)) {
				writeByteToFiles(0x40, outputFile, outputFileBin);
				locationCounter++;
			}
			else if (regex_search(line, RegExpr::jmpFixed)) {
				line = regex_replace(line, RegExpr::jmpFixed, "");

				writeByteToFiles(0x50, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddressJmp(line, outputFile, outputFileBin);
			}
			else if (regex_search(line, RegExpr::jeqFixed)) {
				line = regex_replace(line, RegExpr::jeqFixed, "");

				writeByteToFiles(0x51, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddressJmp(line, outputFile, outputFileBin);
			}
			else if (regex_search(line, RegExpr::jneFixed)) {
				line = regex_replace(line, RegExpr::jneFixed, "");

				writeByteToFiles(0x52, outputFile, outputFileBin);
				locationCounter++;
				processAndWriteAddressJmp(line, outputFile, outputFileBin);
			}
			else if (regex_search(line, RegExpr::jgtFixed)) {
				line = regex_replace(line, RegExpr::jgtFixed, "");

				writeByteToFiles(0x53, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddressJmp(line, outputFile, outputFileBin);
			}
			else if (regex_search(line, symbols, RegExpr::push)) {
				int reg = extractRegInfo(symbols[1]);
				writeByteToFiles(0xB0, outputFile, outputFileBin);
				writeByteToFiles((reg << 4) | 0x06, outputFile, outputFileBin);
				writeByteToFiles(0x12, outputFile, outputFileBin);
				locationCounter += 3;
			}
			else if (regex_search(line, symbols, RegExpr::pop)) {
				int reg = extractRegInfo(symbols[1]);
				writeByteToFiles(0xA0, outputFile, outputFileBin);
				writeByteToFiles((reg << 4) | 0x06, outputFile, outputFileBin);
				writeByteToFiles(0x42, outputFile, outputFileBin);
				locationCounter += 3;
			}
			else if (regex_search(line, symbols, RegExpr::xchg)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x60, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::add)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x70, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::sub)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x71, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::mul)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x72, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::div)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x73, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::cmp)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x74, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicNot)) {
				int regD = extractRegInfo(symbols[1]);
				writeByteToFiles(0x80, outputFile, outputFileBin);
				writeByteToFiles((regD << 4), outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicAnd)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x81, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicOr)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x82, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::logicXor)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x83, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::test)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x84, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::shl)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x90, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::shr)) {
				int regD = extractRegInfo(symbols[1]);
				int regS = extractRegInfo(symbols[2]);
				writeByteToFiles(0x91, outputFile, outputFileBin);
				writeByteToFiles((regD << 4) | regS, outputFile, outputFileBin);
				locationCounter += 2;
			}
			else if (regex_search(line, symbols, RegExpr::ldrFixed)) {
				int reg1 = extractRegInfo(symbols[1]);
				line = regex_replace(line, RegExpr::ldrFixed, "");

				writeByteToFiles(0xA0, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddress(line, reg1, outputFile, outputFileBin);
			}
			else if (regex_search(line, symbols, RegExpr::strFixed)) {
				int reg1 = extractRegInfo(symbols[1]);
				line = regex_replace(line, RegExpr::strFixed, "");

				writeByteToFiles(0xB0, outputFile, outputFileBin);
				locationCounter++;

				processAndWriteAddress(line, reg1, outputFile, outputFileBin);
			}

		}
	}
	catch (AssemblerException e) {
		string message = "Error";
		if (table.areAllSymbolsKnown())
		{
			message += " on line ";
			message += to_string(lineNumber);
		}
		message += ": ";
		message += e.getMsg();

		throw AssemblerException(message);
	}

}