#ifndef LEX_H
#define LEX_H
#include <regex>

using namespace std;

class RegExpr 
{
public:
	//directives
	static regex directiveGlobal;
	static regex directiveExtern;
	static regex directiveSection;
	static regex directiveWord;
	static regex directiveSkip;
	static regex directiveEqu;
	static regex directiveEnd;

	//symbol and number extraction
	static regex symbol;
	static regex value;
	static regex hex;

	//label regex
	static regex label;

};

#endif

