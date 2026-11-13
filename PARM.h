#pragma once
#include "LIBS.h"

#define PARM_IN L"-in"
#define PARM_OUT L"-out"
#define PARM_LOG L"-log"
#define PARM_MAX_SIZE 300

#define OUT_EXTANCION L".out"
#define LOG_EXTANCION L".log"

namespace PARMS
{
	struct Parm {
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
	};

	Parm getparm(int argc, wchar_t* argv[]);
}