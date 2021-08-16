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