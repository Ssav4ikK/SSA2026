#pragma once
#include "LIBS.h"

namespace CPP
{
    struct Cpp
    {
        wchar_t CPPfile[PARM_MAX_SIZE];
        std::ofstream* stream = nullptr;
    };

    const Cpp INITCPP = { L"", nullptr };

    Cpp GetCPP(wchar_t CPPFile[]);
    void WriteLine(Cpp& cpp, const char* text, ...);
    void WriteLine(Cpp& cpp, const wchar_t* text, ...);
    void WriteCPP(Cpp& cpp);
    void Close(Cpp& cpp);
}
