#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "buildAutomat.h"   
#include "enums.h"
#include "Token.h"
using namespace std;
class LexicalAnalysis
{

public:


	 LexicalAnalysis();	
	 Token* TokenLex(string lex, buildAutomat* root_b,int currentLine);
	 Token* getListTokens(string code, buildAutomat* root_b); //פונקציה שמקבלת לקסמה ומחזירה צומת של לקסמה עם טוקן ברשימה
	 void printListToken(Token* head);

	 void deleteTokens(Token* head);

	 //פעולה בונה לא מחזירה ערך
	 //רשימה של רשימות
};








