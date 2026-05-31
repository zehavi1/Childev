#pragma once
#include <string>
using namespace std;
#include "enums.h"
struct Lexema
{
	Token typeToken;
	string lex;
	Lexema* nextlex;
	int lineNumber = 1;
};
