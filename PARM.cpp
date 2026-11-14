#include "LIBS.h"

namespace PARMS
{

	static bool hasPrefix(const wchar_t* arg, const wchar_t* prefix)
	{
		size_t n = wcslen(prefix);
		return wcsncmp(arg, prefix, n) == 0 && (arg[n] == L'\0' || arg[n] == L':');
	}

	Parm getparm(int argc, wchar_t* argv[])
	{
		Parm parm = { 0 };
		bool in_parm_found = false;

		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(104);
			}

			if (hasPrefix(argv[i], PARM_IN)) {
				wchar_t* value = argv[i] + wcslen(PARM_IN);
				if (*value == L':') value++;
				wcscpy_s(parm.in, value);
				in_parm_found = true;
			}
			else if (hasPrefix(argv[i], PARM_OUT)) {
				wchar_t* value = argv[i] + wcslen(PARM_OUT);
				if (*value == L':') value++;
				wcscpy_s(parm.out, value);
			}
			else if (hasPrefix(argv[i], PARM_LOG)) {
				wchar_t* value = argv[i] + wcslen(PARM_LOG);
				if (*value == L':') value++;
				wcscpy_s(parm.log, value);
			}
		}

		if (!in_parm_found || wcslen(parm.in) == 0) {
			throw ERROR_THROW(100);
		}

		if (wcslen(parm.out) == 0) {
			wcscpy_s(parm.out, parm.in);
			wcscat_s(parm.out, OUT_EXTANCION);
		}

		if (wcslen(parm.log) == 0) {
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, LOG_EXTANCION);
		}

		return parm;
	}
}
