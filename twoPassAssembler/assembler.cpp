#include <iostream>
#include "assembler.h"
#include "assemblerException.h"
Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}

void Assembler::assemble(ifstream& inputFile) {
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

}