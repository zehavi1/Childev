#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "buildAutomat.h"   
using namespace std;

struct Lexema
{
	int numToken;
	string lex;
	Lexema* nextlex;
};

struct expression
{
	string type;
	string value;
};


class LexicalAnalysis
{

public:


	 LexicalAnalysis();	
	 Lexema* TokenLex(string lex, buildAutomat* root_b);
	 Lexema** getListLexema(string code, buildAutomat* root_b); //פונקציה שמקבלת לקסמה ומחזירה צומת של לקסמה עם טוקן ברשימה
	 
	 //פעולה בונה לא מחזירה ערך
	 //רשימה של רשימות
};








