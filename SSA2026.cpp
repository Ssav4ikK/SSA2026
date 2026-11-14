#include "LIBS.h"

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "RU");
	LOG::Log log = LOG::INITLOG;

	IN::In in = {};
	LEXER::Lexer lex = {};

	try {
		PARMS::Parm param = PARMS::getparm(argc, argv);
		log = LOG::GetLog(param.log);

		LOG::WriteLog(log);
		LOG::WriteParm(log, param);

		in = IN::getIn(param.in);
		LOG::WriteIn(log, in);

		lex = LEXER::Analyze(in);
		LEXER::PrintLexTable(log, lex.lextable);
		LEXER::PrintIdTable(log, lex.idtable);

		std::ofstream OutFile(param.out, std::ios::binary);
		if (OutFile.is_open())
		{
			OutFile.write(in.text, in.size);
			OutFile.close();
		}

		std::cout << "Lexical analysis completed." << std::endl;
		std::cout << "Syntax analysis started." << std::endl;

		GRB::Greibach grammar = GRB::GetGreibach();
		MFST::Mfst parser(lex, grammar);

		LOG::WriteLine(log, "\r\nSYNTAX ANALYSIS", "");
		parser.setTraceLog(log);
		parser.writeTraceHeader();
		bool is_valid = parser.start();
		if (is_valid)
		{
			LOG::WriteLine(log, "Syntax analysis completed without errors.", "");
			parser.writeParseTree(log);
			std::cout << "\nSyntax analysis completed without errors." << std::endl;
		}
		else
		{
			LOG::WriteLine(log, "Syntax errors detected:", "");
			std::cout << "\nSyntax errors detected:" << std::endl;

			char buffer[DIAGNOSTIC_MESSAGE_SIZE];
			for (short i = 0; i < MAX_DIAGNOSTIC_MESSAGES; i++)
			{
				parser.getDiagnosticMessage(i, buffer);
				if (buffer[0] != '\0')
				{
					LOG::WriteLine(log, buffer, "");
					std::cout << "  " << buffer << std::endl;
				}
			}
		}
		std::cout << "Protocol was created." << std::endl;
	}
	catch (ERROR::Error error) {
		std::cout << "Execution failed.\n";
		LOG::WriteError(log, error);
	}
	catch (const std::exception& e) {
		std::cout << "Execution failed: " << e.what() << std::endl;
		if (log.stream) LOG::WriteLine(log, "Unexpected exception: ", e.what(), "");
	}
	catch (...) {
		std::cout << "Execution failed: unknown error" << std::endl;
	}

	delete[] in.text;
	LEXTABLE::Delete(lex.lextable);
	IDTABLE::Delete(lex.idtable);
	LOG::Close(log);

	return 0;
}
