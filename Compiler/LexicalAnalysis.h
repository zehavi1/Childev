#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "buildAutomat.h"   
#include "enums.h"
#include "Token.h"
#include "ErrorReporter.h"
using namespace std;
class LexicalAnalysis
{
private:
	void reportLexicalError(
		shared_ptr<ErrorReporter> errorReporter,
		const string& message,
		const string& lexeme,
		int lineNumber,
		int columnNumber = 0
	);

public:


	 LexicalAnalysis();	

	 Token* TokenLex(string lex, buildAutomat* root_b,int currentLine, shared_ptr<ErrorReporter> errorReporter = nullptr,
		 int columnNumber = 0);
	 Token* getListTokens(string code, buildAutomat* root_b, shared_ptr<ErrorReporter> errorReporter = nullptr); //פונקציה שמקבלת לקסמה ומחזירה צומת של לקסמה עם טוקן ברשימה
	 void printListToken(Token* head);

	 void deleteTokens(Token* head);

	 //פעולה בונה לא מחזירה ערך
	 //רשימה של רשימות
};








