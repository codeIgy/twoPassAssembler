#ifndef LEX_H
#define LEX_H
#include <regex>

using namespace std;

class RegExpr 
{
public:
	//directives
	static regex directiveGlobal("\\.global\\s+\\w(\\w|\\d)*(\\s+,\\s+\\w(\\w|\\d))*");
	static regex directiveExtern;
	static regex directiveSection;
	static regex directiveWord;
	static regex directiveSkip;
	static regex directiveEqu;
	static regex directiveEnd;

	//label regex

	static regex
};

#endif

