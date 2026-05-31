
#include <iostream>
#include "LexicalAnalyzer.h"
//#include "buildAutomat.h"
#include <iostream>
#include <vector>
#include <string>

#include "LexicalAnalyzer.h"
#include "SyntaxAnalysis.h"
#include "ASTNode.h"

using namespace std;

vector<Lexema> convertLexemaListToVector(Lexema* head)
{
    vector<Lexema> result;

    Lexema* curr = head;

    while (curr != nullptr)
    {
        Lexema copy = *curr;
        copy.nextlex = nullptr; // כדי שהווקטור לא יחזיק מצביעים ישנים
        result.push_back(copy);

        curr = curr->nextlex;
    }

    // אם ה־Lexer שלך לא מוסיף Tok_count לבד, נוסיף אותו כאן כסוף קלט
    if (result.empty() || result.back().typeToken != Tok_count)
    {
        Lexema eof;
        eof.typeToken = Tok_count;
        eof.lex = "EOF";
        eof.nextlex = nullptr;
        result.push_back(eof);
    }

    return result;
}
int main()
{
    string code1 =
        "x <- 5\n"
        "y <- \n"
        "write(\"after error\", x)\n"
        "if x = 5 :\n"
        "write(\"ok\")\n"
        "||\n"
        "for i <- 1 to :\n"
        "write(i)\n"
        "||\n";
    string code =
        "x <- 5\n"
        "y <- \n"                              // חסר ביטוי אחרי השמה
        "write(\"after first error\", x)\n"
        "\n"

        "write(\"missing close paren\", x\n"   // חסר )
        "z <- 10\n"
        "\n"

        "if x = 5\n"                           // חסר :
        "write(\"if without colon\")\n"
        "||\n"
        "\n"

        "if :\n"                               // חסר תנאי אחרי if
        "write(\"missing condition\")\n"
        "||\n"
        "\n"

        "if x > 3 :\n"
        "write(\"ok\")\n"
        "else :\n"                             // חסר || לפני else
        "write(\"bad else location\")\n"
        "||\n"
        "\n"

        "while :\n"                            // חסר תנאי אחרי while
        "x <- x + 1\n"
        "||\n"
        "\n"

        "while x < 10 :\n"
        "x <- x + \n"                          // חסר ביטוי אחרי +
        "write(\"still inside while\", x)\n"
        "||\n"
        "\n"

        "for i <- 1 to :\n"                    // חסר ערך סיום אחרי to
        "write(i)\n"
        "||\n"
        "\n"

        "for <- 1 to 10 :\n"                   // חסר משתנה אחרי for
        "write(\"bad for\")\n"
        "||\n"
        "\n"

        "for j <- to 10 :\n"                   // חסר ערך התחלה אחרי <-
        "write(j)\n"
        "||\n"
        "\n"

        "for (k <- 1 to 5 :\n"                 // חסר )
        "write(k)\n"
        "||\n"
        "\n"

        "write(\"a\", , x)\n"                  // חסר ביטוי אחרי פסיק
        "write(, x)\n"                         // חסר ביטוי ראשון
        "write(\"missing comma\" x)\n"         // חסר פסיק בין ארגומנטים
        "\n"

        "a b <- 7\n"                           // חסר מפריד / מבנה לא חוקי
        "m <- 3 * \n"                          // חסר ביטוי אחרי *
        "n <- (4 + 5\n"                        // חסר )
        "p <- 2 ^ \n"                          // חסר ביטוי אחרי ^
        "\n"

        "||\n"                                 // סוף בלוק בלי פתיחת בלוק
        "\n"

        "q <- 9\n"
        "write(\"finished\", q)\n";
    LexicalAnalyzer lexicalAnalysis;

    auto errorReporter = make_shared<ErrorReporter>();

    Lexema* list = lexicalAnalysis.getLexemaList(code);

    vector<Lexema> tokens = convertLexemaListToVector(list);

    SyntacticAnalysis parser(tokens, errorReporter);

    shared_ptr<ASTNode> ast = parser.parse();

    cout << endl << "AST:" << endl;
    parser.printASTNodes(ast);

    errorReporter->printErrors();

    LexicalAnalyzer::freeLexemaList(list);

    return 0;
}
int main_check_syntatic()
{
    try
    {
        string code =
            "x <- 5\n"
            "y <- 10\n"
            "sum <- 0\n"
            "write(\"Start program\", x, y)\n"
            "\n"
            "if x = 5 :\n"
            "write(\"x is five\")\n"
            "sum <- x + y * 2\n"
            "||\n"
            "else :\n"
            "write(\"x is not five\")\n"
            "sum <- y\n"
            "||\n"
            "\n"
            "while x < 10 :\n"
            "write(\"x = \", x)\n"
            "x <- x + 1\n"
            "||\n"
            "\n"
            "for i <- 1 to 5 :\n"
            "write(\"i = \", i, \" sum = \", sum)\n"
            "sum <- sum + i\n"
            "||\n"
            "\n"
            "for (j <- 1 to 3) :\n"
            "if j != 2 :\n"
            "write(\"j is not 2\", j)\n"
            "||\n"
            "else :\n"
            "write(\"j is 2\")\n"
            "||\n"
            "||\n"
            "\n"
            "result <- (sum + x) * 2 ^ 3\n"
            "write(\"Final result = \", result)\n";

        cout << "==============================" << endl;
        cout << "SOURCE CODE" << endl;
        cout << "==============================" << endl;
        cout << code << endl;

        LexicalAnalyzer lexicalAnalysis;

        Lexema* list = lexicalAnalysis.getLexemaList(code);

        cout << endl;
        cout << "==============================" << endl;
        cout << "LEXEMA LIST" << endl;
        cout << "==============================" << endl;

        LexicalAnalyzer::printLexemaList(list);

        vector<Lexema> tokens = convertLexemaListToVector(list);

        cout << endl;
        cout << "==============================" << endl;
        cout << "SYNTAX ANALYSIS - AST" << endl;
        cout << "==============================" << endl;

        SyntacticAnalysis parser(tokens);

        shared_ptr<ASTNode> ast = parser.parse();

        parser.printASTNodes(ast);

        cout << endl;
        cout << "==============================" << endl;
        cout << "PARSE FINISHED SUCCESSFULLY" << endl;
        cout << "==============================" << endl;

        LexicalAnalyzer::freeLexemaList(list);
    }
    catch (const exception& ex)
    {
        cerr << endl;
        cerr << "ERROR:" << endl;
        cerr << ex.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << endl;
        cerr << "UNKNOWN ERROR" << endl;
        return 1;
    }

    return 0;
}



int checkLexer()
{

    //buildAutomat b; //בניית האוטומט- בניית עצם מסוג ...

    //LexicalAnalysis l;
    //l.getLexema("123");
    //l.
    std::string code =
        "x<-5\n"
        "if x=5 : write(\"yes\") ||\n";

    LexicalAnalyzer lexicalAnalysis;

    Lexema* list = lexicalAnalysis.getLexemaList(code);

    LexicalAnalyzer::printLexemaList(list);

    LexicalAnalyzer::freeLexemaList(list);


    return 0;


}