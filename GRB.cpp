#include "LIBS.h"
#define GRB_ERROR_SERIES 600

namespace GRB
{


	Rule::Chain::Chain(short count_of_chars, grbalphabet s, ...)
		: size(count_of_chars > 0 ? count_of_chars : 0), term_and_notterm(nullptr)
	{
		if (size <= 0) return;

		term_and_notterm = new grbalphabet[size];
		term_and_notterm[0] = s;

		va_list args;
		va_start(args, s);
		for (short i = 1; i < size; i++) {
			term_and_notterm[i] = static_cast<grbalphabet>(va_arg(args, int));
		}
		va_end(args);
	}

	Rule::Chain::Chain(const Chain& other)
		: size(other.size), term_and_notterm(other.size > 0 ? new grbalphabet[other.size] : nullptr)
	{
		if (term_and_notterm) {
			std::memcpy(term_and_notterm, other.term_and_notterm, size * sizeof(grbalphabet));
		}
	}

	Rule::Chain& Rule::Chain::operator=(const Chain& other)
	{
		if (this != &other) {
			grbalphabet* temp = other.size > 0 ? new grbalphabet[other.size] : nullptr;
			if (temp && other.term_and_notterm) {
				std::memcpy(temp, other.term_and_notterm, other.size * sizeof(grbalphabet));
			}
			delete[] term_and_notterm;
			term_and_notterm = temp;
			size = other.size;
		}
		return *this;
	}

	Rule::Chain::~Chain() { delete[] term_and_notterm; }

	char* Rule::Chain::GetChain(char* buffer) const
	{
		int index = 0;
		char temp[5];

		for (short i = 0; i < size; i++) {
			alphabet_to_char(term_and_notterm[i], temp);
			buffer[index++] = temp[0];
			buffer[index++] = temp[1];
			buffer[index++] = temp[2];
			buffer[index++] = temp[3];
			buffer[index++] = ' ';
		}
		buffer[index] = '\0';
		return buffer;
	}



	Rule::Rule(grbalphabet _not_terminal, int _errorID, short _count, Chain* chains, ...)
		: not_terminal(_not_terminal), errorID(_errorID), count(0), chain(nullptr)
	{
		if (_count <= 0 || chains == nullptr) return;

		count = _count;
		chain = new Chain[count];
		chain[0] = *chains;

		va_list args;
		va_start(args, chains);
		for (short i = 1; i < count; i++) {
			Chain* ptr = va_arg(args, Chain*);
			chain[i] = *ptr;
		}
		va_end(args);
	}

	Rule::Rule(const Rule& other)
		: not_terminal(other.not_terminal), errorID(other.errorID), count(other.count),
		chain(other.count > 0 ? new Chain[other.count] : nullptr)
	{
		for (short i = 0; i < count; i++) {
			chain[i] = other.chain[i];
		}
	}

	Rule& Rule::operator=(const Rule& other)
	{
		if (this != &other) {
			Chain* temp = other.count > 0 ? new Chain[other.count] : nullptr;
			for (short i = 0; i < other.count; ++i)
				temp[i] = other.chain[i];
			delete[] chain;
			chain = temp;
			not_terminal = other.not_terminal;
			errorID = other.errorID;
			count = other.count;
		}
		return *this;
	}

	Rule::~Rule() { delete[] chain; }

	char* Rule::GetRule(char* buffer, short number_of_chain, size_t buffer_size) const
	{
		if (number_of_chain < 0 || number_of_chain >= count || buffer_size < 16) {
			if (buffer_size > 0) buffer[0] = '\0';
			return buffer;
		}

		char chain_buf[512];
		char nt_str[5];

		Chain::alphabet_to_char(not_terminal, nt_str);

		buffer[0] = nt_str[0];
		buffer[1] = nt_str[1];
		buffer[2] = nt_str[2];
		buffer[3] = nt_str[3];
		buffer[4] = ' ';
		buffer[5] = '-';
		buffer[6] = '>';
		buffer[7] = ' ';
		buffer[8] = '\0';

		chain[number_of_chain].GetChain(chain_buf);
		strncat_s(buffer, buffer_size, chain_buf, buffer_size - strlen(buffer) - 1);
		return buffer;
	}

	short Rule::GetNextChain(grbalphabet first_symb, Chain& back_chain, short chain_num) const
	{
		for (short i = chain_num + 1; i < count; i++) {
			if (chain[i].size > 0 && chain[i].term_and_notterm[0] == first_symb) {
				back_chain = chain[i];
				return i;
			}
		}
		return -1;
	}


	Greibach::Greibach(short _rule_count, grbalphabet _start_symbol, grbalphabet _stack_bottom, Rule* _rules, ...)
		: rule_count(_rule_count > 0 ? _rule_count : 0), start_symbol(_start_symbol), stack_bottom(_stack_bottom), rules(nullptr)
	{
		if (rule_count <= 0 || _rules == nullptr) { rule_count = 0; return; }

		rules = new Rule[rule_count];
		rules[0] = *_rules;

		va_list args;
		va_start(args, _rules);
		for (short i = 1; i < rule_count; i++) {
			Rule* ptr = va_arg(args, Rule*);
			rules[i] = *ptr;
		}
		va_end(args);
	}

	Greibach::Greibach(const Greibach& other)
		: rule_count(other.rule_count), start_symbol(other.start_symbol), stack_bottom(other.stack_bottom),
		rules(other.rule_count > 0 ? new Rule[other.rule_count] : nullptr)
	{
		for (short i = 0; i < rule_count; ++i)
			rules[i] = other.rules[i];
	}

	Greibach& Greibach::operator=(const Greibach& other)
	{
		if (this != &other) {
			Rule* temp = other.rule_count > 0 ? new Rule[other.rule_count] : nullptr;
			for (short i = 0; i < other.rule_count; i++) {
				temp[i] = other.rules[i];
			}
			delete[] rules;
			rules = temp;
			rule_count = other.rule_count;
			start_symbol = other.start_symbol;
			stack_bottom = other.stack_bottom;
		}
		return *this;
	}

	Greibach::~Greibach() { delete[] rules; }

	short Greibach::GetRule(grbalphabet left_symbol_of_rule, Rule& back_rule) const
	{
		for (short i = 0; i < rule_count; i++) {
			if (rules[i].not_terminal == left_symbol_of_rule) {
				back_rule = rules[i];
				return i;
			}
		}
		return -1;
	}

	Rule Greibach::GetRule(short number) const
	{
		if (number < 0 || number >= rule_count) {
			return Rule();
		}
		return rules[number];
	}


#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)


	Greibach GetGreibach()
	{
		static Rule::Chain Eps;			// eps


		// TYPE


		static Rule::Chain Type_c1(1, TS(LEX_INT8));			// i8
		static Rule::Chain Type_c2(1, TS(LEX_INT16));			// i16
		static Rule::Chain Type_c3(1, TS(LEX_INT32));			// i32
		static Rule::Chain Type_c4(1, TS(LEX_INT64));			// i64
		static Rule::Chain Type_c5(1, TS(LEX_FLOAT32));			// f32
		static Rule::Chain Type_c6(1, TS(LEX_FLOAT64));			// f64
		static Rule::Chain Type_c7(1, TS(LEX_BOOLEAN));			// bool
		static Rule::Chain Type_c8(1, TS(LEX_STRING));			// str
		static Rule::Chain Type_c9(1, TS(LEX_SYMBOL));			// symb
		static Rule::Chain Type_c10(1, TS(LEX_NUMBER));			// numb
		static Rule::Chain Type_c11(1, TS(LEX_HELP));			// help
		static Rule::Chain Type_c12(1, TS(LEX_VOID));			// void 

		static Rule ruleType(NS("Type"), GRB_ERROR_SERIES + 10, 12, &Type_c1, &Type_c2, &Type_c3, &Type_c4, &Type_c5, &Type_c6, &Type_c7, &Type_c8, &Type_c9, &Type_c10, &Type_c11, &Type_c12);


		// ARRAY TYPE


		static Rule::Chain Array_c1(2, TS(LEX_ARRAY), NS("Type"));												// arr Type
		
		static Rule ruleArray(NS("ArrT"), GRB_ERROR_SERIES + 20, 1, &Array_c1);


		// PARAMS


		static Rule::Chain Prms_c1(4, NS("Type"), TS(LEX_IDENTIFICATOR), TS(LEX_COMMA), NS("Prms"));			// Type idfr, Prms
		static Rule::Chain Prms_c2(4, NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_COMMA), NS("Prms"));			// ArrT idfr, Prms

		static Rule::Chain Prms_c3(2, NS("Type"), TS(LEX_IDENTIFICATOR));										// Type idfr
		static Rule::Chain Prms_c4(2, NS("ArrT"), TS(LEX_IDENTIFICATOR));										// ArrT idfr
		
		static Rule rulePrms(NS("Prms"), GRB_ERROR_SERIES + 60, 5, &Prms_c1, &Prms_c2, &Prms_c3, &Prms_c4, &Eps);


		// BLOCK


		static Rule::Chain Blck_c1(3, TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET));			// {Body}
		static Rule::Chain Blck_c2(3, TS(LEX_START), NS("Body"), TS(LEX_END));											// strt Body end
		
		static Rule ruleBlck(NS("Blck"), GRB_ERROR_SERIES + 30, 2, &Blck_c1, &Blck_c2);


		// PROCEDURE


		static Rule::Chain Proc_c1(7, NS("Type"), TS(LEX_PROCEDURE), TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_BRACKET), NS("Prms"), TS(LEX_RIGHT_BRACKET), NS("Blck"));			// Type proc idfr (Prms) Blck
		static Rule::Chain Proc_c2(7, NS("ArrT"), TS(LEX_PROCEDURE), TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_BRACKET), NS("Prms"), TS(LEX_RIGHT_BRACKET), NS("Blck"));			// ArrT proc idfr (Prms) Blck
		
		static Rule ruleProc(NS("Proc"), GRB_ERROR_SERIES + 40, 2, &Proc_c1, &Proc_c2);


		// START


		static Rule::Chain Strt_c1(3, TS(LEX_SSA2026), NS("Proc"), NS("Strt"));																// 2026 Proc Strt
		static Rule::Chain Strt_c2(4, TS(LEX_SSA2026), TS(LEX_MAIN), NS("Blck"), NS("Strt"));												// 2026 main Blck Strt
		static Rule::Chain Strt_c3(3, TS(LEX_MAIN), NS("Blck"), NS("Strt"));																// main Blck Strt
		static Rule::Chain Strt_c4(1, TS("$   "));																							// $
		static Rule::Chain Strt_c5(2, NS("Proc"), NS("Strt"));																				// Proc Strt
		
		static Rule ruleStrt(NS("Strt"), GRB_ERROR_SERIES + 0, 5, &Strt_c1, &Strt_c2, &Strt_c3, &Strt_c4, &Strt_c5);


		// BODY


		static Rule::Chain Body_c1(2, NS("Inst"), NS("Body"));									// Inst Body
		
		static Rule ruleBody(NS("Body"), GRB_ERROR_SERIES + 1, 2, &Body_c1, &Eps);


		// STATE
		

		static Rule::Chain Inst_c1(1, NS("Decl"));																																						// Decl
		static Rule::Chain Inst_c2(1, NS("Asgn"));																																						// Asgn

		static Rule::Chain Inst_c3(5, TS(LEX_PRINT), TS(LEX_LEFT_BRACKET), NS("PArg"), TS(LEX_RIGHT_BRACKET), TS(LEX_SEMICOLON));																		// prn (PArg);
		static Rule::Chain Inst_c4(5, TS(LEX_PRINTF), TS(LEX_LEFT_BRACKET), NS("PArg"), TS(LEX_RIGHT_BRACKET), TS(LEX_SEMICOLON));																		// prnf (PArg);

		static Rule::Chain Inst_c5(6, TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), TS(LEX_INPUT), TS(LEX_LEFT_BRACKET), TS(LEX_RIGHT_BRACKET), TS(LEX_SEMICOLON));												// idfr = inpt ();
		static Rule::Chain Inst_c6(7, NS("Type"), TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), TS(LEX_INPUT), TS(LEX_LEFT_BRACKET), TS(LEX_RIGHT_BRACKET), TS(LEX_SEMICOLON));									// Type idfr = inpt ();

		static Rule::Chain Inst_c7(3, TS(LEX_BACK), NS("Expr"), TS(LEX_SEMICOLON));																														// back Expr;
		static Rule::Chain Inst_c15(2, TS(LEX_BACK), TS(LEX_SEMICOLON));																																// back ;
		static Rule::Chain Inst_c8(2, TS(LEX_BREAK), TS(LEX_SEMICOLON));																																// brek;
		static Rule::Chain Inst_c9(1, NS("Cond"));																																						// Cond
		static Rule::Chain Inst_c10(1, NS("Cycl"));																																						// Cycl
		static Rule::Chain Inst_c11(2, NS("Expr"), TS(LEX_SEMICOLON));																																	// Expr;
		static Rule::Chain Inst_c12(2, NS("Step"), TS(LEX_SEMICOLON));																																	// Step;

		static Rule::Chain Inst_c13(4, TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("Step"), TS(LEX_SEMICOLON));																							// idfr = Step;
		static Rule::Chain Inst_c14(5, NS("Type"), TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("Step"), TS(LEX_SEMICOLON));																				// Type idfr = Step;
		
		static Rule ruleInst(NS("Inst"), GRB_ERROR_SERIES + 100, 15,&Inst_c1, &Inst_c2, &Inst_c3, &Inst_c4, &Inst_c5, &Inst_c6, &Inst_c7, &Inst_c15, &Inst_c8, &Inst_c9, &Inst_c10, &Inst_c11, &Inst_c12, &Inst_c13, &Inst_c14);


		// DECLARATIONS


		static Rule::Chain Decl_c1(5, NS("Type"), TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));																																			// Type idfr = Expr;
		static Rule::Chain Decl_c2(3, NS("Type"), TS(LEX_IDENTIFICATOR), TS(LEX_SEMICOLON));																																									// Type idfr;

		static Rule::Chain Decl_c3(8, NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_EQUAL), NS("AVal"), TS(LEX_SEMICOLON));																	// ArrT idfr [Expr] = AVal;
		static Rule::Chain Decl_c4(6, NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_SEMICOLON));																								// ArrT idfr [Expr];

		static Rule::Chain Decl_c5(5, NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("AVal"), TS(LEX_SEMICOLON));																																			// ArrT idfr = AVal;
		static Rule::Chain Decl_c6(7, NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), TS(LEX_LEFT_FIGURE_BRACKET), NS("AVal"), TS(LEX_RIGHT_FIGURE_BRACKET),TS(LEX_SEMICOLON));																				// ArrT idfr = {AVal};
		static Rule::Chain Decl_c7(10,NS("ArrT"), TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_EQUAL), TS(LEX_LEFT_FIGURE_BRACKET), NS("AVal"),TS(LEX_RIGHT_FIGURE_BRACKET),TS(LEX_SEMICOLON));			// ArrT idfr [Expr] = {AVal};
		
		static Rule ruleDecl(NS("Decl"), GRB_ERROR_SERIES + 130, 7, &Decl_c1, &Decl_c2, &Decl_c3, &Decl_c4, &Decl_c5, &Decl_c6, &Decl_c7);


		// ASSIGNMENT


		static Rule::Chain Asgn_c1(4, TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));																							// idfr = Expr;
		static Rule::Chain Asgn_c2(4, TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("AVal"), TS(LEX_SEMICOLON));																							// idfr = AVal;

		static Rule::Chain Asgn_c3(4, TS(LEX_IDENTIFICATOR), TS(LEX_PLUS_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr += Expr;
		static Rule::Chain Asgn_c4(4, TS(LEX_IDENTIFICATOR), TS(LEX_MINUS_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr -= Expr;
		static Rule::Chain Asgn_c5(4, TS(LEX_IDENTIFICATOR), TS(LEX_STAR_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr *= Expr;
		static Rule::Chain Asgn_c6(4, TS(LEX_IDENTIFICATOR), TS(LEX_DIRSLAH_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr /= Expr;

		static Rule::Chain Asgn_c7(7, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));					// idfr [Expr] = Expr;
		static Rule::Chain Asgn_c8(7, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_PLUS_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));				// idfr [Expr] += Expr;
		static Rule::Chain Asgn_c9(7, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_MINUS_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr [Expr] -= Expr;
		static Rule::Chain Asgn_c10(7, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_STAR_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr [Expr] *= Expr;
		static Rule::Chain Asgn_c11(7, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_SQUARE_BRACKET), NS("Expr"), TS(LEX_RIGHT_SQUARE_BRACKET), TS(LEX_DIRSLAH_EQUAL), NS("Expr"), TS(LEX_SEMICOLON));			// idfr [Expr] /= Expr;
		static Rule ruleAsgn(NS("Asgn"), GRB_ERROR_SERIES + 131, 11, &Asgn_c1, &Asgn_c2, &Asgn_c3, &Asgn_c4, &Asgn_c5, &Asgn_c6, &Asgn_c7, &Asgn_c8, &Asgn_c9, &Asgn_c10, &Asgn_c11);


		// STEP/ FOR


		static Rule::Chain Step_c1(3, TS(LEX_IDENTIFICATOR), TS(LEX_EQUAL), NS("Expr"));								// idfr = Expr
		static Rule::Chain Step_c2(1, NS("Expr"));																		// Expr
		static Rule::Chain Step_c3(3, TS(LEX_IDENTIFICATOR), TS(LEX_PLUS), TS(LEX_PLUS));								// idfr ++
		static Rule::Chain Step_c4(3, TS(LEX_IDENTIFICATOR), TS(LEX_MINUS), TS(LEX_MINUS));								// idfr --
		static Rule ruleStep(NS("Step"), GRB_ERROR_SERIES + 132, 4, &Step_c1, &Step_c2, &Step_c3, &Step_c4);

		static Rule::Chain ForP_c1(1, NS("Decl"));			// Decl
		static Rule::Chain ForP_c2(1, NS("Asgn"));			// Asgn
		static Rule ruleForP(NS("ForP"), GRB_ERROR_SERIES + 8, 2, &ForP_c1, &ForP_c2);


		// CONDITION


		static Rule::Chain Cond_c1(8, TS(LEX_IF), TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET), NS("Elif"));				// if (Expr) {Body} Elif
		static Rule::Chain Cond_c2(7, TS(LEX_SWITCH), TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Case"), TS(LEX_RIGHT_FIGURE_BRACKET));						// swch (Expr) {Case}
		
		static Rule ruleCond(NS("Cond"), GRB_ERROR_SERIES + 90, 2, &Cond_c1, &Cond_c2);


		// ELIF


		static Rule::Chain Elif_c1(8, TS(LEX_ELIF), TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET), NS("Elif"));			// elif (Expr) {Body} Elif
		static Rule::Chain Elif_c2(4, TS(LEX_ELSE), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET));																					// else {Body}
		
		static Rule ruleElif(NS("Elif"), GRB_ERROR_SERIES + 110, 3, &Elif_c1, &Elif_c2, &Eps);



		static Rule::Chain Case_c1(5, TS(LEX_CASE), NS("Expr"), TS(LEX_DOUBLEPOINT), NS("Body"), NS("Case"));			// case Expr : Body Case
		static Rule::Chain Case_c2(3, TS(LEX_DEFAULT), TS(LEX_DOUBLEPOINT), NS("Body"));								// deft : Body
		
		static Rule ruleCase(NS("Case"), GRB_ERROR_SERIES + 120, 3, &Case_c1, &Case_c2, &Eps);


		// CYCLES


		static Rule::Chain Cycl_c1(7, TS(LEX_WHILE), TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET));													// whle (Expr) {Body}
		static Rule::Chain Cycl_c2(9, TS(LEX_DO), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET), TS(LEX_WHILE), TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET), TS(LEX_SEMICOLON));					// do {Body} whle (Expr);
		static Rule::Chain Cycl_c3(9, TS(LEX_FOR), TS(LEX_LEFT_BRACKET), TS(LEX_IDENTIFICATOR), TS(LEX_IN), TS(LEX_IDENTIFICATOR), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET));		// for (idfr in idfr) {Body}
		static Rule::Chain Cycl_c4(10, TS(LEX_FOR), TS(LEX_LEFT_BRACKET), NS("ForP"), NS("Expr"), TS(LEX_SEMICOLON), NS("Step"), TS(LEX_RIGHT_BRACKET), TS(LEX_LEFT_FIGURE_BRACKET), NS("Body"), TS(LEX_RIGHT_FIGURE_BRACKET));			// for (ForP Expr; Step) {Body}
		
		static Rule ruleCycl(NS("Cycl"), GRB_ERROR_SERIES + 7, 4, &Cycl_c1, &Cycl_c2, &Cycl_c3, &Cycl_c4);


		// EXPRESSIONS

		static Rule::Chain Expr_c1(1, NS("Cmpd"));							// Cmpd
		
		static Rule ruleExpr(NS("Expr"), GRB_ERROR_SERIES + 5, 1, &Expr_c1);


		// COMPARISON


		static Rule::Chain Cmpd_c1(2, NS("Summ"), NS("CmpT"));				// Summ CmpT

		static Rule ruleCmpd(NS("Cmpd"), GRB_ERROR_SERIES + 5, 1, &Cmpd_c1);

		static Rule::Chain CmpT_c1(3, TS(LEX_EQUAL_EQUAL), NS("Summ"), NS("CmpT"));		// == Summ CmpT
		static Rule::Chain CmpT_c2(3, TS(LEX_NOTEQUAL), NS("Summ"), NS("CmpT"));		// != Summ CmpT
		static Rule::Chain CmpT_c3(3, TS(LEX_LESS), NS("Summ"), NS("CmpT"));			// < Summ CmpT
		static Rule::Chain CmpT_c4(3, TS(LEX_GREATER), NS("Summ"), NS("CmpT"));			// > Summ CmpT
		static Rule::Chain CmpT_c5(3, TS(LEX_LESSEQ), NS("Summ"), NS("CmpT"));			// <= Summ CmpT
		static Rule::Chain CmpT_c6(3, TS(LEX_GREATEREQ), NS("Summ"), NS("CmpT"));		// >= Summ CmpT

		static Rule ruleCmpT(NS("CmpT"), GRB_ERROR_SERIES + 5, 7, &CmpT_c1, &CmpT_c2, &CmpT_c3, &CmpT_c4, &CmpT_c5, &CmpT_c6, &Eps);


		// ADDITIVE ( + , - )


		static Rule::Chain Summ_c1(2, NS("Mult"), NS("SumT"));						// Mult SumT

		static Rule ruleSumm(NS("Summ"), GRB_ERROR_SERIES + 5, 1, &Summ_c1);

		static Rule::Chain SumT_c1(3, TS(LEX_PLUS), NS("Mult"), NS("SumT"));		// + Mult SumT
		static Rule::Chain SumT_c2(3, TS(LEX_MINUS), NS("Mult"), NS("SumT"));		// - Mult SumT

		static Rule ruleSumT(NS("SumT"), GRB_ERROR_SERIES + 5, 3, &SumT_c1, &SumT_c2, &Eps);


		// MULTIPLICATIVE ( * , / )


		static Rule::Chain Mult_c1(2, NS("Unry"), NS("MulT"));						// Unry MulT

		static Rule ruleMult(NS("Mult"), GRB_ERROR_SERIES + 5, 1, &Mult_c1);

		static Rule::Chain MulT_c1(3, TS(LEX_STAR), NS("Unry"), NS("MulT"));		// * Unry MulT
		static Rule::Chain MulT_c2(3, TS(LEX_DIRSLASH), NS("Unry"), NS("MulT"));	// / Unry MulT

		static Rule ruleMulT(NS("MulT"), GRB_ERROR_SERIES + 5, 3, &MulT_c1, &MulT_c2, &Eps);


		// UNARY ( - , ! )


		static Rule::Chain Unry_c1(2, TS(LEX_MINUS), NS("Unry"));				// - Unry
		static Rule::Chain Unry_c2(2, TS(LEX_EXCLAMATORY), NS("Unry"));			// ! Unry
		static Rule::Chain Unry_c3(1, NS("Sqre"));								// Sqre

		static Rule ruleUnry(NS("Unry"), GRB_ERROR_SERIES + 5, 3, &Unry_c1, &Unry_c2, &Unry_c3);


		// SQUARE ( ^2 )


		static Rule::Chain Sqre_c1(2, NS("Opnd"), NS("SqrT"));						// Opnd SqrT

		static Rule ruleSqre(NS("Sqre"), GRB_ERROR_SERIES + 5, 1, &Sqre_c1);

		static Rule::Chain SqrT_c1(2, TS(LEX_CUBE), NS("SqrT"));					// ^2 SqrT

		static Rule ruleSqrT(NS("SqrT"), GRB_ERROR_SERIES + 5, 2, &SqrT_c1, &Eps);


		// OPERAND


		static Rule::Chain Opnd_c1(4, TS(LEX_IDENTIFICATOR), TS(LEX_LEFT_BRACKET), NS("PCal"), TS(LEX_RIGHT_BRACKET));			// idfr (PCal)
		static Rule::Chain Opnd_c2(1, TS(LEX_IDENTIFICATOR));																	// idfr
		static Rule::Chain Opnd_c3(1, TS(LEX_LITERAL));																			// lter
		static Rule::Chain Opnd_c4(1, TS(LEX_STRING_LITERAL));																	// strl
		static Rule::Chain Opnd_c5(3, TS(LEX_LEFT_BRACKET), NS("Expr"), TS(LEX_RIGHT_BRACKET));									// (Expr)
		static Rule::Chain Opnd_c6(3, TS(LEX_INPUT),TS(LEX_LEFT_BRACKET),TS(LEX_RIGHT_BRACKET));								// inpt ()
		
		static Rule ruleOpnd(NS("Opnd"), GRB_ERROR_SERIES + 150, 6, &Opnd_c1, &Opnd_c2, &Opnd_c3, &Opnd_c4, &Opnd_c5, &Opnd_c6);



		// PROCEDURE CALL


		static Rule::Chain PCal_c1(3, NS("Expr"), TS(LEX_COMMA), NS("PCal"));			// Expr, PCal
		static Rule::Chain PCal_c2(1, NS("Expr"));										// Expr
		
		static Rule rulePCal(NS("PCal"), GRB_ERROR_SERIES + 160, 3, &PCal_c1, &PCal_c2, &Eps);


		// ARRAY VALUES


		static Rule::Chain AVal_c1(3, TS(LEX_LITERAL), TS(LEX_COMMA), NS("AVal"));					// lter, AVal
		static Rule::Chain AVal_c2(3, TS(LEX_IDENTIFICATOR), TS(LEX_COMMA), NS("AVal"));			// idfr, AVal
		static Rule::Chain AVal_c3(1, TS(LEX_LITERAL));												// lter
		static Rule::Chain AVal_c4(1, TS(LEX_IDENTIFICATOR));										// idfr
		
		static Rule ruleAVal(NS("AVal"), GRB_ERROR_SERIES + 180, 4, &AVal_c1, &AVal_c2, &AVal_c3, &AVal_c4);



		// PRINT ARGUMENT


		static Rule::Chain PArg_c1(1, NS("Expr"));						// Expr
		static Rule::Chain PArg_c2(1, TS(LEX_FORMATED_STRING));			// fstr
		
		static Rule rulePArg(NS("PArg"), GRB_ERROR_SERIES + 150, 2, &PArg_c1, &PArg_c2);


		return Greibach(30, NS("Strt"), TS("#   "),
			&ruleStrt, &ruleProc, &ruleBlck, &ruleBody, &ruleInst,
			&ruleDecl, &ruleAsgn, &ruleStep, &ruleForP, &ruleType,
			&ruleArray, &rulePrms, &ruleCond, &ruleElif, &ruleCase,
			&ruleCycl, &ruleExpr, &ruleCmpd, &ruleCmpT, &ruleSumm,
			&ruleSumT, &ruleMult, &ruleMulT, &ruleUnry, &ruleSqre,
			&ruleSqrT, &ruleOpnd, &rulePCal, &ruleAVal, &rulePArg
		);
	}

}