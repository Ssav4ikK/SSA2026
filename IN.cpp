#include "LIBS.h"

int default_code[256];

void table_initialization()
{
	static bool initialized = false;
	if (initialized) {
		return;
	}
	initialized = true;
	static const int temp_table[256] = IN_CODE_TABLE;
	for (int i = 0; i < 256; i++) {
		default_code[i] = temp_table[i];
	}
}

namespace IN 
{
	In getIn(wchar_t InFile[]) {
		table_initialization();
		In in;

		for (int i = 0; i < 256; ++i) {
			in.code[i] = default_code[i];
		}
		in.size = 0;
		in.lines = 0;
		in.ignore = 0;
		in.text = new char[IN_MAX_LENGTH_TEXT];

		if (!in.text) {
			throw ERROR_THROW(110);
		}

		std::ifstream file(InFile, std::ios::binary);

		if (!file.is_open()) {
			delete[] in.text;
			throw ERROR_THROW(110);
		}

		char symbol;
		char quote_symbol = '\0';				// внутри строкового литерала комментарии не начинаются
		int line = 0;
		int col = 0;
		int pos = 0;

		while (file.get(symbol)) {
			unsigned char usymbol = symbol;

			// комментарий "//" до конца строки в текст программы не попадает
			if (quote_symbol == '\0' && symbol == '/' && file.peek() == '/') {
				file.get(symbol);						// второй '/'
				in.ignore += 2;
				while (file.get(symbol) && symbol != '\n') {
					in.ignore++;
				}
				if (symbol == '\n' && pos < IN_MAX_LENGTH_TEXT - 1) {
					in.text[pos++] = '\n';
					in.size++;
					in.lines++;
					line++;
					col = 0;
				}
				continue;
			}

			if (quote_symbol == '\0' && (symbol == '"' || symbol == '\'')) {
				quote_symbol = symbol;
			}
			else if (symbol == quote_symbol) {
				quote_symbol = '\0';
			}

			int rule = in.code[usymbol];

			if (rule == In::F)
			{
				delete[] in.text;
				throw ERROR_THROW_PLACE(111, line, col);
			}
			else if (rule == In::I)
			{
				in.ignore++;
				col++;
			}
			else
			{
				char out_symbol;
				if (rule == In::T)
				{
					out_symbol = symbol;
				}
				else
				{
					out_symbol = (char)rule;
				}


				if (pos < IN_MAX_LENGTH_TEXT - 1)
				{
					in.text[pos++] = out_symbol;
					in.size++;
				}

				col++;
				if (usymbol == '\n') {
					in.lines++;
					line++;
					col = 0;
				}
			}
		}
		in.text[pos] = '\0';
		file.close();

		// строка без завершающего "\n" - тоже строка
		if (in.size > 0 && in.text[pos - 1] != '\n') {
			in.lines++;
		}

		return in;
	}
}