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
			int commentStart = line.find('#');
			if (commentStart) {
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
				for (int i = 1; i < symbols.size(); i++) {
					table.changeVisibilityToGlobal(symbols[i]);
				}
			}
			else if(regex_search(line,symbols, RegExpr::directiveExtern)) {
				for (int i = 1; i < symbols.size(); i++) {
					table.insertNonSection(symbols[i], 0, 0, 'g', true);//declare symbol as extern
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
				for (int i = 1; i < symbols.size(); i++) {
					table.markAsUsed(symbols[i]);//just declare symbols
					locationCounter += 2;
				}
			}
			else if (regex_search(line, symbols, RegExpr::directiveSkip)) {
				stringstream s(symbols[1]);
				int num;
				s >> num;
				locationCounter += num;
			}
			else if (regex_search(line, symbols, RegExpr::directiveEqu)) {
				string label = symbols[1];
				stringstream s(symbols[2]);
				int num;
				s >> num;

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