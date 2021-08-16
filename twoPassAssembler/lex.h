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

	//assembly instructions
	static regex halt;
	static regex interrupt;
	static regex iret;
	static regex callFixed;
	static regex ret;
	static regex jmpFixed;
	static regex jeqFixed;
	static regex jneFixed;
	static regex jgtFixed;
	static regex push;
	static regex pop;
	static regex xchg;
	static regex add;
	static regex sub;
	static regex mul;
	static regex div;
	static regex cmp;
	static regex logicNot;
	static regex logicAnd;
	static regex logicOr;
	static regex logicXor;
	static regex test;
	static regex shl;
	static regex shr;
	static regex ldrFixed;
	static regex strFixed;
};

#endif

