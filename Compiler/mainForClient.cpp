#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "SyntaxAnalysis.h"
#include "ASTNode.h"
#include "LexicalAnalyzer.h"

using namespace std;

string readFileToString(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        throw runtime_error("Cannot open input file");
    }

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

string sanitizeForClient(string text)
{
    for (char& ch : text)
    {
        if (ch == '|')
            ch = '/';

        if (ch == '\n' || ch == '\r' || ch == '\t')
            ch = ' ';
    }

    return text;
}
void addEOFClient(Token* head)
{
    Token* p = head;
    while (p != nullptr && p->nextlex != nullptr)
    {
        p = p->nextlex;
    }
    if (p->lex != "EOF" || p->typeToken != Tok_EOF)
    {
        Token eof;
        eof.typeToken = Tok_count;
        eof.lex = "EOF";
        eof.nextlex = nullptr;
        p->nextlex = new Token(eof);
    }
}
int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            cout << "ERROR|0|0|System|Missing input file path" << endl;
            return 0;
        }

        string inputFilePath = argv[1];

        // קוראים את הקוד שהשרת שמר בקובץ זמני
        string code = readFileToString(inputFilePath);

        LexicalAnalyzer lexicalAnalysis;

        auto errorReporter = make_shared<ErrorReporter>();

        Token* list = lexicalAnalysis.getLexemaList(code);

        addEOFClient(list);

        SyntacticAnalysis parser(list, errorReporter);

        shared_ptr<ASTNode> ast = parser.parse();

        // לא להדפיס AST כאן, כי השרת צריך פלט נקי
        // parser.printASTNodes(ast);

        errorReporter->printErrorsForClient();

        LexicalAnalyzer::freeLexemaList(list);

        return 0;
    }
    catch (const exception& ex)
    {
        cout << "ERROR|0|0|System|"
            << sanitizeForClient(ex.what())
            << endl;

        return 0;
    }
    catch (...)
    {
        cout << "ERROR|0|0|System|Unknown compiler error" << endl;
        return 0;
    }
}