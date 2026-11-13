#include "LIBS.h"


namespace PARMS
{
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

			wchar_t* ptr_in_parm = wcsstr(argv[i], PARM_IN);																																					//поиск подстроки параметриа in
			wchar_t* ptr_out_parm = wcsstr(argv[i], PARM_OUT);																																					//поиск подстроки параметра out
			wchar_t* ptr_log_parm = wcsstr(argv[i], PARM_LOG);																																					//поиск подстроки параметра log

			if (ptr_in_parm == argv[i]) {
				wchar_t* value = argv[i] + wcslen(PARM_IN);
				if (*value == L':') {
					value++;
				}
				wcscpy_s(parm.in, value);
				in_parm_found = true;
			}
			else if (ptr_out_parm == argv[i]) {
				wchar_t* value = argv[i] + wcslen(PARM_OUT);
				if (*value == L':') {
					value++;
				}
				wcscpy_s(parm.out, value);
			}
			else if (ptr_log_parm == argv[i]) {
				wchar_t* value = argv[i] + wcslen(PARM_LOG);
				if (*value == L':') {
					value++;
				}
				wcscpy_s(parm.log, value);
			}
		}

		if (!in_parm_found) {
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
