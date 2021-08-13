#include <iostream>

#include "assembler.h"
#include "assemblerException.h"
#include "lex.h"

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
	}
}