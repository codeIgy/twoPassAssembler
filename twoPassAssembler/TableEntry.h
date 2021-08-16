#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>

using namespace std;

struct TableEntry
{
public:
	TableEntry(){}
	TableEntry(int id, string label, int section, int value, char visibility, bool isExt = false) : id(id), label(label), section(section),
		value(value), visibility(visibility), isExt(isExt) {}

	~TableEntry(){}

	int id;
	string label;//label name
	int section; //0 = undefined, 1 = absolute symbol, otherwise section id
	int value;//label value
	char visibility;//'l' for local and 'g' for global
	bool isExt;//whether symbol is extern
	int size;//section size
};
#endif // !TABLEENTRY_H
