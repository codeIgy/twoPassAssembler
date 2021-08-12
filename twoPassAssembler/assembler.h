#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <string>

class Assembler
{
public:
	Assembler();
	~Assembler();
	void assemble(ifstream& inputFile);

private:
	void passFirstTime(ifstream& inputFile);
	void passSecondTime();
};
#endif
