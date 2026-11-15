#pragma once
#include "LIBS.h"

#define MAX_DIAGNOSTIC_MESSAGES 3
#define DIAGNOSTIC_MESSAGE_SIZE 1024
#define MAX_PARSER_STEPS 5000000

typedef std::stack<grbalphabet> mfststack;

namespace MFST
{
    struct MfstState
    {
        short lenta_position;
        short current_rule_number;
        short current_chain_number;

        std::size_t step_count;
        mfststack stack;

        MfstState();
        MfstState(short _lenta_position, mfststack _stack, short _current_rule_number, short _current_chain_number, std::size_t _step_count = 0);
    };

    struct Mfst
    {
        enum RC_STEP
        {
            NS_OK,
            NS_NORULE,
            NS_NORULECHAIN,
            NS_ERROR,
            TS_OK,
            TS_NOK,
            LENTA_END,
            SURPRISE
        };

        struct MfstDiagnostics
        {
            short lenta_position;
            RC_STEP step_result;
            short rule_number;
            short rule_chain_number;

            MfstDiagnostics();
            MfstDiagnostics(short _lenta_position, RC_STEP _step_result, short _rule_number, short rule_chain_number);
        } diagnostics[MAX_DIAGNOSTIC_MESSAGES];

        grbalphabet* lex_lenta;
        short lenta_position;
        short current_rule_number;
        short current_chain_number;
        short lenta_size;

        GRB::Greibach grammar;
        LEXER::Lexer lexer;
        mfststack stack;
        std::stack<MfstState> saved_states;

        struct OutStep
        {
            short rule_index;
            short chain_index;
        };

        struct TreeNode
        {
            grbalphabet grammar_symbol;
            std::string label;
            std::vector<int> child;

            int token_index = -1;
        };

        std::vector<OutStep> out_steps;
        std::vector<TreeNode> tree;
        LOG::Log* log;

        int step_count;

        Mfst();
        Mfst(LEXER::Lexer& lexer, GRB::Greibach& grammar);
        ~Mfst();

        Mfst(const Mfst&) = delete;
        Mfst& operator=(const Mfst&) = delete;

        char* getStackText(char* buffer);
        char* getTapeText(char* buffer, short start_position, short token_count = 25);
        char* getDiagnosticMessage(short diagnostic_index, char* buffer);
        bool saveState();
        bool restoreState();
        bool pushChain(const GRB::Rule::Chain& chain);

        bool canStartWith(grbalphabet symbol, grbalphabet lookahead, int depth = 0) const;

        RC_STEP step();
        bool start();
        bool saveDiagnostic(RC_STEP _step_result);

        void setTraceLog(LOG::Log& _log) {log = &_log; }
        void writeTraceHeader();
        void writeTraceLine(const char* action_text);

        void buildParseTree();
        void writeParseTree(LOG::Log& log);
        bool writeParseTreeToFile(const wchar_t* file_name);


        const TreeNode& GetNode(const MFST::Mfst& parser, int node_index);
    };
}
