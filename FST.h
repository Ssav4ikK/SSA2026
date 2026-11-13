#pragma once
#include "LIBS.h"

namespace FST 
{
	struct Relation
	{
		char symbol;
		short num_node;
		
		Relation(char ch = 0x00, short new_state = NULL);
	};

	struct Node
	{
		short num_relations;
		Relation* relations;

		Node();
		Node(short count_relations, Relation relations, ...);
	};

	struct Fst
	{
		char* string;
		short current_position;
		short count_states;
		Node* nodes;
		short* possible_states;

		Fst(char* string, short count_states, Node n, ...);
		~Fst();
	};

	bool execute(Fst& fst);
	bool execute(Fst& fst, short finalState);
	std::string match(char* str);

	//INT
	bool isInt8(char* str);
	bool isInt16(char* str);
	bool isInt32(char* str);
	bool isInt64(char* str);

	// FLOAT
	bool isFloat32(char* str);
	bool isFloat64(char* str);

	// STRING, SYMBOL
	bool isSymbol(char* str);
	bool isString(char* str);

	// BOOLEAN
	bool isBoolean(char* str);

	// NUMBER, HELP
	bool isNumber(char* str);
	bool isHelp(char* str);

	// ARRAY
	bool isArray(char* str);

	// PROCEDURES
	bool isProcedure(char* str);

	// START, END
	bool isStart(char* str);
	bool isEnd(char* str);

	// BACK
	bool isBack(char* str);

	// MAIN
	bool isMain(char* str);

	// BASIC FUNCTIONS
	bool isPrint(char* str);
	bool isPrintF(char* str);
	bool isInput(char* str);

	// CONSTRUCTIONS
	bool isSwitch(char* str);
	bool isCase(char* str);
	bool isBreak(char* str);
	bool isDefault(char* str);
	bool isIf(char* str);
	bool isElif(char* str);
	bool isElse(char* str);

	// FOR
	bool isFor(char* str);
	bool isIn(char* str);

	// WHILE, DO
	bool isDo(char* str);
	bool isWhile(char* str);

	// IDENTIFICATORS, LITERALS
	bool isIdentifier(char* str);
	bool isIntegerLiteral(char* str);
	bool isFloatLiteral(char* str);
	bool isBooleanLiteral(char* str);
	bool isSymbolLiteral(char* str);
	bool isStringLiteral(char* str);
	bool isFormatedStringLiteral(char* str);
	bool isSSA(char* str);
}