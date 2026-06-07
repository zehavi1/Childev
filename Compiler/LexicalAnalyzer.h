#pragma once
#include <string>
#include "Token.h"
class LexicalAnalyzer
{

    private:
        static void addLexema(Token*& head, Token*& tail, TokenType type, const std::string& lex);

        static std::string toLowerStr(const std::string& str);

        static TokenType getKeywordOrIdentifier(const std::string& word);

        static bool isMathOperator(char c);
        

    public:
        LexicalAnalyzer() = default;

        Token* getLexemaList(const std::string& code);

        static void printLexemaList(Token* head);

        static void freeLexemaList(Token* head);
    };


