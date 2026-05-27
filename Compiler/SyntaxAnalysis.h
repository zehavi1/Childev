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



};