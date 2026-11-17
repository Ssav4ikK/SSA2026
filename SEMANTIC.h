#pragma once
#include "LIBS.h"
#include "AST.h"

namespace SEMANTIC
{
	struct TypeInfo 
	{

		AST::Type base_type = AST::Void;
		AST::Type element = AST::Void;
		int array_size = 0;
		bool is_array = false;
		bool is_unknow = false;

	};

	enum SymbolKind
	{
		SYMBOL_VARIABLE,
		SYMBOL_PARAMETR,
		SYMBOL_ITERATOR
	};

	struct Symbol
	{
		std::string name;
		SymbolKind kind = SymbolKind::SYMBOL_VARIABLE;
		TypeInfo type;

		int node_index = -1;
		int line = 0;
		int column = 0;
		int scope = 0;
		bool is_init = false;
	};

	struct Parameter
	{
		std::string name;
		TypeInfo type;
	};

	struct ProcedureInfo 
	{
		std::string name;
		TypeInfo return_type;

		std::vector<Parameter> params;

		int node_index = -1;
		int line = 0;
		int column = 0;
		bool is_buildin = false;
	};
	
	struct Scope 
	{
		int parent = -1;
		int depth = 0;
		int owner_node = -1;
		int line = 0;
		int column = 0;
	};

	struct Diagnostics
	{
		int code = 0;
		std::string message;

		int line = 0;
		int column = 0;
	};

	struct Context
	{
		AST::AstTree* tree = nullptr;

		std::vector<Scope> scopes;
		std::vector<ProcedureInfo> procedures;
		std::vector<Symbol> symbols;
		std::vector<Diagnostics> diagnostics;

		bool has_main = false;

		int loop_depth = 0;
		int switch_depth = 0;
		int procedure_depth = 0;
		TypeInfo return_type;
	};

	struct Result
	{
		bool is_okay = true;
		std::vector<Diagnostics> diagnostic;
	};

	Result Analyze(AST::AstTree& tree);
	void WriteLog(LOG::Log& log, Result& result);


	void InitContext(Context& context, AST::AstTree& tree);
	bool RunSemantic(Context& context);
	void CollectDeclarations(Context& context);
	void CheckMain(Context& context, int nodeIndex);
	void CheckProcedure(Context& context, int nodeIndex);
	void CheckParameter(Context& context, int nodeIndex);


	void PushScope(Context& context, int ownerNode);
	void PopScope(Context& context);
	int DeclareSymbol(Context& context, Symbol symbol);
	int LookupSymbol(Context& context, const std::string& name); 
	int FindFunction(Context& context, const std::string& name); 




	void CheckBlock(Context& context, int nodeIndex);
	void CheckStatement(Context& context, int nodeIndex);
	void CheckDeclaration(Context& context, int nodeIndex);
	void CheckArrayDeclaration(Context& context, int nodeIndex);
	void CheckAssignment(Context& context, int nodeIndex);
	void CheckPrint(Context& context, int nodeIndex, bool isPrintf);
	void CheckReturn(Context& context, int nodeIndex);
	void CheckIf(Context& context, int nodeIndex);
	void CheckElif(Context& context, int nodeIndex);
	void CheckElse(Context& context, int nodeIndex);
	void CheckSwitch(Context& context, int nodeIndex);
	void CheckCase(Context& context, int nodeIndex);
	void CheckDefault(Context& context, int nodeIndex);
	void CheckWhile(Context& context, int nodeIndex);
	void CheckDoWhile(Context& context, int nodeIndex);
	void CheckFor(Context& context, int nodeIndex);
	void CheckForIn(Context& context, int nodeIndex);
	void CheckBreak(Context& context, int nodeIndex);
	void CheckStep(Context& context, int nodeIndex);
	void CheckArrayInitialization(Context& context, int nodeIndex, TypeInfo elementType);




	TypeInfo CheckExpression(Context& context, int nodeIndex);
	TypeInfo CheckLiteral(Context& context, int nodeIndex);
	TypeInfo CheckVariable(Context& context, int nodeIndex);
	TypeInfo CheckBinary(Context& context, int nodeIndex);
	TypeInfo CheckUnary(Context& context, int nodeIndex);
	TypeInfo CheckCall(Context& context, int nodeIndex);
	TypeInfo CheckArrayAccess(Context& context, int nodeIndex);
	TypeInfo CheckInput(Context& context, int nodeIndex);
	TypeInfo CheckFormattedString(Context& context, int nodeIndex);
	TypeInfo CheckPlaceholder(Context& context, int nodeIndex);



	TypeInfo TypeFromAst(AST::Type type);
	TypeInfo TypeArrayOf(AST::Type element, int size);

	bool IsInt8(const TypeInfo& type);
	bool IsInt16(const TypeInfo& type);
	bool IsInt32(const TypeInfo& type);
	bool IsInt64(const TypeInfo& type);

	bool IsFloating(const TypeInfo& type);
	bool IsNumeric(const TypeInfo& type);
	bool IsString(const TypeInfo& type);
	bool IsBoolean(const TypeInfo& type);
	bool IsPrintable(const TypeInfo& type);
	bool IsAssignable(const TypeInfo& from, const TypeInfo& to);
	TypeInfo CommonNumeric(const TypeInfo& left, const TypeInfo& right);
	void Annotate(Context& context, int nodeIndex, const TypeInfo& type);




	bool EvalConstInt(Context& context, int nodeIndex, long long& value); 
	void AddError(Context& context, int code, int line, int column, const std::string & detail);
	void AddErrorNode(Context& context, int code, int nodeIndex, const std::string & detail);
	void AddBuiltin(Context& context, const char* name, AST::Type returnType, ...);




}