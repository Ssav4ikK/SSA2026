#pragma once
#include "LIBS.h"

typedef int grbalphabet;

namespace GRB
{
	struct Rule
	{
		grbalphabet not_terminal;
		int errorID;
		short count;

		struct Chain {

			short size;
			grbalphabet* term_and_notterm;

			Chain() : size(0), term_and_notterm(nullptr) {}
			Chain(short count_of_chars, grbalphabet s, ...);
			Chain(const Chain& other);
			~Chain();

			Chain& operator=(const Chain& chain);

			char* GetChain(char* buffer) const;

			static grbalphabet T(const char* t) {
				int val = 0;
				std::memcpy(&val, t, 4);
				return val;
			}

			static grbalphabet N(const char* n) {
				int val = 0;
				std::memcpy(&val, n, 4);
				return -val;
			}

			static bool isT(grbalphabet s) { return s > 0; }
			static bool isN(grbalphabet s) { return s < 0; }

			static void alphabet_to_char(grbalphabet s, char* dest) {
				if (s < 0) s = -s;
				std::memcpy(dest, &s, 4);
				dest[4] = '\0';
			}
		}*chain;

		Rule() : not_terminal(0), errorID(0), count(0), chain(nullptr) {}
		Rule(grbalphabet _not_terminal, int _errorID, short _count, Chain* chains, ...);
		Rule(const Rule& other);

		Rule& operator=(const Rule& other);

		~Rule();

		char* GetRule(char* buffer, short number_of_chain, size_t buffer_size = 512) const;
		short GetNextChain(grbalphabet first_symb, Chain& back_chain, short chain_num) const;
	};

	struct Greibach
	{
		short rule_count;
		grbalphabet start_symbol;
		grbalphabet stack_bottom;
		Rule* rules;

		Greibach() : rule_count(0), start_symbol(0), stack_bottom(0), rules(nullptr) {}
		Greibach(short _rule_count, grbalphabet _start_symbol, grbalphabet _stack_bottom, Rule* _rules, ...);
		Greibach(const Greibach& other);

		Greibach& operator=(const Greibach& other);

		~Greibach();

		short GetRule(grbalphabet left_symbol_of_rule, Rule& back_rule) const;
		Rule GetRule(short number) const;
	};

	Greibach GetGreibach();
}