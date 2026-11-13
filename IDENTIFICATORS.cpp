#include "LIBS.h"

namespace IDTABLE
{
	IdTable Create(int size) {	
		if (size > IDENTIFICATOR_TABLE_MAX_SIZE) throw ERROR_THROW(130);

		IdTable idtable;
		idtable.it_max_size = size;
		idtable.current_size = 0;
		idtable.table = new Entry[size];
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry) {
		if (idtable.current_size >= idtable.it_max_size) throw ERROR_THROW(131);

		idtable.table[idtable.current_size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int str_num) {
		if (str_num < 0 || str_num >= idtable.current_size) throw ERROR_THROW(132);
		
		return idtable.table[str_num];
	}

	int IsID(IdTable& idtable, char id[ID_MAX_CHARS + 1]) {
		for (short i = 0; i < idtable.current_size; i++) {
			if (strcmp(idtable.table[i].id, id) == 0) return i;
		}
		return IT_NULLIDX;
	}
	
	void Delete(IdTable& idtable) {
		if (idtable.table != nullptr) {
			delete[] idtable.table;
			idtable.table = nullptr;
		}
		
		idtable.current_size = 0;
		idtable.it_max_size = 0;
	}
}
