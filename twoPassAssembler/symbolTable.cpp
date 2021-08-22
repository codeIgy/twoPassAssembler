#include "SymbolTable.h"
#include <iomanip>
void MySymbolTable::insertNonSection(string label, int section, int value,
	char visibility, bool isExt, bool isAbs) {

	if (usedSymbols.find(label) != usedSymbols.end()) {
		usedSymbols.erase(label);
	}

	if (enteredSymbols.find(label) != enteredSymbols.end()) { //check for duplicates
		int index = enteredSymbols[label];

		if (index < 0) {//meaning the symbol is an section
			throw "Symbol has been alredy declared as a section!";
		}

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

		enteredSymbols[label] = (int)table.size() - sectionId; //save index of the element it possesses in the non-section part of the table
		table.push_back(entry);
	}
}

int MySymbolTable::insertSection(string label, int value) {

	if (usedSymbols.find(label) != usedSymbols.end()) {
		usedSymbols.erase(label);
	}

	if (enteredSymbols.find(label) != enteredSymbols.end()) {
		throw AssemblerException("Symbol has already been declared!");
	}

	TableEntry entry;

	entry.id = sectionId;
	entry.label = label;
	entry.section = sectionId;
	entry.value = 0;
	entry.visibility = 'l';
	entry.isExt = false;

	enteredSymbols[label] = -sectionId; //negative so that we know that this symbol is in the section part of the table
	table.insert(table.begin() + sectionId, entry);

	return sectionId++;

}

void MySymbolTable::updateSectionSize(int entryNum, int size)
{
	table[entryNum].size = size;
}

void MySymbolTable::changeVisibilityToGlobal(string label)
{
	if (enteredSymbols.find(label) != enteredSymbols.end()) { //check for symbol
		int index = enteredSymbols[label];
		TableEntry& entry2 = table[sectionId + index];
		if (canBeDeclaredGlobal(entry2)) {
			entry2.visibility = 'g';
		}
		else {
			throw AssemblerException("Can't be declared global!");
		}
	}
	else {
		insertNonSection(label, 0, 0, 'g');
	}
}

void MySymbolTable::markAsUsed(string label)
{
	if (enteredSymbols.find(label) == enteredSymbols.end()) {
		usedSymbols.insert(label);
	}
}

void MySymbolTable::clearTable()
{
	enteredSymbols.clear();
	usedSymbols.clear();

	//erase every table row except the first 2 which represent undefined and absolute section
	while (table.size() > 2) {
		table.erase(table.begin() + 2);
	}

	sectionId = 2;
}

bool MySymbolTable::areAllSymbolsKnown()
{
	return  usedSymbols.empty();
}

void MySymbolTable::setOrdinals()
{
	for (auto it = table.begin() + sectionId; it != table.end(); it++) {
		it->id = sectionId++;
	}
}

TableEntry& MySymbolTable::getSymbol(string name)
{
	int index = enteredSymbols.at(name);

	if (index < 0) {
		index = -index;
	}

	return table[index];
}

unordered_set<string> MySymbolTable::getUnknownUsedSymbols()
{
	return usedSymbols;
}

void MySymbolTable::printSymbolTable(ofstream & outputFileTxt, ofstream & outputFileBinary)
{
	outputFileTxt << setw(10) << left << hex << "id" << setw(12) << "name" << setw(10) << "section" << "value" << "visibility" << "is external" << "size" << endl;

	for (auto entry : table) {
		outputFileTxt << setw(10) << left << hex << entry.id << setw(12) << entry.label << setw(10) << entry.section << entry.value << entry.visibility << entry.isExt << entry.size << endl;
	}
}

bool MySymbolTable::canBeDeclaredGlobal(TableEntry& entry)
{
	return !entry.isExt && entry.id == -1 && entry.section != 0 && entry.section != 1 && entry.visibility == 'l'; //id == -1 means that the symbol has yet to receive an id and that it is not a section
}

MySymbolTable::MySymbolTable()
{
	//insert undefined and absolute section
	table.push_back(TableEntry(sectionId++, "", 0, 0, 'l'));//undefined
	table.push_back(TableEntry(sectionId++, "", 1, 0, 'l'));//absolute
}

MySymbolTable::~MySymbolTable()
{
}