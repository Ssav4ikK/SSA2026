#pragma once
#include "LIBS.h"

namespace LEXER
{
	struct Lexer {
		LEXTABLE::LexTable lextable;
		IDTABLE::IdTable idtable;
	};

	Lexer Analyze(IN::In in);
	void PrintLexTable(LOG::Log log, LEXTABLE::LexTable lextable);
	void PrintIdTable(LOG::Log log, IDTABLE::IdTable idtable);
}