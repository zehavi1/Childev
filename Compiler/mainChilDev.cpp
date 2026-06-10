#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <filesystem>

#include "SyntaxAnalysis.h"
#include "ASTNode.h"
#include "LexicalAnalysis.h"
#include "buildAutomat.h"
#include "ErrorReporter.h"

using namespace std;
string readFileToString(const string& filePath)
{
    filesystem::path path(filePath);

    ifstream file(path, ios::binary);

    if (!file.is_open())
    {
        throw runtime_error("Cannot open input file " + filePath);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string code = buffer.str();

    if (code.size() >= 3 &&
        (unsigned char)code[0] == 0xEF &&
        (unsigned char)code[1] == 0xBB &&
        (unsigned char)code[2] == 0xBF)
    {
        code = code.substr(3);
    }

    return code;
}
string readFileToString___remove(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        throw runtime_error("Cannot open input file " + filePath);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string code = buffer.str();

    // הסרת BOM אם קיים בתחילת הקובץ
    if (code.size() >= 3 &&
        (unsigned char)code[0] == 0xEF &&
        (unsigned char)code[1] == 0xBB &&
        (unsigned char)code[2] == 0xBF)
    {
        code = code.substr(3);
    }

    return code;
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
string cleanPathFromServer(string path)
{
    // הסרת UTF-8 BOM אם הוא הגיע בתחילת הנתיב
    if (path.size() >= 3 &&
        (unsigned char)path[0] == 0xEF &&
        (unsigned char)path[1] == 0xBB &&
        (unsigned char)path[2] == 0xBF)
    {
        path = path.substr(3);
    }

    // הסרת רווחים / ירידות שורה מסוף הנתיב
    while (!path.empty() &&
        (path.back() == '\r' ||
            path.back() == '\n' ||
            path.back() == '\t' ||
            path.back() == ' '))
    {
        path.pop_back();
    }

    // אם במקרה הנתיב הגיע עם גרשיים
    if (path.size() >= 2 && path.front() == '"' && path.back() == '"')
    {
        path = path.substr(1, path.size() - 2);
    }

    return path;
}
void setCurrentDirectoryToExeFolder(char* argv0)
{
    filesystem::path exePath = filesystem::absolute(argv0);
    filesystem::path exeFolder = exePath.parent_path();

    filesystem::current_path(exeFolder);
}

void addEOF(Token* head)
{
    if (head == nullptr)
    {
        return;
    }

    Token* p = head;

    while (p != nullptr && p->nextlex != nullptr)
    {
        p = p->nextlex;
    }

    if (p->lex != "EOF" || p->typeToken != Tok_EOF)
    {
        Token eof;
        eof.typeToken = Tok_EOF;
        eof.lex = "EOF";
        eof.nextlex = nullptr;

        p->nextlex = new Token(eof);
    }
}

void freeTokenList(Token* head)
{
    Token* current = head;

    while (current != nullptr)
    {
        Token* next = current->nextlex;
        delete current;
        current = next;
    }
}
string analyzeCodeForClient(const string& code, buildAutomat& automat)
{
    Token* list = nullptr;

    try
    {
        auto errorReporter = make_shared<ErrorReporter>();

        LexicalAnalysis lexicalAnalysis;

        // כאן משתמשים באוטומט קיים, לא יוצרים חדש
        list = lexicalAnalysis.getListTokens(code, &automat, errorReporter);

        addEOF(list);

        SyntacticAnalysis parser(list, errorReporter);

        shared_ptr<ASTNode> ast = parser.parse();

        // לא מדפיסים AST ולא רשימת טוקנים במצב שרת
        // כי השרת צריך פלט נקי בלבד

        stringstream output;

        // אם printErrorsForClient מדפיסה ל-cout,
        // נלכוד את הפלט שלה לתוך string
        streambuf* oldCout = cout.rdbuf(output.rdbuf());

        errorReporter->printErrorsForClient();

        cout.rdbuf(oldCout);

        if (list != nullptr)
        {
            freeTokenList(list);
            list = nullptr;
        }

        string result = output.str();

        //// אם אין שגיאות והפעולה לא הדפיסה כלום,
        // נחזיר OK לשרת
        if (result.empty())
        {
            result = "OK\n";
        }

        return result;
    }
    catch (const exception& ex)
    {
        if (list != nullptr)
        {
            freeTokenList(list);
        }

        return "ERROR|0|0|System|" + sanitizeForClient(ex.what()) + "\n";
    }
    catch (...)
    {
        if (list != nullptr)
        {
            freeTokenList(list);
        }

        return "ERROR|0|0|System|Unknown compiler error\n";
    }
}
int runSingleFileMode(const string& inputFilePath)
{
    try
    {
        string code = readFileToString(inputFilePath);

        buildAutomat automat; // במצב רגיל נבנה פעם אחת להרצה הזאת

        string result = analyzeCodeForClient(code, automat);

        cout << result;

        return 0;
    }
    catch (const exception& ex)
    {
        cout << "ERROR|0|0|System|"
            << sanitizeForClient(ex.what())
            << endl;

        return 0;
    }
}
int runServerMode()
{
    try
    {
        // נבנה פעם אחת בלבד כאשר Childev.exe עולה
        buildAutomat automat;

        string inputFilePath;

        // השרת C# ישלח בכל פעם שורה עם נתיב לקובץ
        while (getline(cin, inputFilePath))
        {
            inputFilePath = cleanPathFromServer(inputFilePath);
            if (inputFilePath == "__EXIT__")
            {
                break;
            }

            try
            {
                string code = readFileToString(inputFilePath);

                string result = analyzeCodeForClient(code, automat);

                cout << "BEGIN_RESULT" << endl;

                cout << result;

                if (result.empty() || result.back() != '\n')
                {
                    cout << endl;
                }

                cout << "END_RESULT" << endl;
                cout.flush();
            }
            catch (const exception& ex)
            {
                cout << "BEGIN_RESULT" << endl;
                cout << "ERROR|0|0|System|"
                    << sanitizeForClient(ex.what())
                    << endl;
                cout << "END_RESULT" << endl;
                cout.flush();
            }
        }

        return 0;
    }
    catch (const exception& ex)
    {
        cout << "BEGIN_RESULT" << endl;
        cout << "ERROR|0|0|System|"
            << sanitizeForClient(ex.what())
            << endl;
        cout << "END_RESULT" << endl;
        cout.flush();

        return 0;
    }
}
void chilDev(const string& code)
{
    LexicalAnalysis lexicalAnalysis;

    auto errorReporter = make_shared<ErrorReporter>();
    auto b = new buildAutomat();

    Token* list = lexicalAnalysis.getListTokens(code, b, errorReporter);
    addEOF(list);
    lexicalAnalysis.printListToken(list);

    SyntacticAnalysis parser(list, errorReporter);

    shared_ptr<ASTNode> ast = parser.parse();

    cout << endl << "AST:" << endl;
    parser.printASTNodes(ast);

    errorReporter->printErrors();
    if (list != nullptr)
        freeTokenList(list);
    list = nullptr;
}

int main(int argc, char* argv[])
{
    try
    {
        // חשוב אם buildAutomat קורא קבצים יחסית לתיקיית ה-exe
        setCurrentDirectoryToExeFolder(argv[0]);

        // מצב שרת:
        // Childev.exe --server
        if (argc >= 2 && string(argv[1]) == "--server")
        {
            return runServerMode();
        }

        // מצב רגיל:
        // Childev.exe input.txt
        if (argc >= 2)
        {
            filesystem::path inputFilePath = filesystem::absolute(argv[1]);
            return runSingleFileMode(inputFilePath.string());
        }
        string code="x <- 5\n"
                    "y <- \n"
                    "write(\"after error\", x)\n"
                    "if x = 5 :\n"
                    "write(\"ok\")\n"
                    "||\n"
                    "for i <- 1 to :\n"
                    "write(i)\n"
                    "||\n";
        chilDev(code);
       // cout << "ERROR|0|0|System|Missing input file path" << endl;
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