#pragma once
#include <string>
using namespace std;
#include "enums.h"
struct Token
{
	TokenType typeToken;
	string lex;
	Token* nextlex;
	int lineNumber = 1;
};
