#include "LIBS.h"

namespace JS
{
	Js GetJS(wchar_t JSFile[])
	{
		Js js;
		wcscpy_s(js.JSfile, JSFile);
		js.stream = new std::ofstream(JSFile, std::ios::binary);

		if (!js.stream->is_open()) {
			delete js.stream;
			js.stream = nullptr;
			throw ERROR_THROW(200);
		}

		return js;
	}

	void WriteLine(Js& js, const char* ch, ...) {
		if (!js.stream) {
			return;
		}
		va_list args;
		va_start(args, ch);
		const char* ch_ptr = ch;

		while (ch_ptr && strlen(ch_ptr) > 0) {
			*(js.stream) << ch_ptr;
			ch_ptr = va_arg(args, const char*);
		}

		va_end(args);
		*(js.stream) << "\r\n";
	}

	void WriteLine(Js& js, const wchar_t* ch, ...) {
		if (!js.stream) {
			return;
		}
		va_list args;
		va_start(args, ch);
		const wchar_t* ch_ptr = ch;
		char buffer[512];

		while (ch_ptr && wcslen(ch_ptr) > 0) {
			size_t converted;
			wcstombs_s(&converted, buffer, sizeof(buffer), ch_ptr, _TRUNCATE);
			*(js.stream) << buffer;
			ch_ptr = va_arg(args, const wchar_t*);
		}

		va_end(args);
		*(js.stream) << "\r\n";
	}

	void WriteJS(Js& js) {
		if (!js.stream)
		{
			return;
		}

		*(js.stream) << "//SSA2026 TO JS\r\n\n";
	}


	void Close(Js& js) {
		if (js.stream)
		{
			js.stream->close();
			delete js.stream;
			js.stream = nullptr;
		}
	}
}