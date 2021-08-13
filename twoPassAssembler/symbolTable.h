#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <string>
#include "TableEntry.h"

using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();



private:
	vector<TableEntry> table;
	int id = 0;
};



SymbolTable::SymbolTable()
{

}

SymbolTable::~SymbolTable()
{
}

#endif

