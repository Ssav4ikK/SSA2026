#pragma once
#include "LIBS.h"

namespace LOG 
{
	struct Log
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream = nullptr;
	};

	const Log INITLOG = { L"", nullptr };

	Log GetLog(wchar_t LogFile[]);
	void WriteLine(Log& log, const char* ch, ...);
	void WriteLine(Log& log, const wchar_t* ch, ...);
	void WriteLog(Log& log);
	void WriteParm(Log& log, PARMS::Parm parm);
	void WriteIn(Log& log, IN::In in);
	void WriteError(Log& log, ERROR::Error error);
	void Close(Log& log);

}