#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>

using namespace std;

struct TableEntry
{
public:
	TableEntry(int id, string label, int section, int value, char visibility, bool isExt, bool isAbs);
	~TableEntry();

	int id;
	string label;//label name
	int section; //0 = undefined otherwise section id
	int value;//label value
	char visibility;//'l' for local and 'g' for global
	bool isExt;//whether symbol is extern
	bool isAbs;//whether symbol is absolute
};

TableEntry::TableEntry(int id, string label, int section, 
	int value, char visibility, bool isExt, bool isAbs) : id(id), label(label), section(section), 
					value(value), visibility(visibility), isExt(isExt), isAbs(isAbs){

}

#endif // !TABLEENTRY_H
