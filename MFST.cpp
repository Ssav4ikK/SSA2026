#include "LIBS.h"
#include <iomanip>
#include <deque>

using namespace std;

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

    void assignTokenIndices(vector<MFST::Mfst::TreeNode>& tree_nodes, int node_index, int& token_index)
    {
        if (node_index < 0 || node_index >= (int)tree_nodes.size())
            return;

        MFST::Mfst::TreeNode& node = tree_nodes[node_index];

        if (GRB::Rule::Chain::isT(node.grammar_symbol))
        {
            if (node.label != "$")
                node.token_index = token_index++;
            else
                node.token_index = -1;
        }

        for (int child_index : node.child)
            assignTokenIndices(tree_nodes, child_index, token_index);
    }
    void writeTreeNode(ostream& out, const vector<MFST::Mfst::TreeNode>& tree_nodes,int node_index, int depth)
    {
        if (node_index < 0 || node_index >= (int)tree_nodes.size())
            return;

        const MFST::Mfst::TreeNode& node = tree_nodes[node_index];
        out << string(depth * 2, ' ') << node.label << "\r\n";

        for (size_t i = 0; i < node.child.size(); ++i)
            writeTreeNode(out, tree_nodes, node.child[i], depth + 1);
    }
}

namespace MFST
{
    MfstState::MfstState()
    {
        lenta_position = 0;
        current_rule_number = -1;
        current_chain_number = -1;
        step_count = 0;
    }

    MfstState::MfstState(short _lenta_position, mfststack _stack,
        short _current_rule_number, short _current_chain_number,
        std::size_t _step_count)
    {
        lenta_position = _lenta_position;
        stack = _stack;
        current_rule_number = _current_rule_number;
        current_chain_number = _current_chain_number;
        step_count = _step_count;
    }

    Mfst::MfstDiagnostics::MfstDiagnostics()
    {
        lenta_position = -1;
        step_result = SURPRISE;
        rule_number = -1;
        rule_chain_number = -1;
    }

    Mfst::MfstDiagnostics::MfstDiagnostics(short _lenta_position,RC_STEP _step_result, short _rule_number, short _rule_chain_number)
    {
        lenta_position = _lenta_position;
        step_result = _step_result;
        rule_number = _rule_number;
        rule_chain_number = _rule_chain_number;
    }

    Mfst::Mfst()
    {
        lex_lenta = nullptr;
        lenta_position = 0;
        current_rule_number = -1;
        current_chain_number = -1;
        lenta_size = 0;
        log = nullptr;
        step_count = 0;
    }

    Mfst::Mfst(LEXER::Lexer& _lexer, GRB::Greibach& _grammar)
    {
        lexer = _lexer;
        grammar = _grammar;

        lex_lenta = nullptr;
        lenta_position = 0;
        current_rule_number = -1;
        current_chain_number = -1;
        lenta_size = 0;
        log = nullptr;
        step_count = 0;

        lenta_size = (short)(lexer.lextable.current_size + 1);
        lex_lenta = new grbalphabet[lenta_size];

        for (short i = 0; i < (short)lexer.lextable.current_size; ++i)
        {
            char token_code[5] = { 0, 0, 0, 0, 0 };
            memcpy(token_code, lexer.lextable.table[i].lexema, 4);
            lex_lenta[i] = GRB::Rule::Chain::T(token_code);
        }

        lex_lenta[lenta_size - 1] = GRB::Rule::Chain::T("$   ");

        stack.push(grammar.stack_bottom);
        stack.push(grammar.start_symbol);
    }

    Mfst::~Mfst()
    {
        if (lex_lenta)
        {
            delete[] lex_lenta;
            lex_lenta = nullptr;
        }
    }

    char* Mfst::getStackText(char* buffer)
    {
        mfststack stack_copy = stack;
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

        if (start_position < 0 || start_position >= lenta_size)
        {
            buffer[0] = '\0';
            return buffer;
        }

        for (short position = start_position;
            position < lenta_size && written_count < token_count;
            ++position, ++written_count)
        {
            GRB::Rule::Chain::alphabet_to_char(
                lex_lenta[position], buffer + written_count * 5);
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

        MfstDiagnostics& diagnostic = diagnostics[diagnostic_index];
        if (diagnostic.lenta_position < 0)
            return buffer;

        int error_id = 600;
        if (diagnostic.rule_number >= 0 &&
            diagnostic.rule_number < grammar.rule_count)
        {
            error_id = grammar.rules[diagnostic.rule_number].errorID;
        }

        int line_number = 0;
        int column_number = 0;
        if (lexer.lextable.current_size > 0)
        {
            short token_index = diagnostic.lenta_position;
            if (token_index >= (short)lexer.lextable.current_size)
                token_index = (short)lexer.lextable.current_size - 1;

            line_number = lexer.lextable.table[token_index].str_number;
            column_number = lexer.lextable.table[token_index].col_number;
        }

        sprintf_s(buffer, DIAGNOSTIC_MESSAGE_SIZE, "%d: %s (строка %d, позиция %d)",
            error_id, ERROR::getError(error_id).message, line_number, column_number);

        return buffer;
    }

    bool Mfst::saveState()
    {
        saved_states.push(MfstState(
            lenta_position,
            stack,
            current_rule_number,
            current_chain_number,
            out_steps.size()));

        return true;
    }

    bool Mfst::restoreState()
    {
        if (saved_states.empty())
            return false;

        MfstState snapshot = saved_states.top();
        saved_states.pop();

        lenta_position = snapshot.lenta_position;
        stack = snapshot.stack;
        current_rule_number = snapshot.current_rule_number;
        current_chain_number = snapshot.current_chain_number;

        if (out_steps.size() > snapshot.step_count)
            out_steps.resize(snapshot.step_count);

        return true;
    }

    bool Mfst::pushChain(const GRB::Rule::Chain& chain)
    {
        for (short position = chain.size - 1; position >= 0; --position)
            stack.push(chain.term_and_notterm[position]);

        return true;
    }

    bool Mfst::saveDiagnostic(RC_STEP _step_result)
    {
        short diagnostic_index = 0;

        while (diagnostic_index < MAX_DIAGNOSTIC_MESSAGES &&
            lenta_position <= diagnostics[diagnostic_index].lenta_position)
        {
            ++diagnostic_index;
        }

        if (diagnostic_index < MAX_DIAGNOSTIC_MESSAGES)
        {
            diagnostics[diagnostic_index] = MfstDiagnostics(
                lenta_position,
                _step_result,
                current_rule_number,
                current_chain_number);

            for (short index = diagnostic_index + 1;
                index < MAX_DIAGNOSTIC_MESSAGES;
                ++index)
            {
                diagnostics[index].lenta_position = -1;
            }
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

                if (canStartWith(
                    rule.chain[chain_index].term_and_notterm[0],
                    lookahead,
                    depth + 1))
                {
                    return true;
                }
            }

            return false;
        }

        return false;
    }

    Mfst::RC_STEP Mfst::step()
    {
        if (stack.empty())
        {
            writeTraceLine("NS_ERROR");
            return NS_ERROR;
        }

        if (lenta_position >= lenta_size)
        {
            if (stack.size() == 1 && stack.top() == grammar.stack_bottom)
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

        grbalphabet stack_top = stack.top();

        if (GRB::Rule::Chain::isT(stack_top))
        {
            if (stack_top == lex_lenta[lenta_position])
            {
                stack.pop();
                ++lenta_position;
                current_chain_number = -1;
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

        for (short rule_position = 0;
            rule_position < grammar.rule_count;
            ++rule_position)
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

        for (short chain_position =
                (current_chain_number >= 0 ? current_chain_number + 1 : 0);
            chain_position < rule.count;
            ++chain_position)
        {
            const GRB::Rule::Chain& chain = rule.chain[chain_position];

            if (chain.size == 0)
            {
                selected_chain_index = chain_position;
                break;
            }

            if (chain.term_and_notterm != nullptr &&
                canStartWith(chain.term_and_notterm[0], lex_lenta[lenta_position]))
            {
                selected_chain_index = chain_position;
                break;
            }
        }

        if (selected_chain_index == -1)
        {
            current_rule_number = matched_rule_index;
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

        current_rule_number = matched_rule_index;
        current_chain_number = selected_chain_index;

        char rule_text[512];
        rule.GetRule(rule_text, selected_chain_index);
        writeTraceLine(rule_text);

        saveState();
        writeTraceLine("SAVESTATE:");

        out_steps.push_back({ matched_rule_index, selected_chain_index });

        stack.pop();
        pushChain(rule.chain[selected_chain_index]);

        current_chain_number = -1;
        return NS_OK;
    }

    bool Mfst::start()
    {
        step_count = 0;
        out_steps.clear();
        tree.clear();

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
        if (!log || !log->stream)
            return;

        *log->stream << "\r\nSYNTAX ANALYSIS TRACE\r\n";
        *log->stream << std::left
            << std::setw(6) << "Step" << " : "
            << std::setw(56) << "Rule"
            << std::setw(58) << "Input tape"
            << "Stack\r\n";
        *log->stream << std::string(6 + 3 + 56 + 58 + 5, '-') << "\r\n";
        *log->stream << std::right;
    }

    void Mfst::writeTraceLine(const char* action_text)
    {
        if (!log || !log->stream)
            return;

        char tape_buffer[256];
        char stack_buffer[512];

        getTapeText(tape_buffer, lenta_position, 10);
        getStackText(stack_buffer);

        std::string rule_text = action_text ? action_text : "";

        if (rule_text.size() > 55)
            rule_text.resize(55);
        else
            rule_text.append(56 - rule_text.size(), ' ');

        *log->stream << std::right << std::setw(6) << step_count << " : "
            << rule_text
            << std::left << std::setw(58) << tape_buffer
            << stack_buffer << "\r\n";
        *log->stream << std::right;
    }

    void Mfst::buildParseTree()
    {
        tree.clear();

        if (out_steps.empty())
            return;

        TreeNode root;
        root.grammar_symbol = grammar.start_symbol;
        root.label = getGrammarSymbolText(grammar.start_symbol);
        tree.push_back(root);

        std::deque<int> nodes_to_expand;
        nodes_to_expand.push_back(0);


        for (std::size_t step_index = 0; step_index < out_steps.size(); ++step_index)
        {
            if (nodes_to_expand.empty())
                break;

            int parent_index = nodes_to_expand.front();
            nodes_to_expand.pop_front();

            const OutStep& applied_step = out_steps[step_index];

            if (applied_step.rule_index < 0 ||
                applied_step.rule_index >= grammar.rule_count)
            {
                continue;
            }

            const GRB::Rule& rule = grammar.rules[applied_step.rule_index];

            if (applied_step.chain_index < 0 ||
                applied_step.chain_index >= rule.count)
            {
                continue;
            }

            const GRB::Rule::Chain& chain = rule.chain[applied_step.chain_index];
            std::vector<int> child_indices;
            std::vector<int> new_frontier;

            if (chain.size == 0)
            {
                TreeNode epsilon;
                epsilon.grammar_symbol = 0;
                epsilon.label = "eps";

                child_indices.push_back((int)tree.size());
                tree.push_back(epsilon);
            }
            else
            {
                for (short symbol_index = 0; symbol_index < chain.size; ++symbol_index)
                {
                    grbalphabet grammar_symbol = chain.term_and_notterm[symbol_index];

                    TreeNode child_node;
                    child_node.grammar_symbol = grammar_symbol;
                    child_node.label = getGrammarSymbolText(grammar_symbol);

                    int child_index = (int)tree.size();
                    child_indices.push_back(child_index);
                    tree.push_back(child_node);

                    if (GRB::Rule::Chain::isN(grammar_symbol)) 
                    {
                        new_frontier.push_back(child_index);
                    }
                        
                }
            }

            tree[parent_index].child = child_indices;
            nodes_to_expand.insert(
                nodes_to_expand.begin(),
                new_frontier.begin(),
                new_frontier.end());
        }

        int tokenIndex = 0;
        if (!tree.empty())
            assignTokenIndices(tree, 0, tokenIndex);
    }

    void Mfst::writeParseTree(LOG::Log& output_log)
    {
        if (!output_log.stream)
            return;

        buildParseTree();

        *output_log.stream << "\r\nPARSE TREE\r\n";

        if (tree.empty())
        {
            *output_log.stream << "(parse tree is empty)\r\n";
            return;
        }

        writeTreeNode(*output_log.stream, tree, 0, 0);
        *output_log.stream << "\r\n";
    }

    bool Mfst::writeParseTreeToFile(const wchar_t* file_name)
    {
        if (file_name == nullptr || file_name[0] == L'\0')
            return false;

        buildParseTree();

        std::ofstream out(file_name, std::ios::binary);

        if (!out.is_open())
        {
            if (log && log->stream)
                LOG::WriteLine(*log, "MFST: cannot create parse tree file", "");

            std::wcerr << L"Warning: cannot create MFST file: " << file_name << std::endl;
            return false;
        }

        out << "PARSE TREE\r\n";

        if (tree.empty())
            out << "(parse tree is empty)\r\n";
        else
            writeTreeNode(out, tree, 0, 0);

        out << "\r\n";
        out.close();

        return true;
    }





    const Mfst::TreeNode& GetNode(const MFST::Mfst& parser, int node_index)
    {
        return parser.tree[node_index];
    }
}
