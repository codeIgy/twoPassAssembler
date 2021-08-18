#include <iostream>
#include <unordered_set>

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
		table.clearTable();

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
				table.insertNonSection(newLabel, sectionIndex ? sectionIndex : 0, locationCounter, 'l');
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

void Assembler::passSecondTime()
{
}