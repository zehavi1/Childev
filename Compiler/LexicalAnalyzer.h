#pragma once
#include <string>
#include "Lexema.h"
class LexicalAnalyzer
{

    private:
        static void addLexema(Lexema*& head, Lexema*& tail, Token type, const std::string& lex);

        static std::string toLowerStr(const std::string& str);

        static Token getKeywordOrIdentifier(const std::string& word);

        static bool isMathOperator(char c);
        

    public:
        LexicalAnalyzer() = default;

        Lexema* getLexemaList(const std::string& code);

        static void printLexemaList(Lexema* head);

        static void freeLexemaList(Lexema* head);
    };


