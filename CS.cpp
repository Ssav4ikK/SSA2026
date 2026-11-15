#include "LIBS.h"

namespace CS
{
    Cs GetCS(wchar_t CSFile[])
    {
        Cs cs;
        wcscpy_s(cs.CSfile, CSFile);
        cs.stream = new std::ofstream(CSFile, std::ios::binary);

        if (!cs.stream->is_open())
        {
            delete cs.stream;
            cs.stream = nullptr;
            throw ERROR_THROW(200);
        }

        return cs;
    }

    void WriteLine(Cs& cs, const char* text, ...)
    {
        if (!cs.stream)
            return;

        va_list args;
        va_start(args, text);
        const char* current = text;

        while (current && strlen(current) > 0)
        {
            *(cs.stream) << current;
            current = va_arg(args, const char*);
        }

        va_end(args);
        *(cs.stream) << "\r\n";
    }

    void WriteLine(Cs& cs, const wchar_t* text, ...)
    {
        if (!cs.stream)
            return;

        va_list args;
        va_start(args, text);
        const wchar_t* current = text;
        char buffer[512];

        while (current && wcslen(current) > 0)
        {
            size_t converted;
            wcstombs_s(&converted, buffer, sizeof(buffer), current, _TRUNCATE);
            *(cs.stream) << buffer;
            current = va_arg(args, const wchar_t*);
        }

        va_end(args);
        *(cs.stream) << "\r\n";
    }

    void WriteCS(Cs& cs)
    {
        if (!cs.stream)
            return;

        *(cs.stream) << "// SSA2026 TO C#\r\n\r\n";
    }

    void Close(Cs& cs)
    {
        if (cs.stream)
        {
            cs.stream->close();
            delete cs.stream;
            cs.stream = nullptr;
        }
    }
}
