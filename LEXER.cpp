#include "LIBS.h"

#define COPY(lex)       strcpy_s(lextable_entry.lexema, LEXEMA_FIXSIZE, lex)
#define COMPARE(lex)    strcmp(words, lex) == 0

namespace LEXER {


	




    Lexer Analyze(IN::In in) {
        Lexer lexer;
        lexer.lextable = LEXTABLE::Create(LEXTABLE_MAX_COUNT_OF_LINES);
        lexer.idtable = IDTABLE::Create(IDENTIFICATOR_TABLE_MAX_SIZE);

        int line = 1;
        int col = 1;
        char current_scope[ID_MAX_CHARS + 1] = "";
        IDTABLE::ID_DATA_TYPE current_type = IDTABLE::INT32;

        bool is_array = false;
        bool is_func = false;
        bool is_param = false;


        char buffer[1024];
        int buf_idx = 0;

        auto processBuffer = [&](int length) {
            if (length <= 0) return;
            char local[1024];
            memcpy(local, buffer, length);
            local[length] = '\0';

            LEXTABLE::Entry lextable_entry;
            lextable_entry.str_number = line;
            lextable_entry.indexIT = LEXTABLE_IDENTIFICATORTABLE_NULLIDX;
            strncpy_s(lextable_entry.lexema, LEXEMA_FIXSIZE, local, _TRUNCATE);

            std::string match_result = FST::match(local);
            const char* words = match_result.c_str();

            //VARS

            if (COMPARE(LEX_INT8)) {
                COPY(LEX_INT8);
                current_type = IDTABLE::INT8;
            }
            else if (COMPARE(LEX_INT16)) {
                COPY(LEX_INT16);
                current_type = IDTABLE::INT16;
            }
            else if (COMPARE(LEX_INT32)) {
                COPY(LEX_INT32);
                current_type = IDTABLE::INT32;
            }
            else if (COMPARE(LEX_INT64)) {
                COPY(LEX_INT64);
                current_type = IDTABLE::INT64;
            }
            else if (COMPARE(LEX_FLOAT32)) {
                COPY(LEX_FLOAT32);
                current_type = IDTABLE::FLOAT32;
            }
            else if (COMPARE(LEX_FLOAT64)) {
                COPY(LEX_FLOAT64);
                current_type = IDTABLE::FLOAT64;
            }
            else if (COMPARE(LEX_BOOLEAN)) {
                COPY(LEX_BOOLEAN);
                current_type = IDTABLE::BOOLEAN;
            }
            else if (COMPARE(LEX_SYMBOL)) {
                COPY(LEX_SYMBOL);
                current_type = IDTABLE::SYMBOL;
            }
            else if (COMPARE(LEX_STRING)) {
                COPY(LEX_STRING);
                current_type = IDTABLE::STRING;
            }
            else if (COMPARE(LEX_NUMBER)) {
                COPY(LEX_NUMBER);
                current_type = IDTABLE::NUMBER;
            }
            else if (COMPARE(LEX_HELP)) {
                COPY(LEX_HELP);
                current_type = IDTABLE::HELP;
            }


            // KEYWORDS


            else if (COMPARE(LEX_MAIN)) {
                COPY(LEX_MAIN);
                strcpy_s(current_scope, "main");
            }
            else if (COMPARE(LEX_START)) {
                COPY(LEX_START);
            }
            else if (COMPARE(LEX_END)) {
                COPY(LEX_END);
            }
            else if (COMPARE(LEX_BACK)) {
                COPY(LEX_BACK);
            }
            else if (COMPARE(LEX_PROCEDURE)) {
                COPY(LEX_PROCEDURE);
                is_func = true;
            }
            else if (COMPARE(LEX_ARRAY)) {
                COPY(LEX_ARRAY);
                is_array = true;
            }


            // CONSTRUCTIONS


            else if (COMPARE(LEX_IF)) {
                COPY(LEX_IF);
            }
            else if (COMPARE(LEX_ELIF)) {
                COPY(LEX_ELIF);
            }
            else if (COMPARE(LEX_ELSE)) {
                COPY(LEX_ELSE);
            }


            // CYCLES


            else if (COMPARE(LEX_WHILE)) {
                COPY(LEX_WHILE);
            }
            else if (COMPARE(LEX_DO)) {
                COPY(LEX_DO);
            }
            else if (COMPARE(LEX_FOR)) {
                COPY(LEX_FOR);
            }
            else if (COMPARE(LEX_IN)) {
                COPY(LEX_IN);
            }


            // SWITCH CASE DEFAULT BREAK


            else if (COMPARE(LEX_SWITCH)) {
                COPY(LEX_SWITCH);
            }
            else if (COMPARE(LEX_CASE)) {
                COPY(LEX_CASE);
            }
            else if (COMPARE(LEX_DEFAULT)) {
                COPY(LEX_DEFAULT);
            }
            else if (COMPARE(LEX_BREAK)) {
                COPY(LEX_BREAK);
            }


            // BASIC FUNCTIONS


            else if (COMPARE(LEX_PRINT)) {
                COPY(LEX_PRINT);
            }
            else if (COMPARE(LEX_PRINTF)) {
                COPY(LEX_PRINTF);
            }
            else if (COMPARE(LEX_INPUT)) {
                COPY(LEX_INPUT);
            }


            // OTHERS, OPERATORS


            else if (COMPARE(LEX_SSA2026)) {
                COPY(LEX_SSA2026);
            }
            else if (COMPARE(LEX_PLUS)) {
                COPY(LEX_PLUS);
            }
            else if (COMPARE(LEX_MINUS)) {
                COPY(LEX_MINUS);
            }
            else if (COMPARE(LEX_STAR)) {
                COPY(LEX_STAR);
            }
            else if (COMPARE(LEX_DIRSLASH)) {
                COPY(LEX_DIRSLASH);
            }
            else if (COMPARE(LEX_EQUAL)) {
                COPY(LEX_EQUAL);
            }
            else if (COMPARE(LEX_SEMICOLON)) {
                COPY(LEX_SEMICOLON);
            }
            else if (COMPARE(LEX_COMMA)) {
                COPY(LEX_COMMA);
            }
            else if (COMPARE(LEX_LEFT_BRACKET)) {
                COPY(LEX_LEFT_BRACKET);
            }
            else if (COMPARE(LEX_RIGHT_BRACKET)) {
                COPY(LEX_RIGHT_BRACKET);
            }
            else if (COMPARE(LEX_LEFT_FIGURE_BRACKET)) {
                COPY(LEX_LEFT_FIGURE_BRACKET);
            }
            else if (COMPARE(LEX_RIGHT_FIGURE_BRACKET)) {
                COPY(LEX_RIGHT_FIGURE_BRACKET);
            }
            else if (COMPARE(LEX_LEFT_SQUARE_BRACKET)) {
                COPY(LEX_LEFT_SQUARE_BRACKET);
            }
            else if (COMPARE(LEX_RIGHT_SQUARE_BRACKET)) {
                COPY(LEX_RIGHT_SQUARE_BRACKET);
            }
            else if (COMPARE(LEX_DOUBLEPOINT)) {
                COPY(LEX_DOUBLEPOINT);
            }


            // LITERALS 


            //INT

            else if (FST::isIntegerLiteral(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;

                id_table_entry.id_data_type = current_type;

                if (current_type == IDTABLE::INT8) {
                    id_table_entry.id_value.var_int8 = (int8_t)atoi(local);
                }
                else if (current_type == IDTABLE::INT16) {
                    id_table_entry.id_value.var_int16 = (int16_t)atoi(local);
                }
                else if (current_type == IDTABLE::INT32) {
                    id_table_entry.id_value.var_int32 = (int32_t)atoi(local);
                }
                else if (current_type == IDTABLE::INT64) {
                    id_table_entry.id_value.var_int64 = std::stoll(local);
                }
                else {
                    long long val = std::stoll(local);
                    if (val >= -128 && val <= 127) {
                        id_table_entry.id_data_type = IDTABLE::INT8;
                        id_table_entry.id_value.var_int8 = (int8_t)val;
                    }
                    else if (val >= -32768 && val <= 32767) {
                        id_table_entry.id_data_type = IDTABLE::INT16;
                        id_table_entry.id_value.var_int16 = (int16_t)val;
                    }
                    else if (val >= -2147483648LL && val <= 2147483647LL) {
                        id_table_entry.id_data_type = IDTABLE::INT32;
                        id_table_entry.id_value.var_int32 = (int32_t)val;
                    }
                    else {
                        id_table_entry.id_data_type = IDTABLE::INT64;
                        id_table_entry.id_value.var_int64 = (int64_t)val;
                    }
                }

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }



            //FLOAT

            else if (FST::isFloatLiteral(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;

                id_table_entry.id_data_type = current_type;

                if (current_type == IDTABLE::FLOAT32) {
                    id_table_entry.id_value.var_float32 = (float)atof(local);
                }
                else if (current_type == IDTABLE::FLOAT64) {
                    id_table_entry.id_value.var_float64 = atof(local);
                }
                else {
                    double val = atof(local);
                    if (val >= -FLT_MAX && val <= FLT_MAX) {
                        id_table_entry.id_data_type = IDTABLE::FLOAT32;
                        id_table_entry.id_value.var_float32 = (float)val;
                    }
                    else {
                        id_table_entry.id_data_type = IDTABLE::FLOAT64;
                        id_table_entry.id_value.var_float64 = val;
                    }
                }

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }
            

            //NUMBER

            else if (FST::isIntegerLiteral(local) && current_type == IDTABLE::NUMBER)
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;
                long long val = std::atof(local);

                if (val >= -128 && val <= 127) {
                    id_table_entry.id_data_type = IDTABLE::INT8;
                    id_table_entry.id_value.var_int8 = (int8_t)val;
                }
                else if (val >= -32768 && val <= 32767) {
                    id_table_entry.id_data_type = IDTABLE::INT16;
                    id_table_entry.id_value.var_int16 = (int16_t)val;
                }
                else if (val >= -2147483648LL && val <= 2147483647LL) {
                    id_table_entry.id_data_type = IDTABLE::INT32;
                    id_table_entry.id_value.var_int32 = (int32_t)val;
                }
                else if(val >= LONG_MIN &&  val <= LONG_MAX){
                    id_table_entry.id_data_type = IDTABLE::INT64;
                    id_table_entry.id_value.var_int64 = (int64_t)val;
                }
                else if (val >= -FLT_MAX && val <= FLT_MAX) {
                    id_table_entry.id_data_type = IDTABLE::FLOAT32;
                    id_table_entry.id_value.var_float32 = (float)val;
                }
                else {
                    id_table_entry.id_data_type = IDTABLE::FLOAT64;
                    id_table_entry.id_value.var_float64 = val;
                }

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
                }


            //BOOLEAN

            else if (FST::isBooleanLiteral(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;
                id_table_entry.id_data_type = IDTABLE::BOOLEAN;

                id_table_entry.id_value.var_boolean = (strcmp(local, "true") == 0);

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }



            //SYMBOL

            else if (FST::isSymbolLiteral(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;
                id_table_entry.id_data_type = IDTABLE::SYMBOL;

                id_table_entry.id_value.var_symbol = (local[0] == '\'') ? local[1] : local[0];

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }



            //STRING

            else if (FST::isStringLiteral(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;
                id_table_entry.id_data_type = IDTABLE::STRING;

                std::string string_value = local;
                if (string_value.size() >= 2 && string_value.front() == '"' && string_value.back() == '"')
                    string_value = string_value.substr(1, string_value.size() - 2);
                strncpy_s(id_table_entry.id_value.var_string, sizeof(id_table_entry.id_value.var_string), string_value.c_str(), _TRUNCATE);

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }

           

            //FORMATED STRING

            else if (FST::isFormatedStringLiteral(local))
            {
                COPY(LEX_FORMATED_STRING);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);
                id_table_entry.id_type = IDTABLE::L;
                id_table_entry.id_data_type = IDTABLE::FSTRING; 

                std::string string_value = local;
                if (string_value.size() >= 2 && string_value.front() == '"' && string_value.back() == '"')
                    string_value = string_value.substr(1, string_value.size() - 2);
                strncpy_s(id_table_entry.id_value.var_string, sizeof(id_table_entry.id_value.var_string), string_value.c_str(), _TRUNCATE);

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
                }


            //HELP

            else if (current_type == IDTABLE::HELP && !FST::isIdentifier(local))
            {
                COPY(LEX_LITERAL);
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;
                strcpy_s(id_table_entry.id, ID_MAX_CHARS, "L");
                _itoa_s(lexer.idtable.current_size, id_table_entry.id + 1, ID_MAX_CHARS - 1, 10);

                id_table_entry.id_type = IDTABLE::L;
                id_table_entry.id_data_type = current_type;


                if (FST::isIntegerLiteral(local)) {
                    long long val = std::stoll(local);
                    if (val >= -128 && val <= 127) {
                        id_table_entry.id_data_type = IDTABLE::INT8;
                        id_table_entry.id_value.var_int8 = (int8_t)val;
                    }
                    else if (val >= -32768 && val <= 32767) {
                        id_table_entry.id_data_type = IDTABLE::INT16;
                        id_table_entry.id_value.var_int16 = (int16_t)val;
                    }
                    else if (val >= -2147483648LL && val <= 2147483647LL) {
                        id_table_entry.id_data_type = IDTABLE::INT32;
                        id_table_entry.id_value.var_int32 = (int32_t)val;
                    }
                    else {
                        id_table_entry.id_data_type = IDTABLE::INT64;
                        id_table_entry.id_value.var_int64 = (int64_t)val;
                    }
                }

                else if (FST::isFloatLiteral(local)) {
                    double val = std::stod(local);

                    if (val >= -FLT_MAX && val <= FLT_MAX) {
                        id_table_entry.id_data_type = IDTABLE::FLOAT32;
                        id_table_entry.id_value.var_float32 = (float)val;
                    }
                    else {
                        id_table_entry.id_data_type = IDTABLE::FLOAT64;
                        id_table_entry.id_value.var_float64 = val;
                    }
                }

                else if (FST::isBooleanLiteral(local)) {
                    id_table_entry.id_data_type = IDTABLE::BOOLEAN;
                    id_table_entry.id_value.var_boolean = (strcmp(local, "true") == 0);
                }

                else if (FST::isSymbolLiteral(local)) {
                    id_table_entry.id_data_type = IDTABLE::SYMBOL;
                    id_table_entry.id_value.var_symbol = (local[0] == '\'') ? local[1] : local[0];
                }

                else if (FST::isStringLiteral(local) || FST::isFormatedStringLiteral(local)) {
                    id_table_entry.id_data_type = IDTABLE::STRING;
                    strncpy_s(id_table_entry.id_value.var_string, sizeof(id_table_entry.id_value.var_string), local, _TRUNCATE);
                }

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
            }


            //IDENTIFICATORS

            else if (FST::isIdentifier(local))
            {
                COPY(LEX_IDENTIFICATOR); 
                IDTABLE::Entry id_table_entry;
                id_table_entry.index_first_string_LE = lexer.lextable.current_size;

                strncpy_s(id_table_entry.id, ID_MAX_CHARS, local, _TRUNCATE);

                id_table_entry.id_data_type = current_type;

                if (is_func) {
                    id_table_entry.id_type = IDTABLE::F; 
                    strncpy_s(current_scope, ID_MAX_CHARS, local, _TRUNCATE); 
                    is_func = false;                      
                }
                else {
                    id_table_entry.id_type = IDTABLE::V;
                }

                IDTABLE::Add(lexer.idtable, id_table_entry);
                lextable_entry.indexIT = lexer.idtable.current_size - 1;
                }

            LEXTABLE::Add(lexer.lextable, lextable_entry);
            };


            char quote_symbol = '\0';
            bool is_escaped = false;

            for (int i = 0; i <= in.size; i++)
            {
                char c = in.text[i];

                if (quote_symbol != '\0')
                {
                    if (buf_idx >= (int)sizeof(buffer) - 1)
                        throw ERROR_THROW_PLACE(111, line, col);

                    buffer[buf_idx++] = c;
                    if (is_escaped)
                        is_escaped = false;
                    else if (c == '\\')
                        is_escaped = true;
                    else if (c == quote_symbol)
                        quote_symbol = '\0';

                    col++;
                    continue;
                }

                if (c == '\n')
                {
                    line++;
                    col = 1;
                    continue;
                }

                if (c == '"' || c == '\'')
                {
                    if (buf_idx >= (int)sizeof(buffer) - 1)
                        throw ERROR_THROW_PLACE(111, line, col);

                    quote_symbol = c;
                    buffer[buf_idx++] = c;
                    col++;
                    continue;
                }

                if (isspace((unsigned char)c) || c == ';' || c == ',' || c == '(' || c == ')' ||
                    c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '!' ||
                    c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '\0')
                {
                    if (buf_idx > 0)
                    {
                        processBuffer(buf_idx);
                        buf_idx = 0;
                    }

                    if (c == '\0') break;

                    if (!isspace((unsigned char)c))
                    {
                        LEXTABLE::Entry lextable_entry;
                        lextable_entry.str_number = line;
                        lextable_entry.indexIT = LEXTABLE_IDENTIFICATORTABLE_NULLIDX;

                        switch (c)
                        {
                        case ';': COPY(LEX_SEMICOLON); break;
                        case ',': COPY(LEX_COMMA); break;
                        case '{': COPY(LEX_LEFT_FIGURE_BRACKET); break;
                        case '}': COPY(LEX_RIGHT_FIGURE_BRACKET); break;
                        case '(': COPY(LEX_LEFT_BRACKET); break;
                        case ')': COPY(LEX_RIGHT_BRACKET); break;
                        case '[': COPY(LEX_LEFT_SQUARE_BRACKET); break;
                        case ']': COPY(LEX_RIGHT_SQUARE_BRACKET); break;
                        case ':': COPY(LEX_DOUBLEPOINT); break;
                        case '!': COPY(LEX_EXCLAMATORY); break;
                        case '+': COPY(LEX_PLUS); break;
                        case '-': COPY(LEX_MINUS); break;
                        case '*': COPY(LEX_STAR); break;
                        case '/': COPY(LEX_DIRSLASH); break;
                        case '=': COPY(LEX_EQUAL); break;
                        }
                        LEXTABLE::Add(lexer.lextable, lextable_entry);
                    }
                }
                else
                {
                    if (buf_idx >= (int)sizeof(buffer) - 1)
                        throw ERROR_THROW_PLACE(111, line, col);

                    buffer[buf_idx++] = c;
                }
                col++;
            }

            return lexer;
    }
	
    void PrintLexTable(LOG::Log log, LEXTABLE::LexTable lextable)
    {
        if (!log.stream) return;
        *(log.stream) << "\r\nТаблица лексем\r\n";

        int current_line = -1;
        for (int i = 0; i < lextable.current_size; i++)
        {
            if (lextable.table[i].str_number != current_line)
            {
                current_line = lextable.table[i].str_number;
                char line_num[10];
                sprintf_s(line_num, "\r\n%02d ", current_line);
                *(log.stream) << line_num;
            }

            const char* lexema = lextable.table[i].lexema;
            int lexema_length = 0;
            while (lexema_length < LEXEMA_FIXSIZE - 1 && lexema[lexema_length] != '\0') {
                lexema_length++;
            }
            while (lexema_length > 0 && lexema[lexema_length - 1] == ' ') {
                lexema_length--;
            }

            log.stream->write(lexema, lexema_length);
            *(log.stream) << ' ';
        }

        *(log.stream) << "\r\n";
    }

    static int GetTextLength(const char* text)
    {
        int text_length = 0;

        for (const unsigned char* symbol = (const unsigned char*)text; *symbol != '\0'; symbol++) {
            if ((*symbol & 0xC0) != 0x80) {
                text_length++;
            }
        }

        return text_length;
    }


    static void PrintTableCell(std::ostream& log_stream, const char* text, int width)
    {
        log_stream << text;

        for (int i = GetTextLength(text); i < width; i++) {
            log_stream << ' ';
        }

        log_stream << '\t';
    }

    void PrintIdTable(LOG::Log log, IDTABLE::IdTable idtable)
    {
        if (!log.stream) return;
        std::ostream& log_stream = *log.stream;

        int id_width = GetTextLength("ID") + 1;
        for (int i = 0; i < idtable.current_size; i++) {
            int id_length = GetTextLength(idtable.table[i].id) + 1;
            if (id_length > id_width) {
                id_width = id_length;
            }
        }

        log_stream << "\r\nТаблица идентификаторов\r\n";

        PrintTableCell(log_stream, "N", 4);
        PrintTableCell(log_stream, "ID", id_width);
        PrintTableCell(log_stream, "Тип Данных", 14);
        PrintTableCell(log_stream, "Тип ID", 14);
        PrintTableCell(log_stream, "Первое вхождение", 17);
        log_stream << "Значение\r\n";

        for (int i = 0; i < idtable.current_size; i++)
        {
            IDTABLE::Entry id_table_entry = idtable.table[i];

            char number[16];
            sprintf_s(number, "%d", i);
            PrintTableCell(log_stream, number, 4);

            PrintTableCell(log_stream, id_table_entry.id, id_width);

            switch (id_table_entry.id_data_type)
            {
            case IDTABLE::INT8:    PrintTableCell(log_stream, "int8", 14);    break;
            case IDTABLE::INT16:   PrintTableCell(log_stream, "int16", 14);   break;
            case IDTABLE::INT32:   PrintTableCell(log_stream, "int32", 14);   break;
            case IDTABLE::INT64:   PrintTableCell(log_stream, "int64", 14);   break;
            case IDTABLE::FLOAT32: PrintTableCell(log_stream, "float32", 14); break;
            case IDTABLE::FLOAT64: PrintTableCell(log_stream, "float64", 14); break;
            case IDTABLE::BOOLEAN: PrintTableCell(log_stream, "boolean", 14); break;
            case IDTABLE::SYMBOL:  PrintTableCell(log_stream, "symbol", 14);  break;
            case IDTABLE::STRING:  PrintTableCell(log_stream, "string", 14);  break;
            case IDTABLE::FSTRING: PrintTableCell(log_stream, "fstring", 14); break;
            case IDTABLE::NUMBER:  PrintTableCell(log_stream, "number", 14);  break;
            case IDTABLE::HELP:    PrintTableCell(log_stream, "help", 14);    break;
            default:               PrintTableCell(log_stream, "unknown", 14); break;
            }

            switch (id_table_entry.id_type)
            {
            case IDTABLE::V: PrintTableCell(log_stream, "Переменная", 14); break;
            case IDTABLE::F: PrintTableCell(log_stream, "Функция", 14);    break;
            case IDTABLE::P: PrintTableCell(log_stream, "Параметр", 14);   break;
            case IDTABLE::L: PrintTableCell(log_stream, "Литерал", 14);    break;
            }

            char first_entry[16];
            sprintf_s(first_entry, "%d", id_table_entry.index_first_string_LE);
            PrintTableCell(log_stream, first_entry, 17);

            if (id_table_entry.id_type == IDTABLE::L)
            {
                switch (id_table_entry.id_data_type)
                {
                case IDTABLE::INT8:
                    *(log.stream) << (int)id_table_entry.id_value.var_int8;
                    break;
                case IDTABLE::INT16:
                    *(log.stream) << id_table_entry.id_value.var_int16;
                    break;
                case IDTABLE::INT32:
                    *(log.stream) << id_table_entry.id_value.var_int32;
                    break;
                case IDTABLE::INT64:
                    *(log.stream) << id_table_entry.id_value.var_int64;
                    break;
                case IDTABLE::FLOAT32:
                    *(log.stream) << id_table_entry.id_value.var_float32;
                    break;
                case IDTABLE::FLOAT64:
                    *(log.stream) << id_table_entry.id_value.var_float64;
                    break;
                case IDTABLE::BOOLEAN:
                    *(log.stream) << (id_table_entry.id_value.var_boolean ? "true" : "false");
                    break;
                case IDTABLE::SYMBOL:
                    *(log.stream) << "'" << id_table_entry.id_value.var_symbol << "'";
                    break;
                case IDTABLE::STRING:
                    *(log.stream) << "\"" << id_table_entry.id_value.var_string << "\"";
                    break;
                case IDTABLE::FSTRING:
                    *(log.stream) << "\"" << id_table_entry.id_value.var_fstring << "\"";
                    break;
                default:
                    *(log.stream) << "[value]";
                    break;
                }
            }

            *(log.stream) << "\r\n";
        }
    }
}
