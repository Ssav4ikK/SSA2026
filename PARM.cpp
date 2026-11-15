#include "LIBS.h"

namespace PARMS
{
    static bool hasPrefix(const wchar_t* arg, const wchar_t* prefix)
    {
        size_t length = wcslen(prefix);
        return wcsncmp(arg, prefix, length) == 0 &&
            (arg[length] == L'\0' || arg[length] == L':');
    }

    wchar_t* removeExtension(const wchar_t* str, const wchar_t* to_remove)
    {
        if (str == nullptr)
            return nullptr;

        size_t str_length = wcslen(str);
        wchar_t* new_str = new wchar_t[str_length + 1];
        wmemcpy(new_str, str, str_length + 1);

        if (to_remove != nullptr)
        {
            wchar_t* position = wcsstr(new_str, to_remove);

            if (position != nullptr)
            {
                size_t remove_length = wcslen(to_remove);
                wmemmove(position, position + remove_length,
                    wcslen(position + remove_length) + 1);
            }
        }

        return new_str;
    }

    static const wchar_t* getOutputExtension(const wchar_t* language)
    {
        if (wcscmp(language, LANG_JS) == 0)
            return JS_EXTANCION;
        if (wcscmp(language, LANG_CPP) == 0)
            return CPP_EXTANCION;
        if (wcscmp(language, LANG_CS) == 0)
            return C_SHARP_EXTANCION;
        if (wcscmp(language, LANG_ASM) == 0)
            return ASSEMBLER_EXTANCION;

        return OUT_EXTANCION;
    }

    static bool isSupportedLanguage(const wchar_t* language)
    {
        return wcscmp(language, LANG_JS) == 0 ||
            wcscmp(language, LANG_CPP) == 0 ||
            wcscmp(language, LANG_CS) == 0 ||
            wcscmp(language, LANG_ASM) == 0;
    }

    Parm getparm(int argc, wchar_t* argv[])
    {
        Parm parm = { 0 };
        bool in_parm_found = false;
        bool lang_parm_found = false;

        for (int i = 1; i < argc; ++i)
        {
            if (wcslen(argv[i]) >= PARM_MAX_SIZE)
                throw ERROR_THROW(104);

            if (hasPrefix(argv[i], PARM_IN))
            {
                wchar_t* value = argv[i] + wcslen(PARM_IN);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.in, value);
                in_parm_found = true;
            }
            else if (hasPrefix(argv[i], PARM_OUT))
            {
                wchar_t* value = argv[i] + wcslen(PARM_OUT);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.out, value);
            }
            else if (hasPrefix(argv[i], PARM_LOG))
            {
                wchar_t* value = argv[i] + wcslen(PARM_LOG);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.log, value);
            }
            else if (hasPrefix(argv[i], PARM_LANG))
            {
                wchar_t* value = argv[i] + wcslen(PARM_LANG);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.lang, value);
                lang_parm_found = true;
            }
            else if (hasPrefix(argv[i], PARM_MFST))
            {
                wchar_t* value = argv[i] + wcslen(PARM_MFST);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.mfst, value);
            }
            else if (hasPrefix(argv[i], PARM_AST))
            {
                wchar_t* value = argv[i] + wcslen(PARM_AST);
                if (*value == L':')
                    ++value;

                wcscpy_s(parm.ast, value);
            }
        }

        if (!in_parm_found || wcslen(parm.in) == 0)
            throw ERROR_THROW(100);

        if (!lang_parm_found || !isSupportedLanguage(parm.lang))
            throw ERROR_THROW(101);

        wchar_t* base_name = removeExtension(parm.in, SSA_EXTANCION);

        if (wcslen(parm.out) == 0)
        {
            wcscpy_s(parm.out, base_name);
            wcscat_s(parm.out, getOutputExtension(parm.lang));
        }

        if (wcslen(parm.log) == 0)
        {
            wcscpy_s(parm.log, base_name);
            wcscat_s(parm.log, LOG_EXTANCION);
        }

        if (wcslen(parm.mfst) == 0)
        {
            wcscpy_s(parm.mfst, base_name);
            wcscat_s(parm.mfst, MFST_EXTANCION);
        }

        if (wcslen(parm.ast) == 0)
        {
            wcscpy_s(parm.ast, base_name);
            wcscat_s(parm.ast, AST_EXTANCION);
        }

        delete[] base_name;
        return parm;
    }
}
