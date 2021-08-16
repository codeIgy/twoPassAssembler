#include "lex.h"

regex RegExpr::directiveGlobal("^\\s*\\.global\\s+([A-Za-z_]\\w*)((\\s*,\\s*[A-Za-z_]\\w*)*)\\s*$");
regex RegExpr::directiveExtern("^\\s*\\.extern\\s+([A-Za-z_]\\w*)((\\s*,\\s*[A-Za-z_]\\w*)*)\\s*$");
regex RegExpr::directiveSection("^\\s*\\.section\\s+([A-Za-z_]\\w*)\\s*$");
regex RegExpr::directiveWord("^\\s*\\.word\\s+([A-Za-z_]\\w*|0|-?[1-9][0-9]*|0[xX][a-fA-F0-9]+)((\\s*,\\s*[A-Za-z_]\\w*|0|[1-9]\\d*|0[xX][a-fA-F0-9]+)*)\\s*$");
regex RegExpr::directiveSkip("^\\s*\\.skip\\s+(0|-?[1-9]\\d*|0[xX][a-fA-F0-9]+)\\s*$");
regex RegExpr::directiveEqu("^\\s*\\.equ\\s+([A-Za-z_]\\w*)\\s*,\\s*(0|-?[1-9]\\d*|0[xX][a-fA-F0-9]+)\\s*$");
regex RegExpr::directiveEnd("^\\s*\\.end\\s*$");

regex RegExpr::symbol("[A-Za-z_]\\w*");
regex RegExpr::value("0|-?[1-9]\\d*|0[xX][a-fA-F0-9]+");
regex RegExpr::hex("0[xX][a-fA-F0-9]+");

regex RegExpr::label("^\\s*([A-Za-z_]\\w*)\\s*:.*$");


 regex RegExpr::halt("^\\s*halt\\s*$");
 regex RegExpr::interrupt("^\\s*int\\s+(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::iret("^\\s*iret\\s*$");
 regex RegExpr::callFixed("^\\s*call\\s+");
 regex RegExpr::ret("^\\s*ret\\s*$");
 regex RegExpr::jmpFixed("^\\s*jump\\s+");
 regex RegExpr::jeqFixed("^\\s*jeq\\s+");
 regex RegExpr::jneFixed("^\\s*jne\\s+");
 regex RegExpr::jgtFixed("^\\s*jgt\\s+");
 regex RegExpr::push("^\\s*push\\s+(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::pop("^\\s*pop\\s+(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::xchg("^\\s*xchg\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::add("^\\s*add\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::sub("^\\s*sub\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::mul("^\\s*mul\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::div("^\\s*div\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::cmp("^\\s*cmp\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::logicNot("^\\s*not\\s+(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::logicAnd("^\\s*and\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::logicOr("^\\s*or\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::logicXor("^\\s*xor\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::test("^\\s*test\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::shl("^\\s*shl\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::shr("^\\s*shr\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*(r[0-7]|sp|pc|psw)\\s*$");
 regex RegExpr::ldrFixed("^\\s*ldr\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*");
 regex RegExpr::strFixed("^\\s*str\\s+(r[0-7]|sp|pc|psw)\\s*,\\s*");