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
	node->numToken = current->numToken;
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
		list_lexema[lineIndex] = lineHead;
		lineIndex++;
		x++;
								
		//אם אתה לא מרשימת המפרידים
	}
	return list_lexema;
	
}
