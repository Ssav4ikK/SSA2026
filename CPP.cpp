#include "LIBS.h"

namespace CPP
{
    Cpp GetCPP(wchar_t CPPFile[])
    {
        Cpp cpp;
        wcscpy_s(cpp.CPPfile, CPPFile);
        cpp.stream = new std::ofstream(CPPFile, std::ios::binary);

        if (!cpp.stream->is_open())
        {
            delete cpp.stream;
            cpp.stream = nullptr;
            throw ERROR_THROW(200);
        }

        return cpp;
    }

    void WriteLine(Cpp& cpp, const char* text, ...)
    {
        if (!cpp.stream)
            return;

        va_list args;
        va_start(args, text);
        const char* current = text;

        while (current && strlen(current) > 0)
        {
            *(cpp.stream) << current;
            current = va_arg(args, const char*);
        }

        va_end(args);
        *(cpp.stream) << "\r\n";
    }

    void WriteLine(Cpp& cpp, const wchar_t* text, ...)
    {
        if (!cpp.stream)
            return;

        va_list args;
        va_start(args, text);
        const wchar_t* current = text;
        char buffer[512];

        while (current && wcslen(current) > 0)
        {
            size_t converted;
            wcstombs_s(&converted, buffer, sizeof(buffer), current, _TRUNCATE);
            *(cpp.stream) << buffer;
            current = va_arg(args, const wchar_t*);
        }

        va_end(args);
        *(cpp.stream) << "\r\n";
    }

    void WriteCPP(Cpp& cpp)
    {
        if (!cpp.stream)
            return;

        *(cpp.stream) << "// SSA2026 TO C++\r\n\r\n";
    }

    void Close(Cpp& cpp)
    {
        if (cpp.stream)
        {
            cpp.stream->close();
            delete cpp.stream;
            cpp.stream = nullptr;
        }
    }
}
