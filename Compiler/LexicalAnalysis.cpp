#include "LexicalAnalysis.h"
#include "buildAutomat.h"

LexicalAnalysis::LexicalAnalysis()
{
}

void LexicalAnalysis::reportLexicalError(
	shared_ptr<ErrorReporter> errorReporter,
	const string& message,
	const string& lexeme,
	int lineNumber,
	int columnNumber
)
{
	if (errorReporter != nullptr)
	{
		errorReporter->report(
			ErrorKind::Lexical,
			message,
			lexeme,
			lineNumber,
			columnNumber
		);
	}
}

Token* LexicalAnalysis::TokenLex(
	string lex,
	buildAutomat* root_b,
	int currentLine,
	shared_ptr<ErrorReporter> errorReporter,
	int columnNumber
)
//פונקצייה המקבלת לקסמה ומצביע למבנה האוטומט וממספר שורה, 
//הפונקציה עוברת על האוטומט למציאת טוקן מקבל
//הפונקציה מחזירה מצביע חטוקן המכיל: מספר טוקן, לקסמה, מספר שורה,ומצביע לטוקן הבא
//אם הלקסמה אינה חוקית - נרשמת שגיאה לקסיקלית והפונקציה מחזירה nullptr
{
	State* current = &(root_b->arrState[0]);

	for (int i = 0; i < lex.length(); i++)
	{
		unsigned char c = lex[i];

		if (current->hashState[c] == nullptr)
		{
			reportLexicalError(
				errorReporter,
				"Unrecognized token",
				lex,
				currentLine,
				columnNumber
			);

			return nullptr;
		}

		current = current->hashState[c];
	}

	if (current->numToken == -1)
	{
		reportLexicalError(
			errorReporter,
			"Unrecognized token",
			lex,
			currentLine,
			columnNumber
		);

		return nullptr;
	}

	Token* node = new Token();
	node->lex = lex;
	node->typeToken = (TokenType)current->numToken;
	node->lineNumber = currentLine;
	node->nextlex = nullptr;

	return node;
}

//פונקצייה שמקבלת קוד, ומצביע לאוטומט
//TokenLex  הפונקציה עוברת על הקוד, מחלקת ללקסמות, בודקת תווים מיוחדים ומזמנת את הפונקציה 
//הפונקציה בונה רשימה של טוקנים ומחזירה אותה.

Token* LexicalAnalysis::getListTokens(
	string code,
	buildAutomat* root_b,
	shared_ptr<ErrorReporter> errorReporter
)
{
	Token* listHead = nullptr;
	Token* listTail = nullptr;

	int currentLine = 1;
	int currentColumn = 1;

	//רשימת מפרידים לבדוק אם לעשות בenum ומה עוד ברשימה
	string List_of_separators = " ()|&:,/+-*^={}\"'<>!=;\n\t\r";

	int x = 0;
	string lexema_from_code = "";

	int tokenLine = currentLine;
	int tokenColumn = currentColumn;

	auto advance = [&]()
		{
			if (x >= code.length())
				return;

			if (code[x] == '\n')
			{
				currentLine++;
				currentColumn = 1;
			}
			else
			{
				currentColumn++;
			}

			x++;
		};

	auto addNode = [&](Token* node) //פונקציית למדה הוספת טוקן לרשימת הטוקנים
		{
			if (node == nullptr) return; //השגיאה כבר תועדה במקום שבו נוצרה

			node->nextlex = nullptr;

			if (listHead == nullptr) //אם הרשימה ריקה
				listHead = listTail = node;
			else
			{
				listTail->nextlex = node;
				listTail = node;
			}
		};

	auto addTokenFromLexeme = [&](const string& lexeme, int line, int column)
		{
			if (lexeme.empty())
				return;

			Token* token = TokenLex(
				lexeme,
				root_b,
				line,
				errorReporter,
				column
			);

			addNode(token);
		};

	auto flushRegularLexeme = [&]()
		{
			if (!lexema_from_code.empty())
			{
				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
			}
		};

	while (x < code.length())
	{
		//אם זה לא מתווי המפרידים
		if (List_of_separators.find(code[x]) == string::npos)
		{
			if (lexema_from_code.empty())
			{
				tokenLine = currentLine;
				tokenColumn = currentColumn;
			}

			lexema_from_code += code[x];
			advance();
		}
		else
		{
			//אם זה מתווי המפרידים-שולחים את הלקסמה האחרונה לאוטומט-רק אם היא לא ריקה
			flushRegularLexeme();

			tokenLine = currentLine;
			tokenColumn = currentColumn;

			//טיפול במפריד
			switch (code[x])
			{
			case '\n':
			case ';':
			{
				if ((listTail != nullptr &&
					(listTail->typeToken == Tok_newline || listTail->typeToken == Tok_semicolon))
					|| (listTail == nullptr))
				{
					advance();
					continue;
				}

				// בשפה שלנו ירידת שורה ונקודה־פסיק משמשות כמפריד פקודות.
				// אם באוטומט  ירידת שורה מוגדרת אחרת, אפשר להחליף כאן ל-"\n".
				lexema_from_code = ";";
				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";

				advance();
				continue;
			}

			case ' ':
			case '\t':
			case '\r':
				lexema_from_code = "";
				advance();
				continue;

			case '\'':
			{
				lexema_from_code += code[x];
				advance();

				if (x < code.length() && code[x] != '\'' && code[x] != '\n') // התו הפנימי
				{
					lexema_from_code += code[x];
					advance();
				}
				else
				{
					reportLexicalError(
						errorReporter,
						"Invalid char literal",
						lexema_from_code,
						tokenLine,
						tokenColumn
					);

					//תו לא סגור
					lexema_from_code = "";
					continue;
				}

				if (x < code.length() && code[x] == '\'') // ' סוגר
				{
					lexema_from_code += code[x];
					advance();

					addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
					lexema_from_code = "";
					continue;
				}
				else
				{
					//תו לא סגור
					reportLexicalError(
						errorReporter,
						"Unclosed char literal",
						lexema_from_code,
						tokenLine,
						tokenColumn
					);

					// התאוששות: מדלגים עד מפריד או עד סוף שורה
					while (x < code.length()
						&& List_of_separators.find(code[x]) == string::npos
						&& code[x] != '\n')
					{
						advance();
					}

					lexema_from_code = "";
					continue;
				}
			}

			case '{':
			{
				lexema_from_code += code[x];
				advance();

				while (x < code.length() && code[x] != '}')
				{
					lexema_from_code += code[x];
					advance();
				}

				if (x < code.length() && code[x] == '}') // אם מצאנו את הסוגר
				{
					lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
					advance();

					//addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
					lexema_from_code = "";
					continue;
				}
				else
				{
					//הוספה שגיאה מתאימה לרשימת השגיאות
					reportLexicalError(
						errorReporter,
						"Unclosed comment",
						lexema_from_code,
						tokenLine,
						tokenColumn
					);

					lexema_from_code = "";
					continue;
				}
			}

			case '"':
			{
				lexema_from_code += code[x];
				advance();

				while (x < code.length() && code[x] != '"')
				{
					lexema_from_code += code[x];
					advance();
				}

				if (x < code.length() && code[x] == '"') // אם מצאנו את הסוגר
				{
					lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
					advance();

					addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
					lexema_from_code = "";
					continue;
				}
				else
				{
					//הוספה שגיאה מתאימה לרשימת השגיאות
					reportLexicalError(
						errorReporter,
						"Unclosed string",
						lexema_from_code,
						tokenLine,
						tokenColumn
					);

					lexema_from_code = "";
					continue;
				}
			}

			case '|':
			{
				lexema_from_code += code[x];
				advance();

				if (x < code.length() && code[x] == '|')
				{
					lexema_from_code += code[x];
					advance();
				}

				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
				continue;
			}

			case '<':
			{
				lexema_from_code += code[x];
				advance();

				if (x < code.length() && (code[x] == '=' || code[x] == '-'))
				{
					lexema_from_code += code[x];
					advance();
				}

				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
				continue;
			}

			case '>':
			{
				lexema_from_code += code[x];
				advance();

				if (x < code.length() && code[x] == '=')
				{
					lexema_from_code += code[x];
					advance();
				}

				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
				continue;
			}

			case '!':
			{
				if (x + 1 < code.length() && code[x + 1] == '=')
				{
					lexema_from_code += code[x];
					advance();

					lexema_from_code += code[x];
					advance();

					addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
					lexema_from_code = "";
					continue;
				}
				else
				{
					//שגיאה אם זה לא !=
					reportLexicalError(
						errorReporter,
						"'!' alone is not legal. Did you mean '!='?",
						"!",
						tokenLine,
						tokenColumn
					);

					lexema_from_code = "";
					advance();
					continue;
				}
			}

			case '=':
			{
				lexema_from_code += code[x];
				advance();

				if (x < code.length() && (code[x] == '<' || code[x] == '>'))
				{
					lexema_from_code += code[x];
					advance();
				}
				else if (x < code.length() && code[x] == '!')
				{
					reportLexicalError(
						errorReporter,
						"Invalid comparison operator. Did you mean '!='?",
						"=!",
						tokenLine,
						tokenColumn
					);

					advance();
					lexema_from_code = "";
					continue;
				}

				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
				continue;
			}

			default:
				//כלומר אם  code[x] הוא מפריד אבל לא אחד מהמיוחדים שצריך לבדוק אם יש להם תו נוסף אחרי או לא- כלומר מפריד רגיל כמו ()&+-*^:,
				lexema_from_code = code[x];
				advance();

				addTokenFromLexeme(lexema_from_code, tokenLine, tokenColumn);
				lexema_from_code = "";
				continue;
			}
		}
	}

	if (!lexema_from_code.empty())
	{
		Token* lexema_end = TokenLex(
			lexema_from_code,
			root_b,
			tokenLine,
			errorReporter,
			tokenColumn
		);

		addNode(lexema_end);
	}

	return listHead;
}

void LexicalAnalysis::printListToken(Token* head)
{
	Token* current = head;

	while (current != nullptr)
	{
		cout << "lexema: "
			<< current->lex
			<< " | token: "
			<< current->typeToken
			<< " | lineNumber: "
			<< current->lineNumber
			<< endl;

		current = current->nextlex;
	}
}

void LexicalAnalysis::deleteTokens(Token* head)
{
	Token* current = head;

	while (current != nullptr)
	{
		Token* next = current->nextlex;
		delete current;
		current = next;
	}
}