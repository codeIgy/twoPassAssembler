#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "TableEntry.h"
#include "assemblerException.h"


using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	void insertNonSection(string label, int section, int value,
						char visibility = 'l', bool isExt = false, bool isAbs = false);

	int insertSection(string label, int value);

	//when we reach end of a section we need to update its size in the table
	void updateSectionSize(int entryNum, int size);

	void changeVisibilityToGlobal(string label);


private:
	unordered_map<string, int> enteredSymbols; //used for quick access to elements in the table. key -> label, value -> row index in table that is slit in half: if the value is negative than the symbol is in the section symbols part of the table and if positive and zero than in the non-section part
	vector<TableEntry> table;
	int sectionId = 0;
	int lastSectionIndex = 0; //last row with section in the symbol table

	bool canBeDeclaredGlobal(TableEntry& entry);

};



SymbolTable::SymbolTable()
{
	//insert undefined and absolute section
	table.push_back(TableEntry(sectionId++,"", 0, 0, 'l'));//undefined
	table.push_back(TableEntry(sectionId++, "", 1, 0, 'l'));//absolute
}

SymbolTable::~SymbolTable()
{
}

#endif

