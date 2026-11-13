#pragma once
#define ID_MAX_CHARS					32
#define IDENTIFICATOR_TABLE_MAX_SIZE	4096

#define IT_INT_DEFAULT					0x00000000
#define IT_FLOAT_DEFAULT				0.0f
#define IT_STRING_DEFAULT				0x00
#define IT_SYMBOL_DEFAULT				0x00
#define IT_BOOLEAN_DEFAULT				0x00

#define IT_NULLIDX						0xffffffff

#define IT_STRING_MAX					512

namespace IDTABLE 
{
	enum ID_DATA_TYPE
	{
		INT8 = 1,
		INT16 = 2,
		INT32 = 3,
		INT64 = 4,

		FLOAT32 = 11,
		FLOAT64 = 12,

		SYMBOL = 21,
		STRING = 22,
		FSTRING = 23,

		BOOLEAN = 0,

		NUMBER = 99,
		HELP = 100
		
	};

	enum ID_TYPE
	{
		V = 1,
		F = 2,
		P = 3,
		L = 4,
	};


	struct Entry
	{
		int index_first_string_LE;
		char id[ID_MAX_CHARS];
		ID_DATA_TYPE id_data_type;
		ID_TYPE id_type;

		union {

			int8_t var_int8;
			int16_t var_int16;
			int32_t var_int32;
			int64_t var_int64;

			float var_float32;
			double var_float64;

			char var_string[IT_STRING_MAX];
			char var_fstring[IT_STRING_MAX];

			char var_symbol;

			bool var_boolean;
		} id_value;	
	};

	struct IdTable
	{
		int it_max_size;
		int current_size;
		Entry* table;
	};

	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int num_str);
	int IsID(IdTable& idtable, char id[ID_MAX_CHARS]);
	void Delete(IdTable& idtable);


}