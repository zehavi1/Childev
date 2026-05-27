
#include <iostream>
#include "LexicalAnalyzer.h"
//#include "buildAutomat.h"




int main()
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