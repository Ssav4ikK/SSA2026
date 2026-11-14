#include "LIBS.h"

namespace LOG
{
	Log GetLog(wchar_t LogFile[])
	{
		Log log;
		wcscpy_s(log.logfile, LogFile);
		log.stream = new std::ofstream(LogFile, std::ios::binary);

		if (!log.stream->is_open()) {
			delete log.stream;
			log.stream = nullptr;
			throw ERROR_THROW(112);
		}

		return log;
	}

	void WriteLine(Log& log, const char* ch, ...) {
		if (!log.stream) {
			return;
		}
		va_list args;
		va_start(args, ch);
		const char* ch_ptr = ch;

		while (ch_ptr && strlen(ch_ptr) > 0) {
			*(log.stream) << ch_ptr;
			ch_ptr = va_arg(args, const char*);
		}

		va_end(args);
		*(log.stream) << "\r\n";
	}

	void WriteLine(Log& log, const wchar_t* ch, ...) {
		if (!log.stream) {
			return;
		}
		va_list args;
		va_start(args, ch);
		const wchar_t* ch_ptr = ch;
		char buffer[512];

		while (ch_ptr && wcslen(ch_ptr) > 0) {
			size_t converted;
			wcstombs_s(&converted, buffer, sizeof(buffer), ch_ptr, _TRUNCATE);
			*(log.stream) << buffer;
			ch_ptr = va_arg(args, const wchar_t*);
		}

		va_end(args);
		*(log.stream) << "\r\n";
	}

	void WriteLog(Log& log) {
		if (!log.stream)
		{
			return;
		}
		time_t _time = time(nullptr);
		tm time_str;
		localtime_s(&time_str, &_time);
		char time_buffer[256];
		strftime(time_buffer, sizeof(time_buffer), "%d.%m.%Y %H:%M:%S", &time_str);

		*(log.stream) << "Working protocol of translator\r\n";
		*(log.stream) << "Creation time: " << time_buffer << "\r\n\r\n";
	}

	void WriteParm(Log& log, PARMS::Parm parm)
	{
		if (!log.stream) return;
		char buffer[PARM_MAX_SIZE];
		size_t converted;

		*(log.stream) << "PARAMETERS\r\n";

		wcstombs_s(&converted, buffer, sizeof(buffer), parm.in, _TRUNCATE);
		*(log.stream) << "IN: " << buffer << "\r\n";

		wcstombs_s(&converted, buffer, sizeof(buffer), parm.out, _TRUNCATE);
		*(log.stream) << "OUT: " << buffer << "\r\n";

		wcstombs_s(&converted, buffer, sizeof(buffer), parm.log, _TRUNCATE);
		*(log.stream) << "LOG: " << buffer << "\r\n";
	}

	void WriteIn(Log& log, IN::In in) {
		if (!log.stream) return;
		*(log.stream) << "\r\nRESULTS OF CODE\r\n";
		*(log.stream) << "Count of lines:      " << in.lines << "\r\n";
		*(log.stream) << "Count of symbols:    " << in.size << "\r\n";
		*(log.stream) << "Count of ignored:    " << in.ignore << "\r\n";
	}

	void WriteError(Log& log, ERROR::Error error)
	{
		if (log.stream && log.stream->is_open())
		{
			*(log.stream) << "ERROR\r\n";
			*(log.stream) << "Error code:    " << error.id << "\r\n";
			*(log.stream) << "Message:       " << error.message << "\r\n";

			if (error.place.line > 0)
			{
				*(log.stream) << "Line:       " << error.place.line << "\r\n";
				*(log.stream) << "Position:     " << error.place.col << "\r\n";
			}
		}
		else
		{
			std::cout << "ERROR\r\n";
			std::cout << "Error code:    " << error.id << "\r\n";
			std::cout << "Message:       " << error.message << "\r\n";

			if (error.place.line > 0)
			{
				std::cout << "Line:       " << error.place.line << "\r\n";
				std::cout << "Position:     " << error.place.col << "\r\n";
			}
		}
	}

	void Close(Log& log) {
		if (log.stream)
		{
			log.stream->close();
			delete log.stream;
			log.stream = nullptr;
		}
	}
}