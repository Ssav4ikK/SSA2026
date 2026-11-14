#pragma once
#include "LIBS.h"
#include "GRB.h"
#include <deque>

#define MAX_DIAGNOSTIC_MESSAGES 3
#define DIAGNOSTIC_MESSAGE_SIZE 1024
#define MAX_PARSER_STEPS 5000000

typedef std::stack<grbalphabet> ParseStack;

namespace MFST
{
    struct ParserSnapshot
    {
        short tape_position;
        short rule_index;
        short chain_index;
        std::size_t derivation_step_count;
        ParseStack parse_stack;

        ParserSnapshot();
        ParserSnapshot(short tape_position, ParseStack parse_stack, short rule_index,
            short chain_index, std::size_t derivation_step_count = 0);
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

        struct ParserDiagnostic
        {
            short tape_position;
            RC_STEP step_result;
            short rule_index;
            short chain_index;

            ParserDiagnostic();
            ParserDiagnostic(short tape_position, RC_STEP step_result,
                short rule_index, short chain_index);
        } diagnostics[MAX_DIAGNOSTIC_MESSAGES];

        grbalphabet* input_tape;
        short tape_position;
        short rule_index;
        short chain_index;
        short tape_size;
        GRB::Greibach grammar;
        LEXER::Lexer lexer;
        ParseStack parse_stack;
        std::stack<ParserSnapshot> saved_states;

        struct DerivationStep
        {
            short rule_index;
            short chain_index;
        };

        struct ParseTreeNode
        {
            grbalphabet grammar_symbol;
            std::string label;
            std::vector<int> child_indices;
        };

        std::vector<DerivationStep> derivation_steps;
        std::vector<ParseTreeNode> parse_tree;
        LOG::Log* trace_log;

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
        bool saveDiagnostic(RC_STEP step_result);

        void setTraceLog(LOG::Log& log) { trace_log = &log; }
        void writeTraceHeader();
        void writeTraceLine(const char* action_text);

        void buildParseTree();
        void writeParseTree(LOG::Log& log);
    };
}
