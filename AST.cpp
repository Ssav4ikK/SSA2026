#include "AST.h"
#include <charconv>

namespace AST
{
    int AddNode(AstTree& tree, Kind kind, int line, int column)
    {
        AstNode node;
        node.kind = kind;
        node.line = line;
        node.column = column;
        tree.nodes.push_back(node);
        return (int)tree.nodes.size() - 1;
    }

    void AddChild(AstTree& tree, int parentIndex, int childIndex)
    {
        if (parentIndex < 0 || parentIndex >= (int)tree.nodes.size())
            return;
        if (childIndex < 0 || childIndex >= (int)tree.nodes.size())
            return;
        tree.nodes[parentIndex].children.push_back(childIndex);
    }

    AstNode* GetNode(AstTree& tree, int index)
    {
        if (index < 0 || index >= (int)tree.nodes.size())
            return nullptr;
        return &tree.nodes[index];
    }

    int GetNodeCount(AstTree& tree)
    {
        return (int)tree.nodes.size();
    }

    void ClearTree(AstTree& tree)
    {
        tree.nodes.clear();
        tree.rootIndex = -1;
    }

    const char* GetAstNodeName(Kind kind)
    {
        switch (kind)
        {
        case AST_PROGRAM: return "Program";
        case AST_MAIN: return "Main";
        case AST_BLOCK: return "Block";
        case AST_PROCEDURE: return "Procedure";
        case AST_PARAMETER: return "Parameter";
        case AST_TYPE: return "Type";
        case AST_ARRAY_TYPE: return "ArrayType";
        case AST_VAR_DECLARATION: return "VarDeclaration";
        case AST_ARRAY_DECLARATION: return "ArrayDeclaration";
        case AST_ARRAY_INITIALIZATION: return "ArrayInitialization";
        case AST_BLOCK_INSTRUCTION: return "BlockInstruction";
        case AST_EXPRESSION_INSTRUCTION: return "ExpressionInstruction";
        case AST_ASSIGNMENT_INSTRUCTION: return "AssignmentInstruction";
        case AST_PRINT_INSTRUCTION: return "PrintInstruction";
        case AST_PRINTF_INSTRUCTION: return "PrintfInstruction";
        case AST_IF_INSTRUCTION: return "IfInstruction";
        case AST_ELIF_BRANCH: return "ElifBranch";
        case AST_ELSE_BRANCH: return "ElseBranch";
        case AST_SWITCH_INSTRUCTION: return "SwitchInstruction";
        case AST_CASE_BRANCH: return "CaseBranch";
        case AST_DEFAULT_BRANCH: return "DefaultBranch";
        case AST_BREAK_INSTRUCTION: return "BreakInstruction";
        case AST_BACK_INSTRUCTION: return "BackInstruction";
        case AST_WHILE_INSTRUCTION: return "WhileInstruction";
        case AST_DOWHILE_INSTRUCTION: return "DoWhileInstruction";
        case AST_FOR_INSTRUCTION: return "ForInstruction";
        case AST_FORIN_INSTRUCTION: return "ForInInstruction";
        case AST_INCREMENT_INSTRUCTION: return "IncrementInstruction";
        case AST_DECREMENT_INSTRUCTION: return "DecrementInstruction";
        case AST_LITERAL_EXPRESSION: return "LiteralExpression";
        case AST_VAR_EXPRESSION: return "VariableExpression";
        case AST_BINARY_EXPRESSION: return "BinaryExpression";
        case AST_UNARY_EXPRESSION: return "UnaryExpression";
        case AST_CALL_EXPRESSION: return "CallExpression";
        case AST_ARRAY_ACCESS_EXPRESSION: return "ArrayAccessExpression";
        case AST_INPUT_EXPRESSION: return "InputExpression";
        case AST_FORMATTED_STRING_EXPRESSION: return "FormattedStringExpression";
        case AST_FORMAT_TEXT: return "FormatText";
        case AST_FORMAT_PLACEHOLDER: return "FormatPlaceholder";
        default: return "Unknown";
        }
    }

    const char* GetTypeName(Type type)
    {
        switch (type)
        {
        case Int8: return "Int8";
        case Int16: return "Int16";
        case Int32: return "Int32";
        case Int64: return "Int64";
        case Float32: return "Float32";
        case Float64: return "Float64";
        case Boolean: return "Boolean";
        case String: return "String";
        case Symbol: return "Symbol";
        case Number: return "Number";
        case Help: return "Help";
        case Void: return "Void";
        case Array: return "Array";
        default: return "Unknown";
        }
    }

    const char* GetLiteralName(Literal literal)
    {
        switch (literal)
        {
        case LIT_INTEGER: return "Integer";
        case LIT_FLOAT: return "Float";
        case LIT_BOOLEAN: return "Boolean";
        case LIT_STRING: return "String";
        case LIT_FORMATTED_STRING: return "FormattedString";
        case LIT_SYMBOL: return "Symbol";
        case LIT_NUMBER: return "Number";
        case LIT_HELP: return "Help";
        default: return "Unknown";
        }
    }

    const char* GetBinaryOperatorName(BinaryOperator op)
    {
        switch (op)
        {
        case Add: return "Add";
        case Subtract: return "Subtract";
        case Multiply: return "Multiply";
        case Divide: return "Divide";
        case Equal: return "Equal";
        case NotEqual: return "NotEqual";
        case Less: return "Less";
        case Greater: return "Greater";
        case LessEqual: return "LessEqual";
        case GreaterEqual: return "GreaterEqual";
        default: return "Unknown";
        }
    }

    const char* GetUnaryOperatorName(UnaryOperator op)
    {
        switch (op)
        {
        case Square: return "Square";
        case Negate: return "Negate";
        case Not: return "Not";
        default: return "Unknown";
        }
    }

    const char* GetAssignmentOperatorName(AssignmentOperator op)
    {
        switch (op)
        {
        case Assign: return "=";
        case AddAssign: return "+=";
        case SubtractAssign: return "-=";
        case MultiplyAssign: return "*=";
        case DivideAssign: return "/=";
        default: return "Unknown";
        }
    }

    AstBuilder::AstBuilder()
    {
    }

    AstBuilder::AstBuilder(MFST::Mfst& parserRef)
    {
        parser = &parserRef;
    }

    AstTree AstBuilder::Build()
    {
        ClearTree(tree);

        if (parser == nullptr || parser->tree.empty())
            return tree;

        tree.rootIndex = AddNodeFromToken(0, AST_PROGRAM);
        BuildStrt(0);
        return tree;
    }

    bool AstBuilder::IsLabel(int nodeIndex, std::string label)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return false;
        return parser->tree[nodeIndex].label == label;
    }

    int AstBuilder::FindChild(int nodeIndex, std::string label)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        for (int childIndex : parser->tree[nodeIndex].child)
        {
            if (parser->tree[childIndex].label == label)
                return childIndex;
        }
        return -1;
    }

    int AstBuilder::GetLine(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return 0;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];

        if (node.token_index >= 0 &&
            node.token_index < parser->lexer.lextable.current_size)
        {
            return parser->lexer.lextable.table[node.token_index].str_number;
        }

        for (int childIndex : node.child)
        {
            int line = GetLine(childIndex);
            if (line > 0)
                return line;
        }
        return 0;
    }

    int AstBuilder::GetColumn(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return 0;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];

        if (node.token_index >= 0 &&
            node.token_index < parser->lexer.lextable.current_size)
        {
            return parser->lexer.lextable.table[node.token_index].col_number;
        }

        for (int childIndex : node.child)
        {
            int column = GetColumn(childIndex);
            if (column > 0)
                return column;
        }
        return 0;
    }

    int AstBuilder::AddNodeFromToken(int nodeIndex, Kind kind)
    {
        return AddNode(tree, kind, GetLine(nodeIndex), GetColumn(nodeIndex));
    }

    std::string AstBuilder::GetTokenText(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return "";

        int tokenIndex = parser->tree[nodeIndex].token_index;
        if (tokenIndex < 0 || tokenIndex >= parser->lexer.lextable.current_size)
            return parser->tree[nodeIndex].label;

        std::string text = parser->lexer.lextable.table[tokenIndex].lexema;
        while (!text.empty() && text.back() == ' ')
            text.pop_back();
        return text;
    }

    std::string AstBuilder::GetIdentifier(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return "";

        int tokenIndex = parser->tree[nodeIndex].token_index;
        if (tokenIndex < 0 || tokenIndex >= parser->lexer.lextable.current_size)
            return "";

        LEXTABLE::Entry& token = parser->lexer.lextable.table[tokenIndex];
        if (token.indexIT == LEXTABLE_IDENTIFICATORTABLE_NULLIDX)
            return GetTokenText(nodeIndex);

        return parser->lexer.idtable.table[token.indexIT].id;
    }

    static std::string FormatFloatLiteral(float value)
    {
        char buffer[64];
        std::to_chars_result result = std::to_chars(buffer, buffer + sizeof(buffer), value);

        if (result.ec != std::errc())
            return std::to_string(value);

        return std::string(buffer, result.ptr);
    }

    static std::string FormatFloatLiteral(double value)
    {
        char buffer[64];
        std::to_chars_result result = std::to_chars(buffer, buffer + sizeof(buffer), value);

        if (result.ec != std::errc())
            return std::to_string(value);

        return std::string(buffer, result.ptr);
    }

    std::string AstBuilder::GetLiteralText(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return "";

        int tokenIndex = parser->tree[nodeIndex].token_index;
        if (tokenIndex < 0 || tokenIndex >= parser->lexer.lextable.current_size)
            return "";

        LEXTABLE::Entry& token = parser->lexer.lextable.table[tokenIndex];
        if (token.indexIT == LEXTABLE_IDENTIFICATORTABLE_NULLIDX)
            return GetTokenText(nodeIndex);

        IDTABLE::Entry& id = parser->lexer.idtable.table[token.indexIT];

        switch (id.id_data_type)
        {
        case IDTABLE::INT8: return std::to_string((int)id.id_value.var_int8);
        case IDTABLE::INT16: return std::to_string(id.id_value.var_int16);
        case IDTABLE::INT32: return std::to_string(id.id_value.var_int32);
        case IDTABLE::INT64: return std::to_string(id.id_value.var_int64);
        case IDTABLE::FLOAT32: return FormatFloatLiteral(id.id_value.var_float32);
        case IDTABLE::FLOAT64: return FormatFloatLiteral(id.id_value.var_float64);
        case IDTABLE::BOOLEAN: return id.id_value.var_boolean ? "true" : "false";
        case IDTABLE::SYMBOL: return std::string(1, id.id_value.var_symbol);
        case IDTABLE::STRING: return std::string(id.id_value.var_string);
        case IDTABLE::FSTRING: return std::string(id.id_value.var_fstring);
        default: return GetTokenText(nodeIndex);
        }
    }

    Literal AstBuilder::GetLiteralKind(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return LIT_INTEGER;

        if (parser->tree[nodeIndex].label == "strl")
            return LIT_STRING;
        if (parser->tree[nodeIndex].label == "fstr")
            return LIT_FORMATTED_STRING;

        int tokenIndex = parser->tree[nodeIndex].token_index;
        if (tokenIndex < 0 || tokenIndex >= parser->lexer.lextable.current_size)
            return LIT_INTEGER;

        LEXTABLE::Entry& token = parser->lexer.lextable.table[tokenIndex];
        if (token.indexIT == LEXTABLE_IDENTIFICATORTABLE_NULLIDX)
            return LIT_INTEGER;

        IDTABLE::Entry& id = parser->lexer.idtable.table[token.indexIT];

        switch (id.id_data_type)
        {
        case IDTABLE::INT8:
        case IDTABLE::INT16:
        case IDTABLE::INT32:
        case IDTABLE::INT64:
            return LIT_INTEGER;
        case IDTABLE::FLOAT32:
        case IDTABLE::FLOAT64:
            return LIT_FLOAT;
        case IDTABLE::BOOLEAN:
            return LIT_BOOLEAN;
        case IDTABLE::STRING:
            return LIT_STRING;
        case IDTABLE::FSTRING:
            return LIT_FORMATTED_STRING;
        case IDTABLE::SYMBOL:
            return LIT_SYMBOL;
        case IDTABLE::NUMBER:
            return LIT_NUMBER;
        case IDTABLE::HELP:
            return LIT_HELP;
        default:
            return LIT_INTEGER;
        }
    }

    Type AstBuilder::BuildType(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return Void;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];

        if (node.label == "ArrT")
            return Array;

        if (node.child.empty())
            return Void;

        std::string typeName = GetTokenText(node.child[0]);

        if (typeName == "i8") return Int8;
        if (typeName == "i16") return Int16;
        if (typeName == "i32") return Int32;
        if (typeName == "i64") return Int64;
        if (typeName == "f32") return Float32;
        if (typeName == "f64") return Float64;
        if (typeName == "bool") return Boolean;
        if (typeName == "str") return String;
        if (typeName == "symb") return Symbol;
        if (typeName == "numb") return Number;
        if (typeName == "help") return Help;

        return Void;
    }

    Type AstBuilder::BuildArrayType(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return Void;

        int typeIndex = FindChild(nodeIndex, "Type");
        if (typeIndex < 0)
            return Void;

        return BuildType(typeIndex);
    }

    int AstBuilder::BuildStrt(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        int programIndex = tree.rootIndex;

        for (std::size_t i = 0; i < node.child.size(); ++i)
        {
            int childIndex = node.child[i];
            std::string label = parser->tree[childIndex].label;

            if (label == "Proc")
            {
                int procedureIndex = BuildProcedure(childIndex);
                AddChild(tree, programIndex, procedureIndex);
            }
            else if (label == "main")
            {
                int blockIndex = -1;
                for (std::size_t j = i + 1; j < node.child.size(); ++j)
                {
                    if (parser->tree[node.child[j]].label == "Blck")
                    {
                        blockIndex = node.child[j];
                        break;
                    }
                }

                int mainIndex = BuildMain(childIndex, blockIndex);
                AddChild(tree, programIndex, mainIndex);
            }
            else if (label == "Strt")
            {
                BuildStrt(childIndex);
            }
        }

        return programIndex;
    }

    int AstBuilder::BuildProcedure(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        int procedureIndex = AddNodeFromToken(nodeIndex, AST_PROCEDURE);

        for (int childIndex : node.child)
        {
            MFST::Mfst::TreeNode& child = parser->tree[childIndex];

            if (child.label == "Type")
            {
                tree.nodes[procedureIndex].type = BuildType(childIndex);
            }
            else if (child.label == "ArrT")
            {
                tree.nodes[procedureIndex].type = Array;
                tree.nodes[procedureIndex].elementType = BuildArrayType(childIndex);
            }
            else if (child.label == "idfr")
            {
                tree.nodes[procedureIndex].name = GetIdentifier(childIndex);
            }
            else if (child.label == "Prms")
            {
                BuildParameters(childIndex, procedureIndex);
            }
            else if (child.label == "Blck")
            {
                int blockIndex = BuildBlock(childIndex);
                AddChild(tree, procedureIndex, blockIndex);
            }
        }

        return procedureIndex;
    }

    int AstBuilder::BuildParameters(int nodeIndex, int procedureIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return 0;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        if (node.label == "eps")
            return 0;

        int typeIndex = -1;
        int identifierIndex = -1;

        for (int childIndex : node.child)
        {
            if (parser->tree[childIndex].label == "Type" ||
                parser->tree[childIndex].label == "ArrT")
            {
                typeIndex = childIndex;
            }
            else if (parser->tree[childIndex].label == "idfr")
            {
                identifierIndex = childIndex;
            }
        }

        if (typeIndex >= 0 && identifierIndex >= 0)
        {
            int parameterIndex = AddNodeFromToken(identifierIndex, AST_PARAMETER);
            tree.nodes[parameterIndex].name = GetIdentifier(identifierIndex);

            if (parser->tree[typeIndex].label == "ArrT")
            {
                tree.nodes[parameterIndex].type = Array;
                tree.nodes[parameterIndex].elementType = BuildArrayType(typeIndex);
            }
            else
            {
                tree.nodes[parameterIndex].type = BuildType(typeIndex);
            }

            AddChild(tree, procedureIndex, parameterIndex);
        }

        int nextParameters = FindChild(nodeIndex, "Prms");
        if (nextParameters >= 0)
            BuildParameters(nextParameters, procedureIndex);

        return procedureIndex;
    }

    int AstBuilder::BuildMain(int mainIndex, int blockIndex)
    {
        int mainNodeIndex = AddNodeFromToken(mainIndex, AST_MAIN);
        int astBlockIndex = BuildBlock(blockIndex);
        AddChild(tree, mainNodeIndex, astBlockIndex);
        return mainNodeIndex;
    }

    int AstBuilder::BuildBlock(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int blockIndex = AddNodeFromToken(nodeIndex, AST_BLOCK);
        int bodyIndex = FindChild(nodeIndex, "Body");

        if (bodyIndex >= 0)
            BuildBody(bodyIndex, blockIndex);

        return blockIndex;
    }

    int AstBuilder::BuildBlockFromBody(int bodyIndex)
    {
        int blockIndex = AddNodeFromToken(bodyIndex, AST_BLOCK);
        BuildBody(bodyIndex, blockIndex);
        return blockIndex;
    }

    int AstBuilder::BuildBody(int bodyIndex, int blockIndex)
    {
        if (parser == nullptr || bodyIndex < 0 || bodyIndex >= (int)parser->tree.size())
            return blockIndex;

        if (parser->tree[bodyIndex].label == "eps")
            return blockIndex;

        int statementIndex = FindChild(bodyIndex, "Inst");
        if (statementIndex < 0)
            statementIndex = FindChild(bodyIndex, "Stmt");
        if (statementIndex >= 0)
        {
            int astStatementIndex = BuildStatement(statementIndex);
            if (astStatementIndex >= 0)
                AddChild(tree, blockIndex, astStatementIndex);
        }

        int nextBodyIndex = FindChild(bodyIndex, "Body");
        if (nextBodyIndex >= 0)
            BuildBody(nextBodyIndex, blockIndex);

        return blockIndex;
    }

    int AstBuilder::BuildStatement(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int declaration = FindChild(nodeIndex, "Decl");
        if (declaration >= 0)
            return BuildDeclaration(declaration);

        int assignment = FindChild(nodeIndex, "Asgn");
        if (assignment >= 0)
            return BuildAssignment(assignment);

        int condition = FindChild(nodeIndex, "Cond");
        if (condition >= 0)
            return BuildCondition(condition);

        int cycle = FindChild(nodeIndex, "Cycl");
        if (cycle >= 0)
            return BuildCycle(cycle);

        if (FindChild(nodeIndex, "prn") >= 0)
            return BuildPrint(nodeIndex, false);

        if (FindChild(nodeIndex, "prnf") >= 0)
            return BuildPrint(nodeIndex, true);

        if (FindChild(nodeIndex, "back") >= 0)
            return BuildReturn(nodeIndex);

        if (FindChild(nodeIndex, "brek") >= 0)
            return AddNodeFromToken(nodeIndex, AST_BREAK_INSTRUCTION);

        int typeChild = FindChild(nodeIndex, "Type");
        int identifierChild = FindChild(nodeIndex, "idfr");
        int inputChild = FindChild(nodeIndex, "inpt");
        int stepChild = FindChild(nodeIndex, "Step");
        int exprChild = FindChild(nodeIndex, "Expr");

        if (typeChild >= 0 && identifierChild >= 0 && inputChild >= 0)
        {
            int declarationIndex = AddNodeFromToken(identifierChild, AST_VAR_DECLARATION);
            tree.nodes[declarationIndex].name = GetIdentifier(identifierChild);
            tree.nodes[declarationIndex].type = BuildType(typeChild);
            int inputIndex = AddNodeFromToken(inputChild, AST_INPUT_EXPRESSION);
            AddChild(tree, declarationIndex, inputIndex);
            return declarationIndex;
        }

        if (identifierChild >= 0 && inputChild >= 0)
        {
            int assignmentIndex = AddNodeFromToken(identifierChild, AST_ASSIGNMENT_INSTRUCTION);
            int variableIndex = AddNodeFromToken(identifierChild, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(identifierChild);
            int inputIndex = AddNodeFromToken(inputChild, AST_INPUT_EXPRESSION);
            AddChild(tree, assignmentIndex, variableIndex);
            AddChild(tree, assignmentIndex, inputIndex);
            tree.nodes[assignmentIndex].assignmentOperator = Assign;
            return assignmentIndex;
        }

        if (typeChild >= 0 && identifierChild >= 0 && stepChild >= 0)
        {
            int declarationIndex = AddNodeFromToken(identifierChild, AST_VAR_DECLARATION);
            tree.nodes[declarationIndex].name = GetIdentifier(identifierChild);
            tree.nodes[declarationIndex].type = BuildType(typeChild);
            int valueIndex = BuildStep(stepChild);
            if (valueIndex >= 0)
                AddChild(tree, declarationIndex, valueIndex);
            return declarationIndex;
        }

        if (identifierChild >= 0 && stepChild >= 0)
        {
            int assignmentIndex = AddNodeFromToken(identifierChild, AST_ASSIGNMENT_INSTRUCTION);
            int variableIndex = AddNodeFromToken(identifierChild, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(identifierChild);
            int valueIndex = BuildStep(stepChild);
            AddChild(tree, assignmentIndex, variableIndex);
            if (valueIndex >= 0)
                AddChild(tree, assignmentIndex, valueIndex);
            tree.nodes[assignmentIndex].assignmentOperator = Assign;
            return assignmentIndex;
        }

        if (exprChild >= 0)
        {
            int expressionIndex = BuildExpression(exprChild);
            int statementIndex = AddNodeFromToken(exprChild, AST_EXPRESSION_INSTRUCTION);
            AddChild(tree, statementIndex, expressionIndex);
            return statementIndex;
        }

        if (stepChild >= 0)
            return BuildStep(stepChild);

        return -1;
    }

    int AstBuilder::BuildDeclaration(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        int arrayTypeIndex = FindChild(nodeIndex, "ArrT");
        int typeIndex = FindChild(nodeIndex, "Type");
        int identifierIndex = FindChild(nodeIndex, "idfr");

        if (identifierIndex < 0)
            return -1;

        if (arrayTypeIndex >= 0)
        {
            int declarationIndex = AddNodeFromToken(identifierIndex, AST_ARRAY_DECLARATION);
            tree.nodes[declarationIndex].name = GetIdentifier(identifierIndex);
            tree.nodes[declarationIndex].type = Array;
            tree.nodes[declarationIndex].elementType = BuildArrayType(arrayTypeIndex);

            int expressionIndex = FindChild(nodeIndex, "Expr");
            if (expressionIndex >= 0)
            {
                int sizeIndex = BuildExpression(expressionIndex);
                AddChild(tree, declarationIndex, sizeIndex);
            }

            int avalIndex = FindChild(nodeIndex, "AVal");
            if (avalIndex >= 0)
            {
                int initializationIndex = AddNodeFromToken(avalIndex, AST_ARRAY_INITIALIZATION);
                BuildArrayValues(avalIndex, initializationIndex);
                AddChild(tree, declarationIndex, initializationIndex);
            }

            return declarationIndex;
        }

        int declarationIndex = AddNodeFromToken(identifierIndex, AST_VAR_DECLARATION);
        tree.nodes[declarationIndex].name = GetIdentifier(identifierIndex);
        tree.nodes[declarationIndex].type = BuildType(typeIndex);

        int expressionIndex = FindChild(nodeIndex, "Expr");
        if (expressionIndex >= 0)
        {
            int initializerIndex = BuildExpression(expressionIndex);
            AddChild(tree, declarationIndex, initializerIndex);
        }

        int avalIndex = FindChild(nodeIndex, "AVal");
        if (avalIndex >= 0)
        {
            int initializationIndex = AddNodeFromToken(avalIndex, AST_ARRAY_INITIALIZATION);
            BuildArrayValues(avalIndex, initializationIndex);
            AddChild(tree, declarationIndex, initializationIndex);
        }

        return declarationIndex;
    }

    int AstBuilder::BuildAssignment(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        int identifierIndex = FindChild(nodeIndex, "idfr");
        if (identifierIndex < 0)
            return -1;

        int assignmentIndex = AddNodeFromToken(identifierIndex, AST_ASSIGNMENT_INSTRUCTION);
        int variableIndex = AddNodeFromToken(identifierIndex, AST_VAR_EXPRESSION);
        tree.nodes[variableIndex].name = GetIdentifier(identifierIndex);

        int targetIndex = variableIndex;
        int leftBracket = FindChild(nodeIndex, "[");
        int expressionCount = 0;
        int firstExpression = -1;
        int lastExpression = -1;

        for (int childIndex : node.child)
        {
            MFST::Mfst::TreeNode& child = parser->tree[childIndex];
            if (child.label == "Expr")
            {
                if (firstExpression < 0)
                    firstExpression = childIndex;
                lastExpression = childIndex;
                ++expressionCount;
            }
        }

        if (leftBracket >= 0 && firstExpression >= 0)
        {
            int arrayAccessIndex = AddNodeFromToken(identifierIndex, AST_ARRAY_ACCESS_EXPRESSION);
            int indexExpression = BuildExpression(firstExpression);
            AddChild(tree, arrayAccessIndex, variableIndex);
            AddChild(tree, arrayAccessIndex, indexExpression);
            targetIndex = arrayAccessIndex;
        }

        int valueExpression = expressionCount > 1 ? lastExpression : firstExpression;
        int valueIndex = -1;
        if (valueExpression >= 0)
            valueIndex = BuildExpression(valueExpression);

        if (valueIndex < 0)
        {
            int avalIndex = FindChild(nodeIndex, "AVal");
            if (avalIndex >= 0)
            {
                valueIndex = AddNodeFromToken(avalIndex, AST_ARRAY_INITIALIZATION);
                BuildArrayValues(avalIndex, valueIndex);
            }
        }

        AssignmentOperator operation = Assign;

        for (int childIndex : node.child)
        {
            std::string label = parser->tree[childIndex].label;
            if (label == "=") operation = Assign;
            else if (label == "+=") operation = AddAssign;
            else if (label == "-=") operation = SubtractAssign;
            else if (label == "*=") operation = MultiplyAssign;
            else if (label == "/=") operation = DivideAssign;
        }

        tree.nodes[assignmentIndex].assignmentOperator = operation;
        AddChild(tree, assignmentIndex, targetIndex);
        if (valueIndex >= 0)
            AddChild(tree, assignmentIndex, valueIndex);
        return assignmentIndex;
    }

    static bool IsEpsilonChain(const MFST::Mfst& parser, int nodeIndex)
    {
        if (nodeIndex < 0 || nodeIndex >= (int)parser.tree.size())
            return true;

        const MFST::Mfst::TreeNode& node = parser.tree[nodeIndex];
        if (node.child.empty())
            return true;

        return parser.tree[node.child[0]].label == "eps";
    }

    static BinaryOperator BinaryOperatorFromLabel(const std::string& label)
    {
        if (label == "-") return Subtract;
        if (label == "*") return Multiply;
        if (label == "/") return Divide;
        if (label == "==") return Equal;
        if (label == "!=") return NotEqual;
        if (label == "<") return Less;
        if (label == ">") return Greater;
        if (label == "<=") return LessEqual;
        if (label == ">=") return GreaterEqual;
        return Add;
    }

    int AstBuilder::BuildExpression(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        if (parser->tree[nodeIndex].label == "Cmpd")
            return BuildComparison(nodeIndex);

        int comparisonIndex = FindChild(nodeIndex, "Cmpd");
        if (comparisonIndex < 0)
            return -1;

        return BuildComparison(comparisonIndex);
    }

    int AstBuilder::BuildComparison(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int additiveIndex = FindChild(nodeIndex, "Summ");
        if (additiveIndex < 0)
            return -1;

        int leftIndex = BuildAdditive(additiveIndex);
        if (leftIndex < 0)
            return -1;

        int tailIndex = FindChild(nodeIndex, "CmpT");

        while (tailIndex >= 0 && !IsEpsilonChain(*parser, tailIndex))
        {
            MFST::Mfst::TreeNode& tail = parser->tree[tailIndex];
            int operatorToken = tail.child[0];
            int rightAdditive = FindChild(tailIndex, "Summ");
            if (rightAdditive < 0)
                break;

            int rightIndex = BuildAdditive(rightAdditive);
            int binaryIndex = AddNodeFromToken(operatorToken, AST_BINARY_EXPRESSION);
            tree.nodes[binaryIndex].binaryOperator =
                BinaryOperatorFromLabel(parser->tree[operatorToken].label);

            AddChild(tree, binaryIndex, leftIndex);
            if (rightIndex >= 0)
                AddChild(tree, binaryIndex, rightIndex);

            leftIndex = binaryIndex;
            tailIndex = FindChild(tailIndex, "CmpT");
        }

        return leftIndex;
    }

    int AstBuilder::BuildAdditive(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int multiplicativeIndex = FindChild(nodeIndex, "Mult");
        if (multiplicativeIndex < 0)
            return -1;

        int leftIndex = BuildMultiplicative(multiplicativeIndex);
        if (leftIndex < 0)
            return -1;

        int tailIndex = FindChild(nodeIndex, "SumT");

        while (tailIndex >= 0 && !IsEpsilonChain(*parser, tailIndex))
        {
            MFST::Mfst::TreeNode& tail = parser->tree[tailIndex];
            int operatorToken = tail.child[0];
            int rightMultiplicative = FindChild(tailIndex, "Mult");
            if (rightMultiplicative < 0)
                break;

            int rightIndex = BuildMultiplicative(rightMultiplicative);
            int binaryIndex = AddNodeFromToken(operatorToken, AST_BINARY_EXPRESSION);
            tree.nodes[binaryIndex].binaryOperator =
                BinaryOperatorFromLabel(parser->tree[operatorToken].label);

            AddChild(tree, binaryIndex, leftIndex);
            if (rightIndex >= 0)
                AddChild(tree, binaryIndex, rightIndex);

            leftIndex = binaryIndex;
            tailIndex = FindChild(tailIndex, "SumT");
        }

        return leftIndex;
    }

    int AstBuilder::BuildMultiplicative(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int unaryIndex = FindChild(nodeIndex, "Unry");
        if (unaryIndex < 0)
            return -1;

        int leftIndex = BuildUnary(unaryIndex);
        if (leftIndex < 0)
            return -1;

        int tailIndex = FindChild(nodeIndex, "MulT");

        while (tailIndex >= 0 && !IsEpsilonChain(*parser, tailIndex))
        {
            MFST::Mfst::TreeNode& tail = parser->tree[tailIndex];
            int operatorToken = tail.child[0];
            int rightUnary = FindChild(tailIndex, "Unry");
            if (rightUnary < 0)
                break;

            int rightIndex = BuildUnary(rightUnary);
            int binaryIndex = AddNodeFromToken(operatorToken, AST_BINARY_EXPRESSION);
            tree.nodes[binaryIndex].binaryOperator =
                BinaryOperatorFromLabel(parser->tree[operatorToken].label);

            AddChild(tree, binaryIndex, leftIndex);
            if (rightIndex >= 0)
                AddChild(tree, binaryIndex, rightIndex);

            leftIndex = binaryIndex;
            tailIndex = FindChild(tailIndex, "MulT");
        }

        return leftIndex;
    }

    int AstBuilder::BuildUnary(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        if (node.child.empty())
            return -1;

        std::string label = parser->tree[node.child[0]].label;

        if (label == "-" || label == "!")
        {
            if (node.child.size() < 2)
                return -1;

            int operandIndex = BuildUnary(node.child[1]);
            if (operandIndex < 0)
                return -1;

            int unaryIndex = AddNodeFromToken(node.child[0], AST_UNARY_EXPRESSION);
            tree.nodes[unaryIndex].unaryOperator = (label == "-") ? Negate : Not;
            AddChild(tree, unaryIndex, operandIndex);
            return unaryIndex;
        }

        int squaredIndex = FindChild(nodeIndex, "Sqre");
        return BuildSquared(squaredIndex);
    }

    int AstBuilder::BuildSquared(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int operandIndex = FindChild(nodeIndex, "Opnd");
        if (operandIndex < 0)
            return -1;

        int leftIndex = BuildOperand(operandIndex);
        if (leftIndex < 0)
            return -1;

        int tailIndex = FindChild(nodeIndex, "SqrT");

        while (tailIndex >= 0 && !IsEpsilonChain(*parser, tailIndex))
        {
            MFST::Mfst::TreeNode& tail = parser->tree[tailIndex];
            int cubeToken = tail.child[0];

            int unaryIndex = AddNodeFromToken(cubeToken, AST_UNARY_EXPRESSION);
            tree.nodes[unaryIndex].unaryOperator = Square;
            AddChild(tree, unaryIndex, leftIndex);

            leftIndex = unaryIndex;
            tailIndex = FindChild(tailIndex, "SqrT");
        }

        return leftIndex;
    }

    int AstBuilder::BuildOperand(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        if (node.child.empty())
            return -1;

        int firstChild = node.child[0];
        std::string label = parser->tree[firstChild].label;

        if (label == "idfr")
        {
            if (node.child.size() > 1 && parser->tree[node.child[1]].label == "(")
            {
                int callIndex = AddNodeFromToken(firstChild, AST_CALL_EXPRESSION);
                tree.nodes[callIndex].name = GetIdentifier(firstChild);
                int fcalIndex = FindChild(nodeIndex, "PCal");
                if (fcalIndex >= 0)
                    BuildCallArguments(fcalIndex, callIndex);
                return callIndex;
            }

            int variableIndex = AddNodeFromToken(firstChild, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(firstChild);
            return variableIndex;
        }

        if (label == "lter")
        {
            int literalIndex = AddNodeFromToken(firstChild, AST_LITERAL_EXPRESSION);
            tree.nodes[literalIndex].literalKind = GetLiteralKind(firstChild);
            tree.nodes[literalIndex].text = GetLiteralText(firstChild);
            return literalIndex;
        }

        if (label == "strl")
        {
            int literalIndex = AddNodeFromToken(firstChild, AST_LITERAL_EXPRESSION);
            tree.nodes[literalIndex].literalKind = LIT_STRING;
            tree.nodes[literalIndex].text = GetLiteralText(firstChild);
            return literalIndex;
        }

        if (label == "fstr")
        {
            return BuildFormattedString(firstChild);
        }

        if (label == "(")
        {
            int expressionIndex = FindChild(nodeIndex, "Expr");
            return BuildExpression(expressionIndex);
        }

        if (label == "inpt")
        {
            return AddNodeFromToken(firstChild, AST_INPUT_EXPRESSION);
        }

        return -1;
    }

    int AstBuilder::BuildCallArguments(int fcalIndex, int callIndex)
    {
        if (parser == nullptr || fcalIndex < 0 || fcalIndex >= (int)parser->tree.size())
            return callIndex;

        if (parser->tree[fcalIndex].label == "eps")
            return callIndex;

        int expressionIndex = FindChild(fcalIndex, "Expr");
        if (expressionIndex >= 0)
        {
            int argumentIndex = BuildExpression(expressionIndex);
            AddChild(tree, callIndex, argumentIndex);
        }

        int nextFcalIndex = FindChild(fcalIndex, "PCal");
        if (nextFcalIndex >= 0)
            BuildCallArguments(nextFcalIndex, callIndex);

        return callIndex;
    }

    int AstBuilder::BuildArrayValues(int avalIndex, int initializationIndex)
    {
        if (parser == nullptr || avalIndex < 0 || avalIndex >= (int)parser->tree.size())
            return initializationIndex;

        if (parser->tree[avalIndex].label == "eps")
            return initializationIndex;

        for (int childIndex : parser->tree[avalIndex].child)
        {
            std::string label = parser->tree[childIndex].label;

            if (label == "lter")
            {
                int valueIndex = AddNodeFromToken(childIndex, AST_LITERAL_EXPRESSION);
                tree.nodes[valueIndex].literalKind = GetLiteralKind(childIndex);
                tree.nodes[valueIndex].text = GetLiteralText(childIndex);
                AddChild(tree, initializationIndex, valueIndex);
            }
            else if (label == "idfr")
            {
                int valueIndex = AddNodeFromToken(childIndex, AST_VAR_EXPRESSION);
                tree.nodes[valueIndex].name = GetIdentifier(childIndex);
                AddChild(tree, initializationIndex, valueIndex);
            }
            else if (label == "AVal")
            {
                BuildArrayValues(childIndex, initializationIndex);
            }
        }

        return initializationIndex;
    }

    int AstBuilder::BuildValue(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        std::string label = parser->tree[nodeIndex].label;

        if (label == "idfr")
        {
            int valueIndex = AddNodeFromToken(nodeIndex, AST_VAR_EXPRESSION);
            tree.nodes[valueIndex].name = GetIdentifier(nodeIndex);
            return valueIndex;
        }

        if (label == "lter")
        {
            int valueIndex = AddNodeFromToken(nodeIndex, AST_LITERAL_EXPRESSION);
            tree.nodes[valueIndex].literalKind = GetLiteralKind(nodeIndex);
            tree.nodes[valueIndex].text = GetLiteralText(nodeIndex);
            return valueIndex;
        }

        if (label == "strl")
        {
            int valueIndex = AddNodeFromToken(nodeIndex, AST_LITERAL_EXPRESSION);
            tree.nodes[valueIndex].literalKind = LIT_STRING;
            tree.nodes[valueIndex].text = GetLiteralText(nodeIndex);
            return valueIndex;
        }

        return -1;
    }

    static bool IsFormatDigit(char symbol)
    {
        return symbol >= '0' && symbol <= '9';
    }

    static bool IsFormatIdentifierStart(char symbol)
    {
        return (symbol >= 'a' && symbol <= 'z') ||
            (symbol >= 'A' && symbol <= 'Z') || symbol == '_';
    }

    static bool IsFormatIdentifierChar(char symbol)
    {
        return IsFormatIdentifierStart(symbol) || IsFormatDigit(symbol);
    }

    struct PlaceholderParser
    {
        const std::string& text;
        std::size_t position;
        int base_line;
        int base_column;
        AstTree& tree;

        PlaceholderParser(const std::string& source, int line_number, int start_column, AstTree& tree_ref)
            : text(source), position(0), base_line(line_number), base_column(start_column), tree(tree_ref)
        {
        }

        bool AtEnd() const
        {
            return position >= text.size();
        }

        char Current() const
        {
            return position < text.size() ? text[position] : '\0';
        }

        int CurrentLine() const
        {
            int value = base_line;

            for (size_t index = 0; index < position && index < text.size(); ++index)
            {
                if (text[index] == '\n')
                    ++value;
            }

            return value;
        }

        int CurrentColumn() const
        {
            int value = base_column;

            for (size_t index = 0; index < position && index < text.size(); ++index)
            {
                if (text[index] == '\n')
                    value = 1;
                else
                    ++value;
            }

            return value;
        }

        [[noreturn]] void Fail() const
        {
            throw ERROR_THROW_PLACE(124, CurrentLine(), CurrentColumn());
        }

        void SkipSpaces()
        {
            while (!AtEnd())
            {
                char symbol = text[position];
                if (symbol != ' ' && symbol != '\t' && symbol != '\r' && symbol != '\n')
                    break;
                ++position;
            }
        }

        bool Match(const char* token)
        {
            size_t length = strlen(token);

            if (text.compare(position, length, token) == 0)
            {
                position += length;
                return true;
            }

            return false;
        }

        int MakeLiteral(const std::string& value, Literal kind, int line, int column)
        {
            int index = AddNode(tree, AST_LITERAL_EXPRESSION, line, column);
            tree.nodes[index].literalKind = kind;
            tree.nodes[index].text = value;
            return index;
        }

        int MakeBinary(BinaryOperator op, int left, int right, int line, int column)
        {
            int index = AddNode(tree, AST_BINARY_EXPRESSION, line, column);
            tree.nodes[index].binaryOperator = op;
            AddChild(tree, index, left);
            AddChild(tree, index, right);
            return index;
        }

        int MakeUnary(UnaryOperator op, int operand, int line, int column)
        {
            int index = AddNode(tree, AST_UNARY_EXPRESSION, line, column);
            tree.nodes[index].unaryOperator = op;
            AddChild(tree, index, operand);
            return index;
        }

        int ParseExpression()
        {
            return ParseComparison();
        }

        int ParseComparison()
        {
            int left = ParseAdditive();

            while (true)
            {
                SkipSpaces();
                int line = CurrentLine();
                int column = CurrentColumn();
                BinaryOperator op = Add;

                if (Match("==")) op = Equal;
                else if (Match("!=")) op = NotEqual;
                else if (Match("<=")) op = LessEqual;
                else if (Match(">=")) op = GreaterEqual;
                else if (Match("<")) op = Less;
                else if (Match(">")) op = Greater;
                else break;

                int right = ParseAdditive();
                left = MakeBinary(op, left, right, line, column);
            }

            return left;
        }

        int ParseAdditive()
        {
            int left = ParseMultiplicative();

            while (true)
            {
                SkipSpaces();
                int line = CurrentLine();
                int column = CurrentColumn();
                BinaryOperator op = Add;

                if (Match("+")) op = Add;
                else if (Match("-")) op = Subtract;
                else break;

                int right = ParseMultiplicative();
                left = MakeBinary(op, left, right, line, column);
            }

            return left;
        }

        int ParseMultiplicative()
        {
            int left = ParseUnary();

            while (true)
            {
                SkipSpaces();
                int line = CurrentLine();
                int column = CurrentColumn();
                BinaryOperator op = Multiply;

                if (Match("*")) op = Multiply;
                else if (Match("/")) op = Divide;
                else break;

                int right = ParseUnary();
                left = MakeBinary(op, left, right, line, column);
            }

            return left;
        }

        int ParseUnary()
        {
            SkipSpaces();
            int line = CurrentLine();
            int column = CurrentColumn();

            if (Match("-"))
                return MakeUnary(Negate, ParseUnary(), line, column);

            if (Match("!"))
                return MakeUnary(Not, ParseUnary(), line, column);

            return ParseSquare();
        }

        int ParseSquare()
        {
            int value = ParsePrimary();

            while (true)
            {
                SkipSpaces();
                int line = CurrentLine();
                int column = CurrentColumn();

                if (!Match("^2"))
                    break;

                value = MakeUnary(Square, value, line, column);
            }

            return value;
        }

        int ParseIdentifier()
        {
            int line = CurrentLine();
            int column = CurrentColumn();
            size_t start = position;

            while (!AtEnd() && IsFormatIdentifierChar(Current()))
                ++position;

            std::string name = text.substr(start, position - start);

            if (name == "true" || name == "false")
                return MakeLiteral(name, LIT_BOOLEAN, line, column);

            SkipSpaces();

            if (name == "input" && Current() == '(')
            {
                ++position;
                SkipSpaces();

                if (!Match(")"))
                    Fail();

                return AddNode(tree, AST_INPUT_EXPRESSION, line, column);
            }

            if (Current() == '(')
            {
                ++position;

                int callIndex = AddNode(tree, AST_CALL_EXPRESSION, line, column);
                tree.nodes[callIndex].name = name;

                SkipSpaces();

                if (!Match(")"))
                {
                    while (true)
                    {
                        int argument = ParseExpression();
                        AddChild(tree, callIndex, argument);
                        SkipSpaces();

                        if (Match(","))
                            continue;

                        if (Match(")"))
                            break;

                        Fail();
                    }
                }

                return callIndex;
            }

            if (Current() == '[')
            {
                ++position;

                int indexExpression = ParseExpression();
                SkipSpaces();

                if (!Match("]"))
                    Fail();

                int accessIndex = AddNode(tree, AST_ARRAY_ACCESS_EXPRESSION, line, column);
                int variableIndex = AddNode(tree, AST_VAR_EXPRESSION, line, column);
                tree.nodes[variableIndex].name = name;
                AddChild(tree, accessIndex, variableIndex);
                AddChild(tree, accessIndex, indexExpression);
                return accessIndex;
            }

            int variableIndex = AddNode(tree, AST_VAR_EXPRESSION, line, column);
            tree.nodes[variableIndex].name = name;
            return variableIndex;
        }

        int ParseSymbolLiteral()
        {
            int line = CurrentLine();
            int column = CurrentColumn();
            ++position;

            if (AtEnd())
                Fail();

            char symbol = text[position++];

            if (!Match("'"))
                Fail();

            return MakeLiteral(std::string(1, symbol), LIT_SYMBOL, line, column);
        }

        int ParseNumber()
        {
            int line = CurrentLine();
            int column = CurrentColumn();

            if (text[position] == '0' && position + 1 < text.size() &&
                (text[position + 1] == 'o' || text[position + 1] == 'O'))
            {
                position += 2;
                size_t digits_start = position;

                while (!AtEnd() && IsFormatDigit(Current()))
                {
                    if (Current() > '7')
                        Fail();
                    ++position;
                }

                if (position == digits_start)
                    Fail();

                std::string digits = text.substr(digits_start, position - digits_start);
                long long value = 0;
                std::from_chars_result result = std::from_chars(
                    digits.data(), digits.data() + digits.size(), value, 8);

                if (result.ec != std::errc())
                    Fail();

                return MakeLiteral(std::to_string(value), LIT_INTEGER, line, column);
            }

            size_t digits_start = position;
            bool has_dot = false;

            while (!AtEnd())
            {
                char symbol = Current();

                if (IsFormatDigit(symbol))
                {
                    ++position;
                    continue;
                }

                if (symbol == '.' && !has_dot)
                {
                    has_dot = true;
                    ++position;
                    continue;
                }

                break;
            }

            if (position == digits_start)
                Fail();

            std::string token = text.substr(digits_start, position - digits_start);

            if (has_dot)
            {
                double value = 0.0;
                std::from_chars_result result = std::from_chars(
                    token.data(), token.data() + token.size(), value, std::chars_format::general);

                if (result.ec != std::errc() || result.ptr != token.data() + token.size())
                    Fail();

                return MakeLiteral(FormatFloatLiteral(value), LIT_FLOAT, line, column);
            }

            int base = 10;
            std::string digits = token;

            if (token.size() > 1 && token[0] == '0')
            {
                base = 8;
                digits = token.substr(1);
            }

            for (size_t index = 0; index < digits.size(); ++index)
            {
                if (!IsFormatDigit(digits[index]))
                    Fail();

                if (base == 8 && digits[index] > '7')
                    Fail();
            }

            long long value = 0;
            std::from_chars_result result = std::from_chars(
                digits.data(), digits.data() + digits.size(), value, base);

            if (result.ec != std::errc())
                Fail();

            return MakeLiteral(std::to_string(value), LIT_INTEGER, line, column);
        }

        int ParsePrimary()
        {
            SkipSpaces();

            if (AtEnd())
                Fail();

            char symbol = Current();

            if (symbol == '(')
            {
                ++position;

                int value = ParseExpression();
                SkipSpaces();

                if (!Match(")"))
                    Fail();

                return value;
            }

            if (IsFormatDigit(symbol))
                return ParseNumber();

            if (symbol == '\'')
                return ParseSymbolLiteral();

            if (IsFormatIdentifierStart(symbol))
                return ParseIdentifier();

            Fail();
        }
    };

    int AstBuilder::BuildPlaceholderExpression(const std::string& text, int line, int start_column)
    {
        PlaceholderParser parser(text, line, start_column, tree);
        int index = parser.ParseExpression();

        parser.SkipSpaces();

        if (!parser.AtEnd())
            parser.Fail();

        return index;
    }

    static void MapTextPosition(const std::string& text, size_t offset,
        int start_line, int start_column, int& line, int& column)
    {
        line = start_line;
        column = start_column;

        for (size_t index = 0; index < offset && index < text.size(); ++index)
        {
            if (text[index] == '\n')
            {
                ++line;
                column = 1;
            }
            else
            {
                ++column;
            }
        }
    }

    int AstBuilder::BuildFormattedString(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        std::string text = GetLiteralText(nodeIndex);
        int formattedIndex = AddNodeFromToken(nodeIndex, AST_FORMATTED_STRING_EXPRESSION);

        int base_line = GetLine(nodeIndex);
        int base_column = GetColumn(nodeIndex) + 1;

        size_t position = 0;

        while (position < text.size())
        {
            size_t open = text.find('{', position);

            if (open == std::string::npos)
            {
                int text_line = 0;
                int text_column = 0;
                MapTextPosition(text, position, base_line, base_column, text_line, text_column);

                int textIndex = AddNode(tree, AST_FORMAT_TEXT, text_line, text_column);
                tree.nodes[textIndex].text = text.substr(position);
                AddChild(tree, formattedIndex, textIndex);
                break;
            }

            if (open > position)
            {
                int text_line = 0;
                int text_column = 0;
                MapTextPosition(text, position, base_line, base_column, text_line, text_column);

                int textIndex = AddNode(tree, AST_FORMAT_TEXT, text_line, text_column);
                tree.nodes[textIndex].text = text.substr(position, open - position);
                AddChild(tree, formattedIndex, textIndex);
            }

            int open_line = 0;
            int open_column = 0;
            MapTextPosition(text, open, base_line, base_column, open_line, open_column);

            size_t close = text.find('}', open + 1);
            if (close == std::string::npos)
                throw ERROR_THROW_PLACE(124, open_line, open_column);

            std::string expression_text = text.substr(open + 1, close - open - 1);

            int placeholderIndex = AddNode(tree, AST_FORMAT_PLACEHOLDER, open_line, open_column);
            tree.nodes[placeholderIndex].text = text.substr(open, close - open + 1);

            int expression_line = 0;
            int expression_column = 0;
            MapTextPosition(text, open + 1, base_line, base_column, expression_line, expression_column);

            int expressionIndex = BuildPlaceholderExpression(
                expression_text,
                expression_line,
                expression_column);

            AddChild(tree, placeholderIndex, expressionIndex);
            AddChild(tree, formattedIndex, placeholderIndex);

            position = close + 1;
        }

        return formattedIndex;
    }

    int AstBuilder::BuildPArg(int nodeIndex, bool isPrintf)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int expressionIndex = FindChild(nodeIndex, "Expr");
        if (expressionIndex >= 0)
            return BuildExpression(expressionIndex);

        int formattedIndex = FindChild(nodeIndex, "fstr");
        if (formattedIndex >= 0)
        {
            if (isPrintf)
                return BuildFormattedString(formattedIndex);

            int literalIndex = AddNodeFromToken(formattedIndex, AST_LITERAL_EXPRESSION);
            tree.nodes[literalIndex].literalKind = LIT_FORMATTED_STRING;
            tree.nodes[literalIndex].text = GetLiteralText(formattedIndex);
            return literalIndex;
        }

        return -1;
    }

    int AstBuilder::BuildPrint(int nodeIndex, bool isPrintf)
    {
        int printIndex = AddNodeFromToken(
            nodeIndex,
            isPrintf ? AST_PRINTF_INSTRUCTION : AST_PRINT_INSTRUCTION);

        int argumentIndex = FindChild(nodeIndex, "PArg");
        if (argumentIndex >= 0)
        {
            int valueIndex = BuildPArg(argumentIndex, isPrintf);
            if (valueIndex >= 0)
                AddChild(tree, printIndex, valueIndex);
        }

        return printIndex;
    }

    int AstBuilder::BuildReturn(int nodeIndex)
    {
        int returnIndex = AddNodeFromToken(nodeIndex, AST_BACK_INSTRUCTION);
        int expressionIndex = FindChild(nodeIndex, "Expr");
        if (expressionIndex >= 0)
        {
            int valueIndex = BuildExpression(expressionIndex);
            AddChild(tree, returnIndex, valueIndex);
        }
        return returnIndex;
    }

    int AstBuilder::BuildCondition(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        if (node.child.empty())
            return -1;

        std::string label = parser->tree[node.child[0]].label;

        if (label == "if")
        {
            int ifIndex = AddNodeFromToken(nodeIndex, AST_IF_INSTRUCTION);
            int conditionIndex = FindChild(nodeIndex, "Expr");
            if (conditionIndex >= 0)
                AddChild(tree, ifIndex, BuildExpression(conditionIndex));

            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, ifIndex, BuildBlockFromBody(bodyIndex));

            int elifIndex = FindChild(nodeIndex, "Elif");
            if (elifIndex >= 0)
                BuildElif(elifIndex, ifIndex);

            return ifIndex;
        }

        if (label == "swch")
        {
            int switchIndex = AddNodeFromToken(nodeIndex, AST_SWITCH_INSTRUCTION);
            int valueIndex = FindChild(nodeIndex, "Expr");
            if (valueIndex >= 0)
                AddChild(tree, switchIndex, BuildExpression(valueIndex));

            int caseIndex = FindChild(nodeIndex, "Case");
            if (caseIndex >= 0)
                BuildCase(caseIndex, switchIndex);

            return switchIndex;
        }

        return -1;
    }

    int AstBuilder::BuildElif(int nodeIndex, int ifIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return ifIndex;

        if (parser->tree[nodeIndex].label == "eps")
            return ifIndex;

        int firstChild = parser->tree[nodeIndex].child.empty()
            ? -1
            : parser->tree[nodeIndex].child[0];

        if (firstChild < 0)
            return ifIndex;

        std::string label = parser->tree[firstChild].label;

        if (label == "elif")
        {
            int branchIndex = AddNodeFromToken(nodeIndex, AST_ELIF_BRANCH);
            int conditionIndex = FindChild(nodeIndex, "Expr");
            if (conditionIndex >= 0)
                AddChild(tree, branchIndex, BuildExpression(conditionIndex));

            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, branchIndex, BuildBlockFromBody(bodyIndex));

            AddChild(tree, ifIndex, branchIndex);

            int nextElif = FindChild(nodeIndex, "Elif");
            if (nextElif >= 0)
                BuildElif(nextElif, ifIndex);
        }
        else if (label == "else")
        {
            int branchIndex = AddNodeFromToken(nodeIndex, AST_ELSE_BRANCH);
            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, branchIndex, BuildBlockFromBody(bodyIndex));
            AddChild(tree, ifIndex, branchIndex);
        }

        return ifIndex;
    }

    int AstBuilder::BuildCase(int nodeIndex, int switchIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return switchIndex;

        if (parser->tree[nodeIndex].label == "eps")
            return switchIndex;

        int firstChild = parser->tree[nodeIndex].child.empty()
            ? -1
            : parser->tree[nodeIndex].child[0];

        if (firstChild < 0)
            return switchIndex;

        std::string label = parser->tree[firstChild].label;

        if (label == "case")
        {
            int caseIndex = AddNodeFromToken(nodeIndex, AST_CASE_BRANCH);
            int valueIndex = FindChild(nodeIndex, "Expr");
            if (valueIndex >= 0)
                AddChild(tree, caseIndex, BuildExpression(valueIndex));

            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, caseIndex, BuildBlockFromBody(bodyIndex));

            AddChild(tree, switchIndex, caseIndex);

            int nextCase = FindChild(nodeIndex, "Case");
            if (nextCase >= 0)
                BuildCase(nextCase, switchIndex);
        }
        else if (label == "deft")
        {
            int defaultIndex = AddNodeFromToken(nodeIndex, AST_DEFAULT_BRANCH);
            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, defaultIndex, BuildBlockFromBody(bodyIndex));
            AddChild(tree, switchIndex, defaultIndex);
        }

        return switchIndex;
    }

    int AstBuilder::BuildCycle(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        MFST::Mfst::TreeNode& node = parser->tree[nodeIndex];
        if (node.child.empty())
            return -1;

        std::string label = parser->tree[node.child[0]].label;

        if (label == "whle")
        {
            int cycleIndex = AddNodeFromToken(nodeIndex, AST_WHILE_INSTRUCTION);
            int conditionIndex = FindChild(nodeIndex, "Expr");
            if (conditionIndex >= 0)
                AddChild(tree, cycleIndex, BuildExpression(conditionIndex));
            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, cycleIndex, BuildBlockFromBody(bodyIndex));
            return cycleIndex;
        }

        if (label == "do")
        {
            int cycleIndex = AddNodeFromToken(nodeIndex, AST_DOWHILE_INSTRUCTION);
            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, cycleIndex, BuildBlockFromBody(bodyIndex));
            int conditionIndex = FindChild(nodeIndex, "Expr");
            if (conditionIndex >= 0)
                AddChild(tree, cycleIndex, BuildExpression(conditionIndex));
            return cycleIndex;
        }

        if (label == "for")
        {
            int forPartIndex = FindChild(nodeIndex, "ForP");

            if (forPartIndex >= 0)
            {
                int cycleIndex = AddNodeFromToken(nodeIndex, AST_FOR_INSTRUCTION);
                int initializerIndex = BuildStep(forPartIndex);
                if (initializerIndex >= 0)
                    AddChild(tree, cycleIndex, initializerIndex);

                int conditionIndex = FindChild(nodeIndex, "Expr");
                if (conditionIndex >= 0)
                    AddChild(tree, cycleIndex, BuildExpression(conditionIndex));

                int stepIndex = FindChild(nodeIndex, "Step");
                if (stepIndex >= 0)
                    AddChild(tree, cycleIndex, BuildStep(stepIndex));

                int bodyIndex = FindChild(nodeIndex, "Body");
                if (bodyIndex >= 0)
                    AddChild(tree, cycleIndex, BuildBlockFromBody(bodyIndex));

                return cycleIndex;
            }

            int cycleIndex = AddNodeFromToken(nodeIndex, AST_FORIN_INSTRUCTION);
            int firstIdentifier = -1;
            int secondIdentifier = -1;

            for (int childIndex : node.child)
            {
                if (parser->tree[childIndex].label == "idfr")
                {
                    if (firstIdentifier < 0)
                        firstIdentifier = childIndex;
                    else if (secondIdentifier < 0)
                        secondIdentifier = childIndex;
                }
            }

            if (firstIdentifier >= 0)
                tree.nodes[cycleIndex].name = GetIdentifier(firstIdentifier);

            if (secondIdentifier >= 0)
            {
                int arrayIndex = AddNodeFromToken(secondIdentifier, AST_VAR_EXPRESSION);
                tree.nodes[arrayIndex].name = GetIdentifier(secondIdentifier);
                AddChild(tree, cycleIndex, arrayIndex);
            }

            int bodyIndex = FindChild(nodeIndex, "Body");
            if (bodyIndex >= 0)
                AddChild(tree, cycleIndex, BuildBlockFromBody(bodyIndex));

            return cycleIndex;
        }

        return -1;
    }

    int AstBuilder::BuildStep(int nodeIndex)
    {
        if (parser == nullptr || nodeIndex < 0 || nodeIndex >= (int)parser->tree.size())
            return -1;

        int declarationIndex = FindChild(nodeIndex, "Decl");
        if (declarationIndex >= 0)
            return BuildDeclaration(declarationIndex);

        int assignmentIndex = FindChild(nodeIndex, "Asgn");
        if (assignmentIndex >= 0)
            return BuildAssignment(assignmentIndex);

        int expressionIndex = FindChild(nodeIndex, "Expr");
        int identifierIndex = FindChild(nodeIndex, "idfr");
        int plusCount = 0;
        int minusCount = 0;

        for (int childIndex : parser->tree[nodeIndex].child)
        {
            if (parser->tree[childIndex].label == "+")
                ++plusCount;
            else if (parser->tree[childIndex].label == "-")
                ++minusCount;
        }

        bool hasIncrement = plusCount >= 2;
        bool hasDecrement = minusCount >= 2;

        if (identifierIndex >= 0 && hasIncrement)
        {
            int resultIndex = AddNodeFromToken(identifierIndex, AST_INCREMENT_INSTRUCTION);
            int variableIndex = AddNodeFromToken(identifierIndex, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(identifierIndex);
            AddChild(tree, resultIndex, variableIndex);
            return resultIndex;
        }

        if (identifierIndex >= 0 && hasDecrement)
        {
            int resultIndex = AddNodeFromToken(identifierIndex, AST_DECREMENT_INSTRUCTION);
            int variableIndex = AddNodeFromToken(identifierIndex, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(identifierIndex);
            AddChild(tree, resultIndex, variableIndex);
            return resultIndex;
        }

        if (identifierIndex >= 0 && expressionIndex >= 0)
        {
            int resultIndex = AddNodeFromToken(identifierIndex, AST_ASSIGNMENT_INSTRUCTION);
            int variableIndex = AddNodeFromToken(identifierIndex, AST_VAR_EXPRESSION);
            tree.nodes[variableIndex].name = GetIdentifier(identifierIndex);
            AddChild(tree, resultIndex, variableIndex);
            AddChild(tree, resultIndex, BuildExpression(expressionIndex));
            tree.nodes[resultIndex].assignmentOperator = Assign;
            return resultIndex;
        }

        if (expressionIndex >= 0)
            return BuildExpression(expressionIndex);

        return -1;
    }

    static std::string EscapeAstText(const std::string& value)
    {
        std::string result;
        result.reserve(value.size());

        for (char symbol : value)
        {
            switch (symbol)
            {
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += symbol; break;
            }
        }

        return result;
    }

    static void WriteAstNode(std::ostream& out, AstTree& tree, int nodeIndex, int depth)
    {
        if (nodeIndex < 0 || nodeIndex >= (int)tree.nodes.size())
            return;

        AstNode& node = tree.nodes[nodeIndex];
        out << std::string(depth * 2, ' ') << GetAstNodeName(node.kind);

        if (!node.name.empty())
            out << " " << node.name;

        if (!node.text.empty())
            out << " " << EscapeAstText(node.text);

        if (node.kind == AST_LITERAL_EXPRESSION)
            out << " [" << GetLiteralName(node.literalKind) << "]";

        if (node.kind == AST_BINARY_EXPRESSION)
            out << " [" << GetBinaryOperatorName(node.binaryOperator) << "]";

        if (node.kind == AST_UNARY_EXPRESSION)
            out << " [" << GetUnaryOperatorName(node.unaryOperator) << "]";

        if (node.kind == AST_ASSIGNMENT_INSTRUCTION)
            out << " [" << GetAssignmentOperatorName(node.assignmentOperator) << "]";

        if (node.kind == AST_VAR_DECLARATION ||
            node.kind == AST_ARRAY_DECLARATION ||
            node.kind == AST_PROCEDURE ||
            node.kind == AST_PARAMETER)
        {
            out << " : " << GetTypeName(node.type);
        }

        if (node.line > 0)
            out << " @" << node.line << ":" << node.column;

        out << "\r\n";

        for (int childIndex : node.children)
            WriteAstNode(out, tree, childIndex, depth + 1);
    }

    void WriteAst(LOG::Log& log, AstTree& tree)
    {
        if (!log.stream)
            return;

        *(log.stream) << "\r\nABSTRACT SYNTAX TREE\r\n";

        if (tree.rootIndex < 0 || tree.nodes.empty())
        {
            *(log.stream) << "(empty)\r\n";
            return;
        }

        WriteAstNode(*log.stream, tree, tree.rootIndex, 0);
        *(log.stream) << "\r\n";
    }

    bool WriteAstToFile(const wchar_t* file_name, AstTree& tree)
    {
        if (file_name == nullptr || file_name[0] == L'\0')
            return false;

        std::ofstream out(file_name, std::ios::binary);

        if (!out.is_open())
        {
            std::wcerr << L"Warning: cannot create AST file: " << file_name << std::endl;
            return false;
        }

        out << "ABSTRACT SYNTAX TREE\r\n";

        if (tree.rootIndex < 0 || tree.nodes.empty())
            out << "(empty)\r\n";
        else
            WriteAstNode(out, tree, tree.rootIndex, 0);

        out << "\r\n";
        out.close();

        return true;
    }
}
