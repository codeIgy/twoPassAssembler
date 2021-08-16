#include <iostream>

#include "assembler.h"
#include "assemblerException.h"
#include "lex.h"
#include <sstream>

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
			else if(regex_search(line,symbols, RegExpr::directiveExtern)) {
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
				sectionIndex = table.insertSection(symbols[1], 0);
				locationCounter = 0;
			}
			else if (sectionIndex == 0) { //section has not been defined!
				throw "Section must be declared first!";
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

						searchStartSymbol = symbols.suffix().first;//iterate
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
			}

		}
	}
	catch (AssemblerException e) {
		string message = "Error on line ";
		message += lineNumber;
		message += ": ";
		message += e.getMsg();

		throw AssemblerException(message.c_str());
	}

}

void Assembler::passSecondTime()
{
}