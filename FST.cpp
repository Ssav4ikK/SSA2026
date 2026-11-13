#include "LIBS.h"

namespace FST
{
	Relation::Relation(char ch, short new_state) : symbol(ch), num_node(new_state) {};
	Node::Node() : num_relations(0), relations(nullptr) {};

	Node::Node(short num, Relation relation, ...) : num_relations(num), relations(nullptr)
	{
		if (num <= 0) return;
		relations = new Relation[num];
		relations[0] = relation;

		va_list args;
		va_start(args, relation);
		for (short i = 1; i < num; ++i) {
			relations[i] = va_arg(args, Relation);
		}
		va_end(args);
	};

	Fst::Fst(char* str, short num_st, Node node, ...) : string(str), count_states(num_st), nodes(nullptr), possible_states(nullptr)
	{
		nodes = new Node[num_st];
		nodes[0] = node;

		va_list args;
		va_start(args, node);
		for (short i = 1; i < num_st; ++i) {
			nodes[i] = va_arg(args, Node);
		}
		va_end(args);

		possible_states = new short[count_states];

		for (short i = 0; i < count_states; ++i) {
			possible_states[i] = -1;
		}
		possible_states[0] = 0;
	}

	Fst::~Fst() {
		if (nodes) {
			for (short i = 0; i < count_states; ++i) {
				if (nodes[i].relations) delete[] nodes[i].relations;
			}
			delete[] nodes;
		}
		if (possible_states) delete[] possible_states;
	}

	static bool step(Fst& fst, short* next_possible_state) {
		bool has_transition = false;
		for (short i = 0; i < fst.count_states; ++i)
		{
			if (fst.possible_states[i] != fst.current_position) continue;

			for (short j = 0; j < fst.nodes[i].num_relations; ++j)
			{
				if (fst.nodes[i].relations[j].symbol == fst.string[fst.current_position])
				{
					short target = fst.nodes[i].relations[j].num_node;
					next_possible_state[target] = fst.current_position + 1;
					has_transition = true;
				}
			}
		}
		return has_transition;
	}

	bool execute(Fst& fst, short finalState)
	{
		short length = (short)strlen(fst.string);

		short* next_possible_states = new short[fst.count_states];

		// execute() может вызываться для одного автомата несколько раз,
		// поэтому начальное состояние восстанавливается при каждом запуске
		for (short state_index = 0; state_index < fst.count_states; ++state_index) {
			fst.possible_states[state_index] = -1;
		}
		fst.possible_states[0] = 0;
		fst.current_position = 0;

		for (short i = 0; i < length; ++i) {
			fst.current_position = i;

			for (short j = 0; j < fst.count_states; ++j) {
				next_possible_states[j] = -1;
			}

			step(fst, next_possible_states);

			for (short j = 0; j < fst.count_states; ++j) {
				fst.possible_states[j] = next_possible_states[j];
			}
		}
			bool accepted = (fst.possible_states[finalState] == length);
			fst.current_position = length;
			delete[] next_possible_states;
			return accepted;
	}

	bool execute(Fst& fst) {
		return execute(fst, fst.count_states - 1);
	}





	//INT

	bool isInt8(char* str) {
		Fst fst(str, 5,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node(1, Relation('t', 3)),
			Node(1, Relation('8', 4)),
			Node());
		return execute(fst);		
	}

	bool isInt16(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node(1, Relation('t', 3)),
			Node(1, Relation('1', 4)),
			Node(1, Relation('6', 5)),
			Node());
		return execute(fst);
	}

	bool isInt32(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node(1, Relation('t', 3)),
			Node(1, Relation('3', 4)),
			Node(1, Relation('2', 5)),
			Node());
		return execute(fst);
	}

	bool isInt64(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node(1, Relation('t', 3)),
			Node(1, Relation('6', 4)),
			Node(1, Relation('4', 5)),
			Node());
		return execute(fst);
	}
	
	//FLOAT

	bool isFloat32(char* str) {
		Fst fst(str, 8,
			Node(1, Relation('f', 1)),
			Node(1, Relation('l', 2)),
			Node(1, Relation('o', 3)),
			Node(1, Relation('a', 4)),
			Node(1, Relation('t', 5)),
			Node(1, Relation('3', 6)),
			Node(1, Relation('2', 7)),
			Node());
		return execute(fst);
	}

	bool isFloat64(char* str) {
		Fst fst(str, 8,
			Node(1, Relation('f', 1)),
			Node(1, Relation('l', 2)),
			Node(1, Relation('o', 3)),
			Node(1, Relation('a', 4)),
			Node(1, Relation('t', 5)),
			Node(1, Relation('6', 6)),
			Node(1, Relation('4', 7)),
			Node());
		return execute(fst);
	}

	//STRING, SYMBOL

	bool isSymbol(char* str) {
		Fst fst(str, 7,
			Node(1, Relation('s', 1)),
			Node(1, Relation('y', 2)),
			Node(1, Relation('m', 3)),
			Node(1, Relation('b', 4)),
			Node(1, Relation('o', 5)),
			Node(1, Relation('l', 6)),
			Node());
		return execute(fst);
	}

	bool isString(char* str) {
		Fst fst(str, 7,
			Node(1, Relation('s', 1)),
			Node(1, Relation('t', 2)),
			Node(1, Relation('r', 3)),
			Node(1, Relation('i', 4)),
			Node(1, Relation('n', 5)),
			Node(1, Relation('g', 6)),
			Node());
		return execute(fst);
	}

	//BOOLEAN

	bool isBoolean(char* str) {
		Fst fst(str, 8,
			Node(1, Relation('b', 1)),
			Node(1, Relation('o', 2)),
			Node(1, Relation('o', 3)),
			Node(1, Relation('l', 4)),
			Node(1, Relation('e', 5)),
			Node(1, Relation('a', 6)),
			Node(1, Relation('n', 7)),
			Node());
		return execute(fst);
	}

	//NUMBER, HELP

	bool isNumber(char* str) {
		Fst fst(str, 7,
			Node(1, Relation('n', 1)),
			Node(1, Relation('u', 2)),
			Node(1, Relation('m', 3)),
			Node(1, Relation('b', 4)),
			Node(1, Relation('e', 5)),
			Node(1, Relation('r', 6)),
			Node());
		return execute(fst);
	}

	bool isHelp(char* str) {
		Fst fst(str, 5,
			Node(1, Relation('h', 1)),
			Node(1, Relation('e', 2)),
			Node(1, Relation('l', 3)),
			Node(1, Relation('p', 4)),
			Node());
		return execute(fst);
	}

	// ARRAY

	bool isArray(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('a', 1)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('r', 3)),
			Node(1, Relation('a', 4)),
			Node(1, Relation('y', 5)),
			Node());
		return execute(fst);
	}

	// PROCEDURES

	bool isProcedure(char* str) {
		Fst fst(str, 10,
			Node(1, Relation('p', 1)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('o', 3)),
			Node(1, Relation('c', 4)),
			Node(1, Relation('e', 5)),
			Node(1, Relation('d', 6)),
			Node(1, Relation('u', 7)),
			Node(1, Relation('r', 8)),
			Node(1, Relation('e', 9)),
			Node());
		return execute(fst);
	}
	
	// START, END

	bool isStart(char* str) {
		Fst fst(str, 6,
			Node(2, Relation('s', 1), Relation('S', 1)),
			Node(2, Relation('t', 2), Relation('T', 2)),
			Node(2, Relation('a', 3), Relation('A', 3)),
			Node(2, Relation('r', 4), Relation('R', 4)),
			Node(2, Relation('t', 5), Relation('T', 5)),
			Node());
		return execute(fst);
	}

	bool isEnd(char* str) {
		Fst fst(str, 4,
			Node(2, Relation('e', 1), Relation('E', 1)),
			Node(2, Relation('n', 2), Relation('N', 2)),
			Node(2, Relation('d', 3), Relation('D', 3)),
			Node());
		return execute(fst);
	}


	// BACK

	bool isBack(char* str) {
		Fst fst(str, 5,
			Node(2, Relation('b', 1), Relation('B', 1)),
			Node(2, Relation('a', 2), Relation('A', 2)),
			Node(2, Relation('c', 3), Relation('C', 3)),
			Node(2, Relation('k', 4), Relation('K', 4)),
			Node());
		return execute(fst);
	}

	// MAIN

	bool isMain(char* str) {
		Fst fst(str, 5,
			Node(2, Relation('m', 1), Relation('M', 1)),
			Node(2, Relation('a', 2), Relation('A', 2)),
			Node(2, Relation('i', 3), Relation('I', 3)),
			Node(2, Relation('n', 4), Relation('N', 4)),
			Node());
		return execute(fst);
	}

	// BASIC FUNCTIONS
	//PRINT PRINTF INPUT

	bool isPrint(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('p', 1)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('i', 3)),
			Node(1, Relation('n', 4)),
			Node(1, Relation('t', 5)),
			Node());
		return execute(fst);
	}

	bool isPrintF(char* str) {
		Fst fst(str, 7,
			Node(1, Relation('p', 1)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('i', 3)),
			Node(1, Relation('n', 4)),
			Node(1, Relation('t', 5)),
			Node(1, Relation('f', 6)),
			Node());
		return execute(fst);
	}

	bool isInput(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node(1, Relation('p', 3)),
			Node(1, Relation('u', 4)),
			Node(1, Relation('t', 5)),
			Node());
		return execute(fst);
	}

	// CONSTRUCTIONS

	//IF ELIF ELSE
	
	bool isSwitch(char* str) {
		Fst fst(str, 7,
			Node(1, Relation('s', 1)),
			Node(1, Relation('w', 2)),
			Node(1, Relation('i', 3)),
			Node(1, Relation('t', 4)),
			Node(1, Relation('c', 5)),
			Node(1, Relation('h', 6)),
			Node());
		return execute(fst);
	}

	bool isCase(char* str) {
		Fst fst(str, 5,
			Node(1, Relation('c', 1)),
			Node(1, Relation('a', 2)),
			Node(1, Relation('s', 3)),
			Node(1, Relation('e', 4)),
			Node());
		return execute(fst);
	}

	bool isBreak(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('b', 1)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('e', 3)),
			Node(1, Relation('a', 4)),
			Node(1, Relation('k', 5)),
			Node());
		return execute(fst);
	}

	bool isDefault(char* str) {
		Fst fst(str, 8,
			Node(1, Relation('d', 1)),
			Node(1, Relation('e', 2)),
			Node(1, Relation('f', 3)),
			Node(1, Relation('a', 4)),
			Node(1, Relation('u', 5)),
			Node(1, Relation('l', 6)),
			Node(1, Relation('t', 7)),
			Node());
		return execute(fst);
	}


	bool isIf(char* str) {
		Fst fst(str, 3,
			Node(1, Relation('i', 1)),
			Node(1, Relation('f', 2)),
			Node());
		return execute(fst);
	}

	bool isElif(char* str) {
		Fst fst(str, 5,
			Node(1, Relation('e', 1)),
			Node(1, Relation('l', 2)),
			Node(1, Relation('i', 3)),
			Node(1, Relation('f', 4)),
			Node());
		return execute(fst);
	}

	bool isElse(char* str) {
		Fst fst(str, 5,
			Node(1, Relation('e', 1)),
			Node(1, Relation('l', 2)),
			Node(1, Relation('s', 3)),
			Node(1, Relation('e', 4)),
			Node());
		return execute(fst);
	}

	//FOR,  FOR IN

	bool isFor(char* str) {
		Fst fst(str, 4,
			Node(1, Relation('f', 1)),
			Node(1, Relation('o', 2)),
			Node(1, Relation('r', 3)),
			Node());
		return execute(fst);
	}

	bool isIn(char* str) {
		Fst fst(str, 3,
			Node(1, Relation('i', 1)),
			Node(1, Relation('n', 2)),
			Node());
		return execute(fst);
	}

	// WHILE, DO 

	bool isDo(char* str) {
		Fst fst(str, 3,
			Node(1, Relation('d', 1)),
			Node(1, Relation('o', 2)),
			Node());
		return execute(fst);
	}

	bool isWhile(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('w', 1)),
			Node(1, Relation('h', 2)),
			Node(1, Relation('i', 3)),
			Node(1, Relation('l', 4)),
			Node(1, Relation('e', 5)),
			Node());
		return execute(fst);
	}

	// IDENTIFICATORS, LITERALS

	bool isIdentifier(char* str) {
		Fst fst(str, 2,
			Node(119,
				Relation('A', 1), Relation('a', 1),
				Relation('B', 1), Relation('b', 1),
				Relation('C', 1), Relation('c', 1),
				Relation('D', 1), Relation('d', 1),
				Relation('E', 1), Relation('e', 1),
				Relation('F', 1), Relation('f', 1),
				Relation('G', 1), Relation('g', 1),
				Relation('H', 1), Relation('h', 1),
				Relation('I', 1), Relation('i', 1),
				Relation('J', 1), Relation('j', 1),
				Relation('K', 1), Relation('k', 1),
				Relation('L', 1), Relation('l', 1),
				Relation('M', 1), Relation('m', 1),
				Relation('N', 1), Relation('n', 1),
				Relation('O', 1), Relation('o', 1),
				Relation('P', 1), Relation('p', 1),
				Relation('Q', 1), Relation('q', 1),
				Relation('R', 1), Relation('r', 1),
				Relation('S', 1), Relation('s', 1),
				Relation('T', 1), Relation('t', 1),
				Relation('U', 1), Relation('u', 1),
				Relation('V', 1), Relation('v', 1),
				Relation('W', 1), Relation('w', 1),
				Relation('X', 1), Relation('x', 1),
				Relation('Y', 1), Relation('y', 1),
				Relation('Z', 1), Relation('z', 1),
				Relation('_', 1),
				Relation('А', 1), Relation('а', 1),
				Relation('Б', 1), Relation('б', 1),
				Relation('В', 1), Relation('в', 1),
				Relation('Г', 1), Relation('г', 1),
				Relation('Д', 1), Relation('д', 1),
				Relation('Е', 1), Relation('е', 1),
				Relation('Ё', 1), Relation('ё', 1),
				Relation('Ж', 1), Relation('ж', 1),
				Relation('З', 1), Relation('з', 1),
				Relation('И', 1), Relation('и', 1),
				Relation('Й', 1), Relation('й', 1),
				Relation('К', 1), Relation('к', 1),
				Relation('Л', 1), Relation('л', 1),
				Relation('М', 1), Relation('м', 1),
				Relation('Н', 1), Relation('н', 1),
				Relation('О', 1), Relation('о', 1),
				Relation('П', 1), Relation('п', 1),
				Relation('Р', 1), Relation('р', 1),
				Relation('С', 1), Relation('с', 1),
				Relation('Т', 1), Relation('т', 1),
				Relation('У', 1), Relation('у', 1),
				Relation('Ф', 1), Relation('ф', 1),
				Relation('Х', 1), Relation('х', 1),
				Relation('Ц', 1), Relation('ц', 1),
				Relation('Ч', 1), Relation('ч', 1),
				Relation('Ш', 1), Relation('ш', 1),
				Relation('Щ', 1), Relation('щ', 1),
				Relation('Ъ', 1), Relation('ъ', 1),
				Relation('Ы', 1), Relation('ы', 1),
				Relation('Ь', 1), Relation('ь', 1),
				Relation('Э', 1), Relation('э', 1),
				Relation('Ю', 1), Relation('ю', 1),
				Relation('Я', 1), Relation('я', 1)
			),
			Node(129,
				Relation('A', 1), Relation('a', 1),
				Relation('B', 1), Relation('b', 1),
				Relation('C', 1), Relation('c', 1),
				Relation('D', 1), Relation('d', 1),
				Relation('E', 1), Relation('e', 1),
				Relation('F', 1), Relation('f', 1),
				Relation('G', 1), Relation('g', 1),
				Relation('H', 1), Relation('h', 1),
				Relation('I', 1), Relation('i', 1),
				Relation('J', 1), Relation('j', 1),
				Relation('K', 1), Relation('k', 1),
				Relation('L', 1), Relation('l', 1),
				Relation('M', 1), Relation('m', 1),
				Relation('N', 1), Relation('n', 1),
				Relation('O', 1), Relation('o', 1),
				Relation('P', 1), Relation('p', 1),
				Relation('Q', 1), Relation('q', 1),
				Relation('R', 1), Relation('r', 1),
				Relation('S', 1), Relation('s', 1),
				Relation('T', 1), Relation('t', 1),
				Relation('U', 1), Relation('u', 1),
				Relation('V', 1), Relation('v', 1),
				Relation('W', 1), Relation('w', 1),
				Relation('X', 1), Relation('x', 1),
				Relation('Y', 1), Relation('y', 1),
				Relation('Z', 1), Relation('z', 1),
				Relation('_', 1),
				Relation('А', 1), Relation('а', 1),
				Relation('Б', 1), Relation('б', 1),
				Relation('В', 1), Relation('в', 1),
				Relation('Г', 1), Relation('г', 1),
				Relation('Д', 1), Relation('д', 1),
				Relation('Е', 1), Relation('е', 1),
				Relation('Ё', 1), Relation('ё', 1),
				Relation('Ж', 1), Relation('ж', 1),
				Relation('З', 1), Relation('з', 1),
				Relation('И', 1), Relation('и', 1),
				Relation('Й', 1), Relation('й', 1),
				Relation('К', 1), Relation('к', 1),
				Relation('Л', 1), Relation('л', 1),
				Relation('М', 1), Relation('м', 1),
				Relation('Н', 1), Relation('н', 1),
				Relation('О', 1), Relation('о', 1),
				Relation('П', 1), Relation('п', 1),
				Relation('Р', 1), Relation('р', 1),
				Relation('С', 1), Relation('с', 1),
				Relation('Т', 1), Relation('т', 1),
				Relation('У', 1), Relation('у', 1),
				Relation('Ф', 1), Relation('ф', 1),
				Relation('Х', 1), Relation('х', 1),
				Relation('Ц', 1), Relation('ц', 1),
				Relation('Ч', 1), Relation('ч', 1),
				Relation('Ш', 1), Relation('ш', 1),
				Relation('Щ', 1), Relation('щ', 1),
				Relation('Ъ', 1), Relation('ъ', 1),
				Relation('Ы', 1), Relation('ы', 1),
				Relation('Ь', 1), Relation('ь', 1),
				Relation('Э', 1), Relation('э', 1),
				Relation('Ю', 1), Relation('ю', 1),
				Relation('Я', 1), Relation('я', 1),

				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1)
				));
		return execute(fst, 1);
	}

	bool isIntegerLiteral(char* str) {
		Fst fst(str, 2,
			Node(10,
				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1)
			),
			Node(10,
				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1)
			));
		return execute(fst, 1);
	}

	bool isFloatLiteral(char* str) {
		Fst fst(str, 4,
			Node(10,
				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1)
			),
			Node(11,
				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1),
				Relation('.', 2)
			),
			Node(10,
				Relation('1', 3), Relation('6', 3),
				Relation('2', 3), Relation('7', 3),
				Relation('3', 3), Relation('8', 3),
				Relation('4', 3), Relation('9', 3),
				Relation('5', 3), Relation('0', 3)
			),
			Node(10,
				Relation('1', 3), Relation('6', 3),
				Relation('2', 3), Relation('7', 3),
				Relation('3', 3), Relation('8', 3),
				Relation('4', 3), Relation('9', 3),
				Relation('5', 3), Relation('0', 3)
			));
		return execute(fst, 3);
	}

	bool isBooleanLiteral(char* str) {
		Fst fst(str, 10,
			Node(2, Relation('t', 1), Relation('f', 5)),
			Node(1, Relation('r', 2)),
			Node(1, Relation('u', 3)),
			Node(1, Relation('e', 4)),
			Node(),
			Node(1, Relation('a', 6)),
			Node(1, Relation('l', 7)),
			Node(1, Relation('s', 8)),
			Node(1, Relation('e', 9)),
			Node()
		);
		return execute(fst, 4) || execute(fst, 9);
	}

	bool isSymbolLiteral(char* str) {
		Fst fst(str, 4,
			Node(1, Relation('\'', 1)),
			Node(155,
				Relation('A', 2), Relation('a', 2),
				Relation('B', 2), Relation('b', 2),
				Relation('C', 2), Relation('c', 2),
				Relation('D', 2), Relation('d', 2),
				Relation('E', 2), Relation('e', 2),
				Relation('F', 2), Relation('f', 2),
				Relation('G', 2), Relation('g', 2),
				Relation('H', 2), Relation('h', 2),
				Relation('I', 2), Relation('i', 2),
				Relation('J', 2), Relation('j', 2),
				Relation('K', 2), Relation('k', 2),
				Relation('L', 2), Relation('l', 2),
				Relation('M', 2), Relation('m', 2),
				Relation('N', 2), Relation('n', 2),
				Relation('O', 2), Relation('o', 2),
				Relation('P', 2), Relation('p', 2),
				Relation('Q', 2), Relation('q', 2),
				Relation('R', 2), Relation('r', 2),
				Relation('S', 2), Relation('s', 2),
				Relation('T', 2), Relation('t', 2),
				Relation('U', 2), Relation('u', 2),
				Relation('V', 2), Relation('v', 2),
				Relation('W', 2), Relation('w', 2),
				Relation('X', 2), Relation('x', 2),
				Relation('Y', 2), Relation('y', 2),
				Relation('Z', 2), Relation('z', 2),
				Relation('_', 2),
				Relation('А', 2), Relation('а', 2),
				Relation('Б', 2), Relation('б', 2),
				Relation('В', 2), Relation('в', 2),
				Relation('Г', 2), Relation('г', 2),
				Relation('Д', 2), Relation('д', 2),
				Relation('Е', 2), Relation('е', 2),
				Relation('Ё', 2), Relation('ё', 2),
				Relation('Ж', 2), Relation('ж', 2),
				Relation('З', 2), Relation('з', 2),
				Relation('И', 2), Relation('и', 2),
				Relation('Й', 2), Relation('й', 2),
				Relation('К', 2), Relation('к', 2),
				Relation('Л', 2), Relation('л', 2),
				Relation('М', 2), Relation('м', 2),
				Relation('Н', 2), Relation('н', 2),
				Relation('О', 2), Relation('о', 2),
				Relation('П', 2), Relation('п', 2),
				Relation('Р', 2), Relation('р', 2),
				Relation('С', 2), Relation('с', 2),
				Relation('Т', 2), Relation('т', 2),
				Relation('У', 2), Relation('у', 2),
				Relation('Ф', 2), Relation('ф', 2),
				Relation('Х', 2), Relation('х', 2),
				Relation('Ц', 2), Relation('ц', 2),
				Relation('Ч', 2), Relation('ч', 2),
				Relation('Ш', 2), Relation('ш', 2),
				Relation('Щ', 2), Relation('щ', 2),
				Relation('Ъ', 2), Relation('ъ', 2),
				Relation('Ы', 2), Relation('ы', 2),
				Relation('Ь', 2), Relation('ь', 2),
				Relation('Э', 2), Relation('э', 2),
				Relation('Ю', 2), Relation('ю', 2),
				Relation('Я', 2), Relation('я', 2),

				Relation('1', 2), Relation('6', 2),
				Relation('2', 2), Relation('7', 2),
				Relation('3', 2), Relation('8', 2),
				Relation('4', 2), Relation('9', 2),
				Relation('5', 2), Relation('0', 2),

				Relation('.', 2), Relation(',', 2),
				Relation('-', 2), Relation('_', 2),
				Relation('?', 2), Relation('!', 2),
				Relation(';', 2), Relation(':', 2),
				Relation('{', 2), Relation('}', 2),
				Relation('&', 2), Relation('$', 2),
				Relation('#', 2), Relation('@', 2),
				Relation('(', 2), Relation(')', 2),
				Relation('[', 2), Relation(']', 2),
				Relation('%', 2), Relation('=', 2),
				Relation('+', 2), Relation('*', 2),
				Relation('/', 2), Relation('\\', 2),
				Relation('~', 2), Relation('`', 2)
			),
			Node(1, Relation('\'', 3)),
			Node()
		);

		return execute(fst, 3);
	}

	bool isStringLiteral(char* str) {
		Fst fst(str, 3,
			Node(1, Relation('\"', 1)),
			Node(157,
				Relation('A', 1), Relation('a', 1),
				Relation('B', 1), Relation('b', 1),
				Relation('C', 1), Relation('c', 1),
				Relation('D', 1), Relation('d', 1),
				Relation('E', 1), Relation('e', 1),
				Relation('F', 1), Relation('f', 1),
				Relation('G', 1), Relation('g', 1),
				Relation('H', 1), Relation('h', 1),
				Relation('I', 1), Relation('i', 1),
				Relation('J', 1), Relation('j', 1),
				Relation('K', 1), Relation('k', 1),
				Relation('L', 1), Relation('l', 1),
				Relation('M', 1), Relation('m', 1),
				Relation('N', 1), Relation('n', 1),
				Relation('O', 1), Relation('o', 1),
				Relation('P', 1), Relation('p', 1),
				Relation('Q', 1), Relation('q', 1),
				Relation('R', 1), Relation('r', 1),
				Relation('S', 1), Relation('s', 1),
				Relation('T', 1), Relation('t', 1),
				Relation('U', 1), Relation('u', 1),
				Relation('V', 1), Relation('v', 1),
				Relation('W', 1), Relation('w', 1),
				Relation('X', 1), Relation('x', 1),
				Relation('Y', 1), Relation('y', 1),
				Relation('Z', 1), Relation('z', 1),
				Relation('_', 1),
				Relation('А', 1), Relation('а', 1),
				Relation('Б', 1), Relation('б', 1),
				Relation('В', 1), Relation('в', 1),
				Relation('Г', 1), Relation('г', 1),
				Relation('Д', 1), Relation('д', 1),
				Relation('Е', 1), Relation('е', 1),
				Relation('Ё', 1), Relation('ё', 1),
				Relation('Ж', 1), Relation('ж', 1),
				Relation('З', 1), Relation('з', 1),
				Relation('И', 1), Relation('и', 1),
				Relation('Й', 1), Relation('й', 1),
				Relation('К', 1), Relation('к', 1),
				Relation('Л', 1), Relation('л', 1),
				Relation('М', 1), Relation('м', 1),
				Relation('Н', 1), Relation('н', 1),
				Relation('О', 1), Relation('о', 1),
				Relation('П', 1), Relation('п', 1),
				Relation('Р', 1), Relation('р', 1),
				Relation('С', 1), Relation('с', 1),
				Relation('Т', 1), Relation('т', 1),
				Relation('У', 1), Relation('у', 1),
				Relation('Ф', 1), Relation('ф', 1),
				Relation('Х', 1), Relation('х', 1),
				Relation('Ц', 1), Relation('ц', 1),
				Relation('Ч', 1), Relation('ч', 1),
				Relation('Ш', 1), Relation('ш', 1),
				Relation('Щ', 1), Relation('щ', 1),
				Relation('Ъ', 1), Relation('ъ', 1),
				Relation('Ы', 1), Relation('ы', 1),
				Relation('Ь', 1), Relation('ь', 1),
				Relation('Э', 1), Relation('э', 1),
				Relation('Ю', 1), Relation('ю', 1),
				Relation('Я', 1), Relation('я', 1),

				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1),

				Relation('.', 1), Relation(',', 1),
				Relation('-', 1), Relation('_', 1),
				Relation('?', 1), Relation('!', 1),
				Relation(';', 1), Relation(':', 1),
				Relation(' ', 1),
				Relation('{', 1),
				Relation('&', 1), Relation('$', 1),
				Relation('#', 1), Relation('@', 1),
				Relation('(', 1), Relation(')', 1),
				Relation('[', 1), Relation(']', 1),
				Relation('%', 1), Relation('=', 1),
				Relation('+', 1), Relation('*', 1),
				Relation('/', 1), Relation('\\', 2),
				Relation('~', 1), Relation('`', 1),
				Relation('\'', 1),
				Relation('\"', 2)
			),
			Node(5,
				Relation('\"', 1),
				Relation('\\', 1),
				Relation('n', 1),
				Relation('t', 1),
				Relation('r', 1)
			)
		);

		return execute(fst, 2);
	}

	bool isFormatedStringLiteral(char* str) {
		Fst fst(str, 6,
			Node(1, Relation('\"', 1)),
			Node(157,
				Relation('A', 1), Relation('a', 1),
				Relation('B', 1), Relation('b', 1),
				Relation('C', 1), Relation('c', 1),
				Relation('D', 1), Relation('d', 1),
				Relation('E', 1), Relation('e', 1),
				Relation('F', 1), Relation('f', 1),
				Relation('G', 1), Relation('g', 1),
				Relation('H', 1), Relation('h', 1),
				Relation('I', 1), Relation('i', 1),
				Relation('J', 1), Relation('j', 1),
				Relation('K', 1), Relation('k', 1),
				Relation('L', 1), Relation('l', 1),
				Relation('M', 1), Relation('m', 1),
				Relation('N', 1), Relation('n', 1),
				Relation('O', 1), Relation('o', 1),
				Relation('P', 1), Relation('p', 1),
				Relation('Q', 1), Relation('q', 1),
				Relation('R', 1), Relation('r', 1),
				Relation('S', 1), Relation('s', 1),
				Relation('T', 1), Relation('t', 1),
				Relation('U', 1), Relation('u', 1),
				Relation('V', 1), Relation('v', 1),
				Relation('W', 1), Relation('w', 1),
				Relation('X', 1), Relation('x', 1),
				Relation('Y', 1), Relation('y', 1),
				Relation('Z', 1), Relation('z', 1),
				Relation('_', 1),
				Relation('А', 1), Relation('а', 1),
				Relation('Б', 1), Relation('б', 1),
				Relation('В', 1), Relation('в', 1),
				Relation('Г', 1), Relation('г', 1),
				Relation('Д', 1), Relation('д', 1),
				Relation('Е', 1), Relation('е', 1),
				Relation('Ё', 1), Relation('ё', 1),
				Relation('Ж', 1), Relation('ж', 1),
				Relation('З', 1), Relation('з', 1),
				Relation('И', 1), Relation('и', 1),
				Relation('Й', 1), Relation('й', 1),
				Relation('К', 1), Relation('к', 1),
				Relation('Л', 1), Relation('л', 1),
				Relation('М', 1), Relation('м', 1),
				Relation('Н', 1), Relation('н', 1),
				Relation('О', 1), Relation('о', 1),
				Relation('П', 1), Relation('п', 1),
				Relation('Р', 1), Relation('р', 1),
				Relation('С', 1), Relation('с', 1),
				Relation('Т', 1), Relation('т', 1),
				Relation('У', 1), Relation('у', 1),
				Relation('Ф', 1), Relation('ф', 1),
				Relation('Х', 1), Relation('х', 1),
				Relation('Ц', 1), Relation('ц', 1),
				Relation('Ч', 1), Relation('ч', 1),
				Relation('Ш', 1), Relation('ш', 1),
				Relation('Щ', 1), Relation('щ', 1),
				Relation('Ъ', 1), Relation('ъ', 1),
				Relation('Ы', 1), Relation('ы', 1),
				Relation('Ь', 1), Relation('ь', 1),
				Relation('Э', 1), Relation('э', 1),
				Relation('Ю', 1), Relation('ю', 1),
				Relation('Я', 1), Relation('я', 1),

				Relation('1', 1), Relation('6', 1),
				Relation('2', 1), Relation('7', 1),
				Relation('3', 1), Relation('8', 1),
				Relation('4', 1), Relation('9', 1),
				Relation('5', 1), Relation('0', 1),

				Relation('.', 1), Relation(',', 1),
				Relation('-', 1), Relation('_', 1),
				Relation('?', 1), Relation('!', 1),
				Relation(';', 1), Relation(':', 1),
				Relation(' ', 1),
				Relation('{', 3),
				Relation('&', 1), Relation('$', 1),
				Relation('#', 1), Relation('@', 1),
				Relation('(', 1), Relation(')', 1),
				Relation('[', 1), Relation(']', 1),
				Relation('%', 1), Relation('=', 1),
				Relation('+', 1), Relation('*', 1),
				Relation('/', 1), Relation('\\', 2),
				Relation('~', 1), Relation('`', 1),
				Relation('\'', 1),
				Relation('\"', 5)
			),


			Node(5,
				Relation('\"', 1),
				Relation('\\', 1),
				Relation('n', 1),
				Relation('t', 1),
				Relation('r', 1)
			),

			Node(130,
				Relation('A', 3), Relation('a', 3),
				Relation('B', 3), Relation('b', 3),
				Relation('C', 3), Relation('c', 3),
				Relation('D', 3), Relation('d', 3),
				Relation('E', 3), Relation('e', 3),
				Relation('F', 3), Relation('f', 3),
				Relation('G', 3), Relation('g', 3),
				Relation('H', 3), Relation('h', 3),
				Relation('I', 3), Relation('i', 3),
				Relation('J', 3), Relation('j', 3),
				Relation('K', 3), Relation('k', 3),
				Relation('L', 3), Relation('l', 3),
				Relation('M', 3), Relation('m', 3),
				Relation('N', 3), Relation('n', 3),
				Relation('O', 3), Relation('o', 3),
				Relation('P', 3), Relation('p', 3),
				Relation('Q', 3), Relation('q', 3),
				Relation('R', 3), Relation('r', 3),
				Relation('S', 3), Relation('s', 3),
				Relation('T', 3), Relation('t', 3),
				Relation('U', 3), Relation('u', 3),
				Relation('V', 3), Relation('v', 3),
				Relation('W', 3), Relation('w', 3),
				Relation('X', 3), Relation('x', 3),
				Relation('Y', 3), Relation('y', 3),
				Relation('Z', 3), Relation('z', 3),
				Relation('_', 3),
				Relation('А', 3), Relation('а', 3),
				Relation('Б', 3), Relation('б', 3),
				Relation('В', 3), Relation('в', 3),
				Relation('Г', 3), Relation('г', 3),
				Relation('Д', 3), Relation('д', 3),
				Relation('Е', 3), Relation('е', 3),
				Relation('Ё', 3), Relation('ё', 3),
				Relation('Ж', 3), Relation('ж', 3),
				Relation('З', 3), Relation('з', 3),
				Relation('И', 3), Relation('и', 3),
				Relation('Й', 3), Relation('й', 3),
				Relation('К', 3), Relation('к', 3),
				Relation('Л', 3), Relation('л', 3),
				Relation('М', 3), Relation('м', 3),
				Relation('Н', 3), Relation('н', 3),
				Relation('О', 3), Relation('о', 3),
				Relation('П', 3), Relation('п', 3),
				Relation('Р', 3), Relation('р', 3),
				Relation('С', 3), Relation('с', 3),
				Relation('Т', 3), Relation('т', 3),
				Relation('У', 3), Relation('у', 3),
				Relation('Ф', 3), Relation('ф', 3),
				Relation('Х', 3), Relation('х', 3),
				Relation('Ц', 3), Relation('ц', 3),
				Relation('Ч', 3), Relation('ч', 3),
				Relation('Ш', 3), Relation('ш', 3),
				Relation('Щ', 3), Relation('щ', 3),
				Relation('Ъ', 3), Relation('ъ', 3),
				Relation('Ы', 3), Relation('ы', 3),
				Relation('Ь', 3), Relation('ь', 3),
				Relation('Э', 3), Relation('э', 3),
				Relation('Ю', 3), Relation('ю', 3),
				Relation('Я', 3), Relation('я', 3),
				Relation('1', 3), Relation('6', 3),
				Relation('2', 3), Relation('7', 3),
				Relation('3', 3), Relation('8', 3),
				Relation('4', 3), Relation('9', 3),
				Relation('5', 3), Relation('0', 3),
				Relation('}', 1)      
			),
			Node(),
			Node()
		);

		return execute(fst, 5);
	}
	bool isSSA(char* str) {
		Fst fst(str, 13,
			Node(1, Relation('S', 1)),
			Node(1, Relation('S', 2)),
			Node(1, Relation('A', 3)),
			Node(1, Relation('V', 4)),
			Node(1, Relation('4', 5)),
			Node(1, Relation('I', 6)),
			Node(1, Relation('K', 7)),
			Node(1, Relation('K', 8)),
			Node(1, Relation('2', 9)),
			Node(1, Relation('0', 10)),
			Node(1, Relation('2', 11)),
			Node(1, Relation('6', 12)),
			Node());
		return execute(fst);
	}



	std::string match(char* str)
	{
		if (isSSA(str))			return LEX_SSA2026;

		if (isInt8(str))		return LEX_INT8;
		if (isInt16(str))		return LEX_INT16;
		if (isInt32(str))		return LEX_INT32;
		if (isInt64(str))		return LEX_INT64;
		if (isFloat32(str))		return LEX_FLOAT32;
		if (isFloat64(str))		return LEX_FLOAT64;
		if (isBoolean(str))		return LEX_BOOLEAN;
		if (isString(str))		return LEX_STRING;
		if (isSymbol(str))		return LEX_SYMBOL;

		if (isNumber(str))		return LEX_NUMBER;
		if (isHelp(str))		return LEX_HELP;

		if (isIf(str))			return LEX_IF;
		if (isElse(str))		return LEX_ELSE;
		if (isElif(str))		return LEX_ELIF;
		if (isFor(str))			return LEX_FOR;
		if (isIn(str))			return LEX_IN;
		if (isDo(str))			return LEX_DO;
		if (isWhile(str))		return LEX_WHILE;
		if (isMain(str))		return LEX_MAIN;
		if (isStart(str))		return LEX_START;
		if (isEnd(str))			return LEX_END;
		if (isBack(str))		return LEX_BACK;
		if (isProcedure(str))	return LEX_PROCEDURE;
		if (isPrint(str))		return LEX_PRINT;
		if (isPrintF(str))		return LEX_PRINTF;
		if (isInput(str))		return LEX_INPUT;
		if (isArray(str))		return LEX_ARRAY;

		if (isSwitch(str))		return LEX_SWITCH;
		if (isCase(str))		return LEX_CASE;
		if (isDefault(str))		return LEX_DEFAULT;
		if (isBreak(str))		return LEX_BREAK;

		if (isIntegerLiteral(str))			return LEX_LITERAL;
		if (isFloatLiteral(str))			return LEX_LITERAL;
		if (isBooleanLiteral(str))			return LEX_LITERAL;
		if (isStringLiteral(str))			return LEX_LITERAL;
		if (isSymbolLiteral(str))			return LEX_LITERAL;
		if (isFormatedStringLiteral(str))	return LEX_LITERAL;

		if (isIdentifier(str))				return LEX_IDENTIFICATOR;

		return "";
	}
}