#pragma once
#include "LIBS.h"

#define PARM_IN		L"-in"
#define PARM_OUT	L"-out"
#define PARM_LOG	L"-log"
#define PARM_LANG	L"-lang"
#define PARM_MFST	L"-mfst"
#define PARM_AST	L"-ast"

#define LANG_JS		L"js"
#define LANG_CPP	L"cpp"
#define LANG_CS		L"cs"
#define LANG_ASM	L"asm"


#define PARM_MAX_SIZE 300

#define SSA_EXTANCION				L".ssa" 

#define OUT_EXTANCION				L".out"
#define LOG_EXTANCION				L".log"

#define MFST_EXTANCION				L".mfst"
#define AST_EXTANCION				L".ast"
#define PARSING_TREE_EXTANCION		AST_EXTANCION

#define JS_EXTANCION				L".js"
#define ASSEMBLER_EXTANCION			L".asm"
#define CPP_EXTANCION				L".cpp"
#define C_SHARP_EXTANCION			L".cs"

namespace PARMS
{
	struct Parm {
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t lang[PARM_MAX_SIZE];
		wchar_t mfst[PARM_MAX_SIZE];
		wchar_t ast[PARM_MAX_SIZE];
	};

	Parm getparm(int argc, wchar_t* argv[]);
	static bool hasPrefix(const wchar_t* arg, const wchar_t* prefix);
	wchar_t* removeExtension(const wchar_t* str, const wchar_t* to_remove);
}