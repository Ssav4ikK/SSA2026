#include "LIBS.h"

namespace ASM
{
    Asm GetASM(wchar_t ASMFile[])
    {
        Asm asm_file;
        wcscpy_s(asm_file.ASMfile, ASMFile);
        asm_file.stream = new std::ofstream(ASMFile, std::ios::binary);

        if (!asm_file.stream->is_open())
        {
            delete asm_file.stream;
            asm_file.stream = nullptr;
            throw ERROR_THROW(200);
        }

        return asm_file;
    }

    void WriteLine(Asm& asm_file, const char* text, ...)
    {
        if (!asm_file.stream)
            return;

        va_list args;
        va_start(args, text);
        const char* current = text;

        while (current && strlen(current) > 0)
        {
            *(asm_file.stream) << current;
            current = va_arg(args, const char*);
        }

        va_end(args);
        *(asm_file.stream) << "\r\n";
    }

    void WriteLine(Asm& asm_file, const wchar_t* text, ...)
    {
        if (!asm_file.stream)
            return;

        va_list args;
        va_start(args, text);
        const wchar_t* current = text;
        char buffer[512];

        while (current && wcslen(current) > 0)
        {
            size_t converted;
            wcstombs_s(&converted, buffer, sizeof(buffer), current, _TRUNCATE);
            *(asm_file.stream) << buffer;
            current = va_arg(args, const wchar_t*);
        }

        va_end(args);
        *(asm_file.stream) << "\r\n";
    }

    void WriteASM(Asm& asm_file)
    {
        if (!asm_file.stream)
            return;

        *(asm_file.stream) << "; SSA2026 TO ASM\r\n\r\n";
    }

    void Close(Asm& asm_file)
    {
        if (asm_file.stream)
        {
            asm_file.stream->close();
            delete asm_file.stream;
            asm_file.stream = nullptr;
        }
    }
}
