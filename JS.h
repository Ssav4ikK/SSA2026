#pragma once
#include "LIBS.h"

namespace JS
{
	struct Js
	{
		wchar_t JSfile[PARM_MAX_SIZE];
		std::ofstream* stream = nullptr;
	};

	const Js INITJS = { L"", nullptr };

	Js GetJS(wchar_t JsFile[]);
	void WriteLine(Js& js, const char* ch, ...);
	void WriteLine(Js& js, const wchar_t* ch, ...);
	void WriteJS(Js& js);


	void Close(Js& log);
}