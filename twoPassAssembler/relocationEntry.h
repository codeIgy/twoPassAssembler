#ifndef RELOCATIONENTRY_H
#define RELOCATIONENTRY_H

#include <string>

struct RelocationEntry {
	static enum type{R_386_32 , R_386_PC32};
	RelocationEntry(int offset, type relType, int ordinal) : offset(offset), relType(relType), ordinal(ordinal) {}
	
	int offset;
	type relType;
	int ordinal;
};

#endif // !RELOCATIONENTRY_H

