#pragma once
#include "LIBS.h"

namespace ASM
{
    struct Asm
    {
        wchar_t ASMfile[PARM_MAX_SIZE];
        std::ofstream* stream = nullptr;
    };

    const Asm INITASM = { L"", nullptr };

    Asm GetASM(wchar_t ASMFile[]);
    void WriteLine(Asm& asm_file, const char* text, ...);
    void WriteLine(Asm& asm_file, const wchar_t* text, ...);
    void WriteASM(Asm& asm_file);
    void Close(Asm& asm_file);
}
