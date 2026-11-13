#include "LIBS.h"

namespace LEXTABLE 
{
	LexTable Create(int size) {
		if (size > LEXTABLE_MAX_COUNT_OF_LINES) throw ERROR_THROW(120);

		LexTable lextable;

		lextable.maxsize = size;
		lextable.current_size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry) {
		if (lextable.current_size >= lextable.maxsize) throw ERROR_THROW(121);

		lextable.table[lextable.current_size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int str_num) {
		if (str_num < 0 || str_num >= lextable.current_size) throw ERROR_THROW(122);

		return lextable.table[str_num];
	}

	void Delete(LexTable& lextable) {
		if (lextable.table != nullptr) {
			delete[] lextable.table;
			lextable.table = nullptr;
		}
		lextable.current_size = 0;
		lextable.maxsize = 0;
	}
}