#pragma once
#include <string>
#include "enums.h"
using namespace std;
#include "ASTNode.h"


class SyntaxAnalysis
{
private:
    Lexema** list;     // מערך של רשימות מקושרות
    int listCount;     // מספר הרשימות
    int i;             // אינדקס הרשימה הנוכחית
    Lexema* lex;       // האיבר הנוכחי
    void MoveToFirstNonEmptyList()
    {
        while (i < listCount && list[i] == nullptr)
        {
            i++;
        }

        if (i < listCount)
            lex = list[i];
        else
            lex = nullptr;
    }

public:
         

    SyntaxAnalysis(Lexema** list, int listCount)
    {
        this->list = list;
        this->listCount = listCount;
        this->i = 0;
        this->lex = nullptr;

        MoveToFirstNonEmptyList();
    }

    Lexema* Next()
    {
        // אם אין איבר נוכחי — אין לאן להתקדם
        if (lex == nullptr)
            return nullptr;

        // אם יש איבר הבא באותה רשימה
        if (lex->nextlex != nullptr)
        {
            lex = lex->nextlex;
            return lex;
        }

        // אחרת עוברים לרשימה הבאה
        i++;

        // מדלגים על רשימות ריקות
        while (i < listCount && list[i] == nullptr)
        {
            i++;
        }

        // אם קיימת רשימה נוספת
        if (i < listCount)
            lex = list[i];
        else
            lex = nullptr;

        return lex;
    }

    Lexema* Current()
    {
        return lex;
    }

    //shared_ptr<TokenNode> match(Pattern pattern, string msg = "UnExcepted");
    //shared_ptr<ASTNode> expression();
    //shared_ptr<ASTNode> term();
    //shared_ptr<ASTNode> factor();
    //shared_ptr<ASTNode> number();
    //shared_ptr<ASTNode> type(); // ניתוח סוגי משתנים

    ////משתנים
    //shared_ptr<ASTNode> declaration();
    //shared_ptr<ASTNode> declaration1(bool canBeFunction = 1);
    //shared_ptr<ASTNode> variable(Pattern typeVariable); // ניתוח משתנה
    //shared_ptr<ASTNode> variable_list(Pattern typeVariable); // ניתוח מזהה
    //shared_ptr<ASTNode> printf_statement();
    ////משפטי השמה והדפסה
    //shared_ptr<ASTNode> print_statement();
    //shared_ptr<ASTNode> variable_declaration(); // ניתוח הצהרות משתנים
    //shared_ptr<ASTNode> expr_print();
    //shared_ptr<ASTNode> expressionInPrint();
    //shared_ptr<ASTNode> assignment(); // ניתוח השמות
    //shared_ptr<ASTNode> assignment1();
    //shared_ptr<ASTNode> statement(); // ניתוח פקודות
    ////פקודות if
    //shared_ptr<ASTNode> if_else_statement(); // ניתוח פקודות if-else
    //shared_ptr<ASTNode> elif_statement(); // ניתוח פקודות if-else-if
    //Token peekNextToken();
    ////תנאים חדש
    //shared_ptr<ASTNode> exprOr();
    //shared_ptr<ASTNode> exprAnd();
    //shared_ptr<ASTNode> exprComparison();
    //shared_ptr<ASTNode> exprArithmetic();
    //shared_ptr<ASTNode> comparison_operator(); // ניתוח אופרטורים להשוואה
    //shared_ptr<ASTNode> block(); // ניתוח בלוקים

    ////לולאות
    //shared_ptr<ASTNode> for_loop(); // ניתוח לולאות for
    //shared_ptr<ASTNode> while_loop(); // ניתוח לולאות while
    //shared_ptr<ASTNode> foreach_loop(); // ניתוח לולאות foreach
    //shared_ptr<ASTNode> function_definition(shared_ptr<ParentNode> p = nullptr); // ניתוח הגדרות פונקציות
    //shared_ptr<ASTNode> collection(); // ניתוח אוספים
    //shared_ptr<ASTNode> parameter_list(); // ניתוח רשימת פרמטרים

};