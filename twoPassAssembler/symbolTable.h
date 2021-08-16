#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "TableEntry.h"
#include "assemblerException.h"


using namespace std;

class MySymbolTable
{
public:
	MySymbolTable();
	~MySymbolTable();

	void insertNonSection(string label, int section, int value,
						char visibility = 'l', bool isExt = false, bool isAbs = false);

	int insertSection(string label, int value);

	//when we reach end of a section we need to update its size in the table
	void updateSectionSize(int entryNum, int size);
	
	//will declare symbol if it does not exist in table or just change visibility to global if the symbol is already defined
	void changeVisibilityToGlobal(string label);
	
	//save symbol if it has not been already defined or declared as extern so that we can check if there are some unknown symbols 
	void markAsUsed(string label);


private:
	unordered_map<string, int> enteredSymbols; //used for quick access to elements in the table. key -> label, value -> row index in table that is slit in half: if the value is negative than the symbol is in the section symbols part of the table and if positive and zero than in the non-section part
	unordered_set<string> usedSymbols;//this should be empty after the first pass, if not then we have some unknown symbols
	vector<TableEntry> table;
	int sectionId = 0;
	int lastSectionIndex = 0; //last row with section in the symbol table

	bool canBeDeclaredGlobal(TableEntry& entry);

};

#endif

