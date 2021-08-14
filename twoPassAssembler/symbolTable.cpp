#include "symbolTable.h"

void SymbolTable::insertNonSection(string label, int section, int value,
	char visibility='l', bool isExt=false, bool isAbs=false) {

	if (enteredSymbols.find(label) != enteredSymbols.end()) { //check for duplicates
		int index = enteredSymbols[label];
		TableEntry& entry2 = table[sectionId + index];
		if (entry2.visibility = 'g' && entry2.section == 0 && !entry2.isExt)//exception if this is a global symbol which needs to be corrected
		{
			entry2.section = section;
			entry2.value = value;
		}
		else {
			throw AssemblerException("Symbol has already been defined or is declared as an external symbol!");
		}
	}
	else {
		TableEntry entry;

		if (isAbs) {
			entry.id = -1;
			entry.label = label;
			entry.section = 1; //absolute symbols have section 1
			entry.value = value;
			entry.visibility = 'l';
			entry.isExt = false;
		}
		else if (isExt) {
			entry.id = -1;
			entry.label = label;
			entry.section = 0; //undefined
			entry.value = 0;
			entry.visibility = 'g';
			entry.isExt = true;
		}
		else {
			entry.id = -1;
			entry.label = label;
			entry.section = section; //undefined
			entry.value = value;
			entry.visibility = visibility;
			entry.isExt = false;
		}

		enteredSymbols[label] = table.size() - sectionId; //save index of the element it possesses in the non-section part of the table
		table.push_back(entry);
	}
}

void SymbolTable::insertSection(string label, int value) {

	if (enteredSymbols.find(label) != enteredSymbols.end()) {
		throw AssemblerException("Symbol has already been declared!");
	}

	TableEntry entry;

	entry.id = sectionId;
	entry.label = label;
	entry.section = sectionId; //undefined
	entry.value = 0;
	entry.visibility = 'l';
	entry.isExt = false;

	sectionId++;

	enteredSymbols[label] = -sectionId; //negative so that we know that this symbol is in the section part of the table
	table.insert(table.begin() + lastSectionIndex++, entry);

}