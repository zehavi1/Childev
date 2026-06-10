//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <memory>
//#include <filesystem>
////#include <windows.h>
//#include "SyntaxAnalysis.h"
//#include "ASTNode.h"
////#include "LexicalAnalyzer.h"
//#include "LexicalAnalysis.h"
//
//using namespace std;
//
//string readFileToString(const string& filePath)
//{
//    ifstream file(filePath);
//
//    if (!file.is_open())
//    {
//        throw runtime_error("Cannot open input file"+filePath);
//    }
//
//    stringstream buffer;
//    buffer << file.rdbuf();
//
//    return buffer.str();
//}
//
//string sanitizeForClient(string text)
//{
//    for (char& ch : text)
//    {
//        if (ch == '|')
//            ch = '/';
//
//        if (ch == '\n' || ch == '\r' || ch == '\t')
//            ch = ' ';
//    }
//
//    return text;
//}
//void setCurrentDirectoryToExeFolder(char* argv0)
//{
//    filesystem::path exePath = filesystem::absolute(argv0);
//    filesystem::path exeFolder = exePath.parent_path();
//
//    filesystem::current_path(exeFolder);
//}
//void addEOFClient(Token* head)
//{
//    Token* p = head;
//    while (p != nullptr && p->nextlex != nullptr)
//    {
//        p = p->nextlex;
//    }
//    if (p->lex != "EOF" || p->typeToken != Tok_EOF)
//    {
//        Token eof;
//        eof.typeToken = Tok_EOF;
//        eof.lex = "EOF";
//        eof.nextlex = nullptr;
//        p->nextlex = new Token(eof);
//    }
//}
//void freeTokenList(Token* head)
//{
//	Token* current = head;
//	while (current != nullptr)
//	{
//		Token* next = current->nextlex;
//		delete current;
//		current = next;
//	}
//}
//int mainclient(int argc, char* argv[])
//{
//    try
//    {
//        
//        if (argc < 2)
//        {
//            cout << "ERROR|0|0|System|Missing input file path" << endl;
//            return 0;
//        }
//        //string inputFilePath = argv[1];
//        filesystem::path inputFilePath = filesystem::absolute(argv[1]);
//        setCurrentDirectoryToExeFolder(argv[0]);
//
//        // קוראים את הקוד שהשרת שמר בקובץ זמני
//        string code = readFileToString(inputFilePath.string());
//
//        //LexicalAnalyzer lexicalAnalysis;
//
//        auto errorReporter = make_shared<ErrorReporter>();
//        auto b = new buildAutomat();
//		LexicalAnalysis lexicalAnalysis;
//        Token* list = lexicalAnalysis.getListTokens(code, b, errorReporter);
//        //addEOFToList(list);
//
//        //Token* list = lexicalAnalysis.getLexemaList(code);
//
//        addEOFClient(list);
//
//        SyntacticAnalysis parser(list, errorReporter);
//
//        shared_ptr<ASTNode> ast = parser.parse();
//
//        // לא להדפיס AST כאן, כי השרת צריך פלט נקי
//        // parser.printASTNodes(ast);
//
//        errorReporter->printErrorsForClient();
//        freeTokenList(list);
//
//        //LexicalAnalyzer::freeLexemaList(list);
//
//        return 0;
//    }
//    catch (const exception& ex)
//    {
//        cout << "ERROR|0|0|System|"
//            << sanitizeForClient(ex.what())
//            << endl;
//
//        return 0;
//    }
//    catch (...)
//    {
//        cout << "ERROR|0|0|System|Unknown compiler error" << endl;
//        return 0;
//    }
//}