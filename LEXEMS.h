#pragma once

#define LEXEMA_FIXSIZE							5		
#define LEXTABLE_MAX_COUNT_OF_LINES				4096
#define LEXTABLE_IDENTIFICATORTABLE_NULLIDX		0xffffffff

//INT

#define LEX_INT8		"i8  "
#define LEX_INT16		"i16 "
#define LEX_INT32		"i32 "
#define LEX_INT64		"i64 "

//FLOAT

#define LEX_FLOAT32		"f32 "
#define LEX_FLOAT64		"f64 "

//BOOLEAN

#define LEX_BOOLEAN		"bool"

//STRING

#define LEX_STRING				"str "
#define LEX_FORMATED_STRING		"fstr"

//SYMBOL

#define LEX_SYMBOL		"symb"

//PROCEDURE/RETURN

#define LEX_PROCEDURE	"proc"
#define LEX_BACK		"back"

//CONSTRUCTIONS

#define LEX_IF			"if  "
#define LEX_ELSE        "else"
#define LEX_ELIF		"elif"
#define LEX_SWITCH      "swch"
#define LEX_DEFAULT		"deft"
#define LEX_CASE		"case"
#define LEX_BREAK		"brek"

//CYCLES

#define LEX_FOR			"for "
#define LEX_IN			"in  "
#define LEX_DO			"do  "
#define LEX_WHILE		"whle"

//SEPARATORS

#define LEX_SEMICOLON				";   "
#define LEX_COMMA					",   "

#define LEX_LEFT_FIGURE_BRACKET		"{   "
#define LEX_RIGHT_FIGURE_BRACKET	"}   "

#define LEX_LEFT_BRACKET			"(   "
#define LEX_RIGHT_BRACKET			")   "

#define LEX_LEFT_SQUARE_BRACKET		"[   "
#define LEX_RIGHT_SQUARE_BRACKET	"]   "

#define LEX_DOUBLEPOINT				":   "

//OPERATORS

#define LEX_PLUS		"+   "
#define LEX_MINUS		"-   "
#define LEX_STAR		"*   "
#define LEX_DIRSLASH	"/   "
#define LEX_EQUAL		"=   "
#define LEX_EXCLAMATORY "!   "
#define LEX_CUBE		"^2  "
#define LEX_LESS		"<   "
#define LEX_GREATER		">   "
#define LEX_LESSEQ		"<=  "
#define LEX_GREATEREQ   ">=  "
#define LEX_EQUAL_EQUAL "==  "
#define LEX_NOTEQUAL	"!=  "

//BASE FUNC

#define LEX_PRINT		"prn "
#define LEX_PRINTF		"prnf"
#define LEX_INPUT		"inpt"

//OTHER

#define LEX_LITERAL			"lter"
#define LEX_IDENTIFICATOR	"idfr"
#define LEX_MAIN			"main"
#define LEX_SSA2026			"2026"
#define LEX_END				"end "
#define LEX_START			"strt"

//CUSTOM

#define LEX_NUMBER			"numb"
#define LEX_HELP			"help"
#define LEX_ARRAY			"arr "

namespace LEXTABLE
{
	struct Entry
	{
		char lexema[LEXEMA_FIXSIZE];
		int str_number;
		int indexIT;
	};

	struct LexTable
	{
		int maxsize;
		int current_size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int str_num);
	void Delete(LexTable& lextable);
}