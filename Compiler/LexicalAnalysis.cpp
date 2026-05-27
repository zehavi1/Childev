#include "LexicalAnalysis.h"
#include "buildAutomat.h"



LexicalAnalysis::LexicalAnalysis()
{
	
	/*string lexema=*/  //פונקציה שעוברת עד לקבלת תו מפריד
	/*int Tok_index=*/ //פונקציה שמקבלת לקסמה ומחזירה מס טוקן
	//בניית הרשימה
}
Lexema* LexicalAnalysis::TokenLex(string lex, buildAutomat* root_b)
{
	State* current = &root_b->arrState[0];

	for (int i = 0; i < lex.length(); i++)
	{
		unsigned char c = lex[i];
		if (current->hashState[c] == nullptr)
			return nullptr;
		current = current->hashState[c];
	}

	if (current->numToken == -1)
		return nullptr;

	Lexema* node = new Lexema();
	node->lex = lex;
	node->typeToken = getTokenType(lex);
	node->nextlex = nullptr;
	return node;
}

//פעולה 
Lexema** LexicalAnalysis::getListLexema(string code,buildAutomat* root_b)
{
	int maxLines = code.length();
	Lexema** list_lexema = new Lexema * [maxLines](); // אתחול ל-nullptr
	int lineIndex = 0;
	string List_of_separators = " ()||&|:,/+-*^={\"'<>!=";//רשימת מפרידים לבדוק אם לעשות בenum ומה עוד ברשימה
	int x = 0;
	string lexema_from_code = "";
	//קוד לא זז ולן צריך לשנות את זהwhile (code!= "") 
	while (x < code.length())
	{
		//	()&+-*^:
	   //	| || //case לבדיקה אם זה or או סיום של if או while
	   //    > < !//case לבדיקה אם זה > או >= או < או <= או != לסייג את !
	   //	=//case לבדיקה אם זה = או >= או < או <= או !=  את =
		Lexema* lineHead = nullptr;
		Lexema* lineTail = nullptr;

		while (x < code.length() && code[x] != '\n' && code[x] != ';')//עבור כל שורה
		{
			//אם זה לא מתווי המפרידים
			if (List_of_separators.find(code[x]) == string::npos)
			{
				lexema_from_code += code[x];
				x++;
			}
			else
			{	//אם זה מתווי המפרידים-שולחים את הלקסמה האחרונה לאוטומט-רק אם היא לא ריקה
				if (!lexema_from_code.empty())
				{
					Lexema* l1 = TokenLex(lexema_from_code, root_b);
					if (l1 != nullptr)
					{
						l1->nextlex = nullptr;
						if (lineHead == nullptr)
							lineHead = lineTail = l1;     // צומת ראשונה בשורה
						else                             // מחברים לסוף השרשרת
						{
							lineTail->nextlex = l1;
							lineTail = l1;
						}
					}
					lexema_from_code = "";
				}



				//טיפול במפריד
				switch (code[x])
				{

				case ' ':
					lexema_from_code = "";
					cout << "One\n";
					break;
				case '\'':
					lexema_from_code += code[x];
					x++;
					if (x < code.length() && code[x] != '\'') // התו הפנימי
					{
						lexema_from_code += code[x];
						x++;
					}
					if (x < code.length() && code[x] == '\'') // ' סוגר
						lexema_from_code += code[x];
					break;


				case '{':
					lexema_from_code += code[x];
					x++;
					while (x < code.length() && code[x] != '}')
					{
						lexema_from_code += code[x];
						x++;
					}
					if (x < code.length() && code[x] == '}') // אם מצאנו את הסוגר
						lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
					else
						cout << "שגיאה לקסיקאלית: הערה לא סגורה\n";
					cout << "הערה- כלומר תמשיך לקרוא פה עד הסלסלים הבאים אם הגעת לסוף שגיאה\n";
					break;

				case '"':
					lexema_from_code += code[x];
					x++;
					while (x < code.length() && code[x] != '"')
					{
						lexema_from_code += code[x];
						x++;
					}
					if (x < code.length() && code[x] == '"') // אם מצאנו את הסוגר
						lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
					else
						cout << "שגיאה לקסיקאלית: מחרוזת לא סגורה\n";


					cout << "מחרוזת- כלומר תמשיך לקרוא פה עד גרשיים הבאים אם הגעת לסוף שגיאה\n";
					break;
				case '|':
					if (x + 1 < code.length() && code[x + 1] == '|')
					{
						lexema_from_code += code[x];
						x++;
					}
					lexema_from_code += code[x];

					cout << "One\n";
					break;
				case '<':
				case '>':
				case '!':
					if (x + 1 < code.length() && code[x + 1] == '=')
					{
						lexema_from_code += code[x];
						x++;

					}
					else {
						if (code[x] == '!')
						{
							//שגיאה אם זה לא !=
							cout << "שגיאה לקסיקאלית: ! לבד אינו חוקי\n";
							lexema_from_code = "";
							//להוסיף לרשימת השגיאות לקסיקאליות
							break;

						}
					}

					lexema_from_code += code[x];


					cout << "One\n";
					break;
				case '=':

					if (x + 1 < code.length() && (code[x + 1] == '<' || code[x + 1] == '>' || code[x + 1] == '!'))
					{
						lexema_from_code += code[x];
						x++;
					}
					lexema_from_code += code[x];



					cout << "One\n";
					break;

					//else
					//{
					//	//מה עושים המקרה שיש = לבד?
					//}

				default://כלומר אם  code[x] הוא מפריד אבל לא אחד מהמיוחדים שצריך לבדוק אם יש להם תו נוסף אחרי או לא- כלומר מפריד רגיל כמו ()&+-*^:,
					//מה זה עושה ? בכל מקרה? או כמו else?
					lexema_from_code = code[x];

					//cout << "Unknown\n";
					break;
				}

				if (x < code.length() && (code[x] == ' ' || code[x]=='!'))
				{
					x++;
					continue;
				}
					
				Lexema* l = TokenLex(lexema_from_code, root_b);
				if (l != nullptr)
				{
					l->nextlex = nullptr;
					if (lineHead == nullptr)
						lineHead = lineTail = l;     // צומת ראשונה בשורה
					else                             // מחברים לסוף השרשרת
					{
						lineTail->nextlex = l;
						lineTail = l;
					}
				}

				
				x++;
				lexema_from_code = "";



				//שורות מיותרות לכאורה
				//שליחת הלקסמה לאוטומט
				//Lexema* l = TokenLex(lexema_from_code);
				//lexema_from_code = "";
				//יצירת מצביע לרשימה חדשה המייצגת שורה חדשה
				//הוספת מצביע זה לרשימה של מצביעים ללקס
				//Lexema* l = this.tokenLex(lexema_from_code);//הוספת טוקן לרשימה



			}
		}
		// אם נשארה לקסמה אחרונה לפני ; או סוף שורה
		if (!lexema_from_code.empty())
		{
			Lexema* l1 = TokenLex(lexema_from_code, root_b);

			if (l1 != nullptr)
			{
				l1->nextlex = nullptr;

				if (lineHead == nullptr)
					lineHead = lineTail = l1;
				else
				{
					lineTail->nextlex = l1;
					lineTail = l1;
				}
			}

			lexema_from_code = "";
		}

		// אם השורה הסתיימה ב-; נוסיף אותו כטוקן בפני עצמו
		if (x < code.length() && code[x] == ';')
		{
			Lexema* endLex = TokenLex(";", root_b);

			if (endLex != nullptr)
			{
				endLex->nextlex = nullptr;

				if (lineHead == nullptr)
					lineHead = lineTail = endLex;
				else
				{
					lineTail->nextlex = endLex;
					lineTail = endLex;
				}
			}

			x++; // מדלגים על ;
		}
		else if (x < code.length() && code[x] == '\n')
		{
			x++; // מדלגים על ירידת שורה
		}

		// שומרים את הרשימה של השורה
		list_lexema[lineIndex] = lineHead;
		lineIndex++;
								
		//אם אתה לא מרשימת המפרידים
	}
	return list_lexema;
	
}
Token LexicalAnalysis::getTokenType(const string& lex)
{
	if (lex == "else")  return Tok_else;
	if (lex == "false") return Tok_false;
	if (lex == "for")   return Tok_for;
	if (lex == "if")    return Tok_if;
	if (lex == "read")  return Tok_read;
	if (lex == "to")    return Tok_to;
	if (lex == "true")  return Tok_true;
	if (lex == "write") return Tok_write;
	if (lex == "while") return Tok_while;

	if (lex == "+" || lex == "-" || lex == "*" || lex == "/" || lex == "%")
		return Tok_math;

	if (lex == "==" || lex == "!=" || lex == ">" || lex == "<" || lex == ">=" || lex == "<=")
		return Tok_comp;

	if (lex == "=")
		return Tok_eq;

	if (lex == "&&" || lex == "||")
		return Tok_log;

	if (lex == ";")
		return Tok_end;

	if (lex == "(")
		return Tok_Left_paren;

	if (lex == ")")
		return Tok_Right_paren;

	if (lex == ",")
		return Tok_comma;

	if (lex == ":")
		return Tok_Colon;

	if (lex.length() >= 2 && lex[0] == '"' && lex[lex.length() - 1] == '"')
		return Tok_string;

	if (lex.length() >= 3 && lex[0] == '\'' && lex[lex.length() - 1] == '\'')
		return Tok_char;

	bool isNumber = true;

	if (lex.empty())
		return Tok_error;

	for (char c : lex)
	{
		if (!isdigit((unsigned char)c))
		{
			isNumber = false;
			break;
		}
	}

	if (isNumber)
		return Tok_num;

	return Tok_identifier;
}
