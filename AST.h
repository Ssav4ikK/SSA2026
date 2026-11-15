#pragma once
#include "LIBS.h"

namespace AST
{
    enum Kind
    {
        AST_PROGRAM,
        AST_MAIN,
        AST_BLOCK,
        AST_PROCEDURE,
        AST_PARAMETER,
        AST_TYPE,
        AST_ARRAY_TYPE,
        AST_VAR_DECLARATION,
        AST_ARRAY_DECLARATION,
        AST_ARRAY_INITIALIZATION,
        AST_BLOCK_INSTRUCTION,
        AST_EXPRESSION_INSTRUCTION,
        AST_ASSIGNMENT_INSTRUCTION,
        AST_PRINT_INSTRUCTION,
        AST_PRINTF_INSTRUCTION,
        AST_IF_INSTRUCTION,
        AST_ELIF_BRANCH,
        AST_ELSE_BRANCH,
        AST_SWITCH_INSTRUCTION,
        AST_CASE_BRANCH,
        AST_DEFAULT_BRANCH,
        AST_BREAK_INSTRUCTION,
        AST_BACK_INSTRUCTION,
        AST_WHILE_INSTRUCTION,
        AST_DOWHILE_INSTRUCTION,
        AST_FOR_INSTRUCTION,
        AST_FORIN_INSTRUCTION,
        AST_INCREMENT_INSTRUCTION,
        AST_DECREMENT_INSTRUCTION,
        AST_LITERAL_EXPRESSION,
        AST_VAR_EXPRESSION,
        AST_BINARY_EXPRESSION,
        AST_UNARY_EXPRESSION,
        AST_CALL_EXPRESSION,
        AST_ARRAY_ACCESS_EXPRESSION,
        AST_INPUT_EXPRESSION,
        AST_FORMATTED_STRING_EXPRESSION,
        AST_FORMAT_TEXT,
        AST_FORMAT_PLACEHOLDER
    };

    enum Type
    {
        Int8,
        Int16,
        Int32,
        Int64,
        Float32,
        Float64,
        Boolean,
        String,
        Symbol,
        Number,
        Help,
        Void,
        Array
    };

    enum Literal
    {
        LIT_INTEGER,
        LIT_FLOAT,
        LIT_BOOLEAN,
        LIT_STRING,
        LIT_FORMATTED_STRING,
        LIT_SYMBOL,
        LIT_NUMBER,
        LIT_HELP
    };

    enum BinaryOperator
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Equal,
        NotEqual,
        Less,
        Greater,
        LessEqual,
        GreaterEqual
    };

    enum UnaryOperator
    {
        Square,
        Negate,
        Not
    };

    enum AssignmentOperator
    {
        Assign,
        AddAssign,
        SubtractAssign,
        MultiplyAssign,
        DivideAssign
    };

    struct AstNode
    {
        int line = 0;
        int column = 0;
        Kind kind = AST_PROGRAM;
        Type type = Void;
        Type elementType = Void;
        Literal literalKind = LIT_INTEGER;
        BinaryOperator binaryOperator = Add;
        UnaryOperator unaryOperator = Square;
        AssignmentOperator assignmentOperator = Assign;
        std::string name;
        std::string text;
        int token_index = -1;
        std::vector<int> children;
    };

    struct AstTree
    {
        std::vector<AstNode> nodes;
        int rootIndex = -1;
    };

    int AddNode(AstTree& tree, Kind kind, int line = 0, int column = 0);
    void AddChild(AstTree& tree, int parentIndex, int childIndex);
    AstNode* GetNode(AstTree& tree, int index);
    int GetNodeCount(AstTree& tree);
    void ClearTree(AstTree& tree);

    const char* GetAstNodeName(Kind kind);
    const char* GetTypeName(Type type);
    const char* GetLiteralName(Literal literal);
    const char* GetBinaryOperatorName(BinaryOperator op);
    const char* GetUnaryOperatorName(UnaryOperator op);
    const char* GetAssignmentOperatorName(AssignmentOperator op);

    struct AstBuilder
    {
        MFST::Mfst* parser = nullptr;
        AstTree tree;

        AstBuilder();
        AstBuilder(MFST::Mfst& parserRef);

        AstTree Build();

        int BuildStrt(int nodeIndex);
        int BuildProcedure(int nodeIndex);
        int BuildParameters(int nodeIndex, int procedureIndex);
        int BuildMain(int mainIndex, int blockIndex);
        int BuildBlock(int nodeIndex);
        int BuildBlockFromBody(int bodyIndex);
        int BuildBody(int bodyIndex, int blockIndex);
        int BuildStatement(int nodeIndex);
        int BuildDeclaration(int nodeIndex);
        int BuildAssignment(int nodeIndex);
        int BuildExpression(int nodeIndex);
        int BuildComparison(int nodeIndex);
        int BuildAdditive(int nodeIndex);
        int BuildMultiplicative(int nodeIndex);
        int BuildUnary(int nodeIndex);
        int BuildSquared(int nodeIndex);
        int BuildOperand(int nodeIndex);
        int BuildCallArguments(int fcalIndex, int callIndex);
        int BuildArrayValues(int avalIndex, int initializationIndex);
        int BuildValue(int nodeIndex);
        int BuildPArg(int nodeIndex, bool isPrintf);
        int BuildPrint(int nodeIndex, bool isPrintf);
        int BuildReturn(int nodeIndex);
        int BuildCondition(int nodeIndex);
        int BuildElif(int nodeIndex, int ifIndex);
        int BuildCase(int nodeIndex, int switchIndex);
        int BuildCycle(int nodeIndex);
        int BuildStep(int nodeIndex);
        int BuildFormattedString(int nodeIndex);
        int BuildPlaceholderExpression(const std::string& text, int line, int startColumn);
        int AddNodeFromToken(int nodeIndex, Kind kind);

        Type BuildType(int nodeIndex);
        Type BuildArrayType(int nodeIndex);
        Literal GetLiteralKind(int nodeIndex);
        std::string GetIdentifier(int nodeIndex);
        std::string GetTokenText(int nodeIndex);
        std::string GetLiteralText(int nodeIndex);
        int GetLine(int nodeIndex);
        int GetColumn(int nodeIndex);
        bool IsLabel(int nodeIndex, std::string label);
        int FindChild(int nodeIndex, std::string label);
    };

    void WriteAst(LOG::Log& log, AstTree& tree);
    bool WriteAstToFile(const wchar_t* file_name, AstTree& tree);
}
