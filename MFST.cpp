#include "LIBS.h"
#include "GRB.h"
#include <iomanip>

using namespace std;
using namespace MFST;

namespace
{
    constexpr int MAX_START_SEARCH_DEPTH = 16;

    string getGrammarSymbolText(grbalphabet symbol)
    {
        char buffer[5];
        GRB::Rule::Chain::alphabet_to_char(symbol, buffer);
        string text(buffer);
        while (!text.empty() && text.back() == ' ')
            text.pop_back();
        return text;
    }

    void writeTreeNode(ostream& out, const vector<Mfst::ParseTreeNode>& tree_nodes,
        int node_index, int depth)
    {
        if (node_index < 0 || node_index >= (int)tree_nodes.size())
            return;

        const Mfst::ParseTreeNode& node = tree_nodes[node_index];
        out << string(depth * 2, ' ') << node.label << "\r\n";

        for (size_t i = 0; i < node.child_indices.size(); ++i)
            writeTreeNode(out, tree_nodes, node.child_indices[i], depth + 1);
    }
}


ParserSnapshot::ParserSnapshot()
{
    tape_position = 0;
    rule_index = -1;
    chain_index = -1;
    derivation_step_count = 0;
}

ParserSnapshot::ParserSnapshot(short tape_position, ParseStack parse_stack,
    short rule_index, short chain_index, std::size_t derivation_step_count)
{
    this->tape_position = tape_position;
    this->parse_stack = parse_stack;
    this->rule_index = rule_index;
    this->chain_index = chain_index;
    this->derivation_step_count = derivation_step_count;
}

Mfst::ParserDiagnostic::ParserDiagnostic()
{
    tape_position = -1;
    step_result = SURPRISE;
    rule_index = -1;
    chain_index = -1;
}

Mfst::ParserDiagnostic::ParserDiagnostic(short tape_position, RC_STEP step_result,
    short rule_index, short chain_index)
{
    this->tape_position = tape_position;
    this->step_result = step_result;
    this->rule_index = rule_index;
    this->chain_index = chain_index;
}


Mfst::Mfst()
{
    input_tape = nullptr;
    tape_position = 0;
    rule_index = -1;
    chain_index = -1;
    tape_size = 0;
    trace_log = nullptr;
    step_count = 0;
}

Mfst::Mfst(LEXER::Lexer& lexer_source, GRB::Greibach& grammar_source)
{
    lexer = lexer_source;
    grammar = grammar_source;
    input_tape = nullptr;
    tape_position = 0;
    rule_index = -1;
    chain_index = -1;
    trace_log = nullptr;
    step_count = 0;

    tape_size = (short)(lexer.lextable.current_size + 1);
    input_tape = new grbalphabet[tape_size];

    for (short i = 0; i < (short)lexer.lextable.current_size; i++)
    {
        char token_code[5] = { 0, 0, 0, 0, 0 };
        memcpy(token_code, lexer.lextable.table[i].lexema, 4);
        input_tape[i] = GRB::Rule::Chain::T(token_code);
    }

    input_tape[tape_size - 1] = GRB::Rule::Chain::T("$   ");

    parse_stack.push(grammar.stack_bottom);
    parse_stack.push(grammar.start_symbol);
}

Mfst::~Mfst()
{
    if (input_tape)
    {
        delete[] input_tape;
        input_tape = nullptr;
    }
}


char* Mfst::getStackText(char* buffer)
{
    ParseStack stack_copy = parse_stack;
    short offset = 0;
    while (!stack_copy.empty() && offset + 5 < 256)
    {
        GRB::Rule::Chain::alphabet_to_char(stack_copy.top(), buffer + offset);
        offset += 4;
        buffer[offset++] = ' ';
        stack_copy.pop();
    }
    buffer[offset] = '\0';
    return buffer;
}

char* Mfst::getTapeText(char* buffer, short start_position, short token_count)
{
    short written_count = 0;
    if (start_position < 0 || start_position >= tape_size)
    {
        buffer[0] = '\0';
        return buffer;
    }

    for (short position = start_position;
        position < tape_size && written_count < token_count;
        ++position, ++written_count)
    {
        GRB::Rule::Chain::alphabet_to_char(input_tape[position], buffer + written_count * 5);
        buffer[written_count * 5 + 4] = ' ';
    }
    buffer[written_count * 5] = '\0';
    return buffer;
}

char* Mfst::getDiagnosticMessage(short diagnostic_index, char* buffer)
{
    buffer[0] = '\0';
    if (diagnostic_index < 0 || diagnostic_index >= MAX_DIAGNOSTIC_MESSAGES)
        return buffer;

    ParserDiagnostic& diagnostic = diagnostics[diagnostic_index];
    if (diagnostic.tape_position < 0)
        return buffer;

    int error_id = 600;
    if (diagnostic.rule_index >= 0 && diagnostic.rule_index < grammar.rule_count)
        error_id = grammar.rules[diagnostic.rule_index].errorID;

    int line_number = 0;
    if (lexer.lextable.current_size > 0)
    {
        short token_index = diagnostic.tape_position;
        if (token_index >= (short)lexer.lextable.current_size)
            token_index = (short)lexer.lextable.current_size - 1;
        line_number = lexer.lextable.table[token_index].str_number;
    }

    sprintf_s(buffer, DIAGNOSTIC_MESSAGE_SIZE, "%d: %s (строка %d)",
        error_id, ERROR::getError(error_id).message, line_number);
    return buffer;
}

bool Mfst::saveState()
{
    saved_states.push(ParserSnapshot(
        tape_position, parse_stack, rule_index, chain_index, derivation_steps.size()));
    return true;
}

bool Mfst::restoreState()
{
    if (saved_states.empty())
        return false;

    ParserSnapshot snapshot = saved_states.top();
    saved_states.pop();

    tape_position = snapshot.tape_position;
    parse_stack = snapshot.parse_stack;
    rule_index = snapshot.rule_index;
    chain_index = snapshot.chain_index;
    if (derivation_steps.size() > snapshot.derivation_step_count)
        derivation_steps.resize(snapshot.derivation_step_count);
    return true;
}

bool Mfst::pushChain(const GRB::Rule::Chain& chain)
{
    for (short position = chain.size - 1; position >= 0; --position)
        parse_stack.push(chain.term_and_notterm[position]);
    return true;
}

bool Mfst::saveDiagnostic(RC_STEP step_result)
{
    short diagnostic_index = 0;
    while (diagnostic_index < MAX_DIAGNOSTIC_MESSAGES &&
        tape_position <= diagnostics[diagnostic_index].tape_position)
    {
        ++diagnostic_index;
    }

    if (diagnostic_index < MAX_DIAGNOSTIC_MESSAGES)
    {
        diagnostics[diagnostic_index] = ParserDiagnostic(
            tape_position, step_result, rule_index, chain_index);
        for (short index = diagnostic_index + 1; index < MAX_DIAGNOSTIC_MESSAGES; ++index)
            diagnostics[index].tape_position = -1;
    }
    return true;
}

bool Mfst::canStartWith(grbalphabet symbol, grbalphabet lookahead, int depth) const
{
    if (GRB::Rule::Chain::isT(symbol))
        return symbol == lookahead;

    if (depth > MAX_START_SEARCH_DEPTH)
        return true;

    for (short rule_index = 0; rule_index < grammar.rule_count; ++rule_index)
    {
        if (grammar.rules[rule_index].not_terminal != symbol)
            continue;

        const GRB::Rule& rule = grammar.rules[rule_index];
        for (short chain_index = 0; chain_index < rule.count; ++chain_index)
        {
            if (rule.chain[chain_index].size == 0)
                return true;
            if (canStartWith(rule.chain[chain_index].term_and_notterm[0], lookahead, depth + 1))
                return true;
        }
        return false;
    }
    return false;
}

Mfst::RC_STEP Mfst::step()
{
    if (parse_stack.empty())
    {
        writeTraceLine("NS_ERROR");
        return NS_ERROR;
    }

    if (tape_position >= tape_size)
    {
        if (parse_stack.size() == 1 && parse_stack.top() == grammar.stack_bottom)
        {
            writeTraceLine("LENTA_END");
            return LENTA_END;
        }

        saveDiagnostic(TS_NOK);
        writeTraceLine("TS_NOK");
        if (restoreState())
        {
            writeTraceLine("RESTATE");
            return TS_NOK;
        }
        writeTraceLine("NS_NORULE");
        return NS_NORULE;
    }

    grbalphabet stack_top = parse_stack.top();

    if (GRB::Rule::Chain::isT(stack_top))
    {
        if (stack_top == input_tape[tape_position])
        {
            parse_stack.pop();
            tape_position++;
            chain_index = -1;
            writeTraceLine("TS_OK");
            return TS_OK;
        }

        saveDiagnostic(TS_NOK);
        writeTraceLine("TS_NOK");
        if (restoreState())
        {
            writeTraceLine("RESTATE");
            return TS_NOK;
        }
        writeTraceLine("NS_NORULE");
        return NS_NORULE;
    }

    short matched_rule_index = -1;
    for (short rule_position = 0; rule_position < grammar.rule_count; ++rule_position)
    {
        if (grammar.rules[rule_position].not_terminal == stack_top)
        {
            matched_rule_index = rule_position;
            break;
        }
    }
    if (matched_rule_index == -1)
    {
        writeTraceLine("NS_ERROR");
        return NS_ERROR;
    }

    GRB::Rule& rule = grammar.rules[matched_rule_index];

    short selected_chain_index = -1;
    for (short chain_position = (chain_index >= 0 ? chain_index + 1 : 0); chain_position < rule.count; ++chain_position)
    {
        const GRB::Rule::Chain& chain = rule.chain[chain_position];

        if (chain.size == 0)
        {
            selected_chain_index = chain_position;
            break;
        }
        if (chain.term_and_notterm != nullptr && canStartWith(chain.term_and_notterm[0], input_tape[tape_position]))
        {
            selected_chain_index = chain_position;
            break;
        }
    }

    if (selected_chain_index == -1)
    {
        rule_index = matched_rule_index;
        saveDiagnostic(NS_NORULECHAIN);
        writeTraceLine("NS_NORULECHAIN");
        if (restoreState())
        {
            writeTraceLine("RESTATE");
            return NS_NORULECHAIN;
        }
        writeTraceLine("NS_NORULE");
        return NS_NORULE;
    }

    rule_index = matched_rule_index;
    chain_index = selected_chain_index;

    char rule_text[512];
    rule.GetRule(rule_text, selected_chain_index);
    writeTraceLine(rule_text);

    saveState();                                  
    writeTraceLine("SAVESTATE:");
    derivation_steps.push_back({ matched_rule_index, selected_chain_index });

    parse_stack.pop();
    pushChain(rule.chain[selected_chain_index]);

    chain_index = -1;
    return NS_OK;
}


bool Mfst::start()
{
    step_count = 0;
    derivation_steps.clear();
    parse_tree.clear();
    RC_STEP step_result = SURPRISE;

    do
    {
        step_result = step();
        ++step_count;
    } while ((step_result == NS_OK || step_result == TS_OK ||
        step_result == TS_NOK || step_result == NS_NORULECHAIN) &&
        step_count < MAX_PARSER_STEPS);

    return step_result == LENTA_END;
}

void Mfst::writeTraceHeader()
{
    if (!trace_log || !trace_log->stream)
        return;

    *trace_log->stream << "\r\nSYNTAX ANALYSIS TRACE\r\n";
    *trace_log->stream << std::left
        << std::setw(6) << "Step" << " : "
        << std::setw(56) << "Rule"
        << std::setw(58) << "Input tape"
        << "Stack\r\n";
    *trace_log->stream << std::string(6 + 3 + 56 + 58 + 5, '-') << "\r\n";
    *trace_log->stream << std::right;
}

void Mfst::writeTraceLine(const char* action_text)
{
    if (!trace_log || !trace_log->stream)
        return;

    char tape_buffer[256];
    char stack_buffer[512];
    getTapeText(tape_buffer, tape_position, 10);
    getStackText(stack_buffer);

    std::string rule_text = action_text ? action_text : "";
    if (rule_text.size() > 55)
        rule_text.resize(55);
    else
        rule_text.append(56 - rule_text.size(), ' ');

    *trace_log->stream << std::right << std::setw(6) << step_count << " : "
        << rule_text
        << std::left << std::setw(58) << tape_buffer
        << stack_buffer << "\r\n";
    *trace_log->stream << std::right;
}

void Mfst::buildParseTree()
{
    parse_tree.clear();
    if (derivation_steps.empty())
        return;

    ParseTreeNode root;
    root.grammar_symbol = grammar.start_symbol;
    root.label = getGrammarSymbolText(grammar.start_symbol);
    parse_tree.push_back(root);

    std::deque<int> nodes_to_expand;
    nodes_to_expand.push_back(0);

    for (std::size_t derivation_index = 0;
        derivation_index < derivation_steps.size();
        ++derivation_index)
    {
        if (nodes_to_expand.empty())
            break;

        int parent_index = nodes_to_expand.front();
        nodes_to_expand.pop_front();

        const DerivationStep& applied_step = derivation_steps[derivation_index];
        if (applied_step.rule_index < 0 || applied_step.rule_index >= grammar.rule_count)
            continue;

        const GRB::Rule& rule = grammar.rules[applied_step.rule_index];
        if (applied_step.chain_index < 0 || applied_step.chain_index >= rule.count)
            continue;

        const GRB::Rule::Chain& chain = rule.chain[applied_step.chain_index];
        std::vector<int> child_indices;
        std::vector<int> new_frontier;

        if (chain.size == 0)
        {
            ParseTreeNode epsilon;
            epsilon.grammar_symbol = 0;
            epsilon.label = "eps";
            child_indices.push_back((int)parse_tree.size());
            parse_tree.push_back(epsilon);
        }
        else
        {
            for (short symbol_index = 0; symbol_index < chain.size; ++symbol_index)
            {
                grbalphabet grammar_symbol = chain.term_and_notterm[symbol_index];
                ParseTreeNode child;
                child.grammar_symbol = grammar_symbol;
                child.label = getGrammarSymbolText(grammar_symbol);

                int child_index = (int)parse_tree.size();
                child_indices.push_back(child_index);
                parse_tree.push_back(child);

                if (GRB::Rule::Chain::isN(grammar_symbol))
                    new_frontier.push_back(child_index);
            }
        }

        parse_tree[parent_index].child_indices = child_indices;
        nodes_to_expand.insert(nodes_to_expand.begin(), new_frontier.begin(), new_frontier.end());
    }
}

void Mfst::writeParseTree(LOG::Log& log)
{
    if (!log.stream)
        return;

    buildParseTree();

    *log.stream << "\r\nPARSE TREE\r\n";
    if (parse_tree.empty())
    {
        *log.stream << "(parse tree is empty)\r\n";
        return;
    }

    writeTreeNode(*log.stream, parse_tree, 0, 0);
    *log.stream << "\r\n";
}
