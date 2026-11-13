#include "LIBS.h"

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "RU");
	LOG::Log log = LOG::INITLOG;

	try {
		PARMS::Parm param = PARMS::getparm(argc, argv);
		log = LOG::GetLog(param.log);

		LOG::WriteLog(log);
		LOG::WriteParm(log, param);

		IN::In in = IN::getIn(param.in);
		LOG::WriteIn(log, in);

		LEXER::Lexer lex = LEXER::Analyze(in);
		LEXER::PrintLexTable(log, lex.lextable);
		LEXER::PrintIdTable(log, lex.idtable);

		std::ofstream OutFile(param.out, std::ios::binary);
		if (OutFile.is_open())
		{
			OutFile.write(in.text, in.size);
			OutFile.close();
		}
		LOG::Close(log);
		std::cout << "Execution success. Protocol was created.\n";
		delete[] in.text;
	}
	catch (ERROR::Error error) {
		std::cout << "Execution failed.\n";
		LOG::WriteError(log, error);
		LOG::Close(log);
	}
	return 0;
}