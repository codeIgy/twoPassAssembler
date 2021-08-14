#include "lex.h"

regex RegExpr::directiveGlobal("^\\s*\\.global\\s+([A-Za-z_]\\w*)(\\s*,\\s*([A-Za-z_]\\w*))*\\s*$");
regex RegExpr::directiveExtern("^\\s*\\.extern\\s+[A-Za-z_]\\w*(\\s*,\\s*[A-Za-z_]\\w*)*\\s*$");
regex RegExpr::directiveSection("^\\s*\\.section\\s+[A-Za-z_]\\w*\\s*$");
regex RegExpr::directiveWord("^\\s*\\.word\\s+([A-Za-z_]\\w*|(0 | -?[1-9]\\d* | 0[xX][a-fA-F\\d]+))(\\s*,\\s*([A-Za-z_]\\w*|(0 | -?[1-9]\\d* | 0[xX][a-fA-F\\d]+)))*\\s*$");
regex RegExpr::directiveSkip("^\\s*\\.skip\\s+(0 | [1-9]\\d* | 0[xX][a-fA-F\\d]+)\\s*$");
regex RegExpr::directiveEqu("^\\s*\\.equ\\s+[A-Za-z_]\\w*\\s*,\\s*(0 | -?[1-9]\\d* | 0[xX][a-fA-F\\d]+)\\s*$");
regex RegExpr::directiveEnd("^\\s*\\.end\\s*$");

regex RegExpr::label("^\\s*([A-Za-z_]\\w*)\\s*:.*$");