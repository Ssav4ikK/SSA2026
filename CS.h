#pragma once
#include "LIBS.h"

namespace CS
{
    struct Cs
    {
        wchar_t CSfile[PARM_MAX_SIZE];
        std::ofstream* stream = nullptr;
    };

    const Cs INITCS = { L"", nullptr };

    Cs GetCS(wchar_t CSFile[]);
    void WriteLine(Cs& cs, const char* text, ...);
    void WriteLine(Cs& cs, const wchar_t* text, ...);
    void WriteCS(Cs& cs);
    void Close(Cs& cs);
}
