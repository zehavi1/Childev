#include "LexicalAnalysis.h"
#include "buildAutomat.h"
LexicalAnalysis::LexicalAnalysis()
{
}
Token* LexicalAnalysis::TokenLex(string lex, buildAutomat* root_b, int currentLine)
//פונקצייה המקבלת לקסמה ומצביע למבנה האוטומט וממספר שורה, 
//הפונקציה עוברת על האוטומט למציאת טוקן מקבל
//הפונקציה מחזירה מצביע חטוקן המכיל: מספר טוקן, לקסמה, מספר שורה,ומצביע לטוקן הבא
//צריך לזרוק פה שגיאות!!!!!!
{
	State* current = &(root_b->arrState[0]);

	for (int i = 0; i < lex.length(); i++)
	{
		unsigned char c = lex[i];
		if (current->hashState[c] == nullptr)
			return nullptr;
		current = current->hashState[c];
	}

	if (current->numToken == -1)
		return nullptr;

	Token* node = new Token();
	node->lex = lex;
	node->typeToken =(TokenType)current->numToken;
	node->lineNumber = currentLine;
	node->nextlex = nullptr;
	return node;
}

//פונקצייה שמקבלת קוד, ומצביע לאוטומט
//TokenLex  הפונקציה עוברת על הקוד, מחלקת ללקסמות, בודקת תווים מיוחדים ומזמנת את הפונקציה 
//הפונקציה בונה רשימה של טוקנים ומחזירה אותה.

Token* LexicalAnalysis::getListTokens(string code, buildAutomat* root_b)
{
	Token* listHead = nullptr;
	Token* listTail = nullptr;
	int currentLine = 0;
	//רשימת מפרידים לבדוק אם לעשות בenum ומה עוד ברשימה
	string List_of_separators = " ()|&:,/+-*^={}\"'<>!=;\n";
	int x = 0;
	string lexema_from_code = "";
	auto addNode = [&](Token* node) //פונקציית למדה הוספת טוקן לרשימת הטוקנים
		{
			if (node == nullptr) return;//צריך לבצע הוספה לרשימת השגיאות
			node->nextlex = nullptr;
			if (listHead == nullptr) //אם הרשימה ריקה
				listHead = listTail = node;
			else
			{
				listTail->nextlex = node;
				listTail = node;
			}
		};
	int tokenLine = currentLine;
	while (x < code.length())
	{


		//אם זה לא מתווי המפרידים
		if (List_of_separators.find(code[x]) == string::npos)
		{
			lexema_from_code += code[x];
			x++;
		}
		else
		{
			//אם זה מתווי המפרידים-שולחים את הלקסמה האחרונה לאוטומט-רק אם היא לא ריקה
			if (!lexema_from_code.empty())
			{
				Token* l1 = TokenLex(lexema_from_code, root_b, currentLine);
				addNode(l1);
				lexema_from_code = "";
			}
			//טיפול במפריד
			switch (code[x])
			{
			case '\n':
				currentLine++;
			case ';':
			{
				if ((listTail != nullptr && listTail->typeToken == Tok_newline) || (listTail == nullptr) /*(code[x - 1] == ';' || code[x - 1] == '\n')*/)
				{
					x++;
					continue;

				}
				tokenLine = currentLine;
				lexema_from_code = ";";
				
				break;
			}
			case ' ':
				lexema_from_code = "";
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
				else {
					x++;
					//תו לא סגור
					continue;
				}
				//הוספה שגיאה מתאימה לרשימת השגיאות
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
				else {
					x++;
					cout << "שגיאה לקסיקאלית: הערה לא סגורה\n";
					continue;

				}

				break;
				//הוספה שגיאה מתאימה לרשימת השגיאות


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
				{
					x++;
					cout << "שגיאה לקסיקאלית: מחרוזת לא סגורה\n";
					continue;
				}
				//הוספה שגיאה מתאימה לרשימת השגיאות


				break;
			case '|':
				if (x + 1 < code.length() && code[x + 1] == '|')
				{
					lexema_from_code += code[x];
					x++;
				}
				lexema_from_code += code[x];
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
					if (code[x] == '<')
					{
						if (x + 1 < code.length() && code[x + 1] == '-')
						{
							lexema_from_code += code[x];
							x++;
						}
					}
				}
				lexema_from_code += code[x];
				break;
			case '=':

				if (x + 1 < code.length() && (code[x + 1] == '<' || code[x + 1] == '>' || code[x + 1] == '!'))
				{
					lexema_from_code += code[x];
					x++;
				}
				lexema_from_code += code[x];
				break;
			default://כלומר אם  code[x] הוא מפריד אבל לא אחד מהמיוחדים שצריך לבדוק אם יש להם תו נוסף אחרי או לא- כלומר מפריד רגיל כמו ()&+-*^:,
				//מה זה עושה ? בכל מקרה? או כמו else?
				lexema_from_code = code[x];
				break;
			}
			if (x < code.length() && (code[x] == ' ' || code[x] == '!'))
			{
				x++;
				continue;
			}
			if (!lexema_from_code.empty())
			{
				Token* l = TokenLex(lexema_from_code, root_b, tokenLine);
				addNode(l);
			}
			x++;
			lexema_from_code = "";
		}
	}
	if (!lexema_from_code.empty())
	{
		Token* lexema_end = TokenLex(lexema_from_code, root_b, tokenLine);

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

//LexicalAnalysis::LexicalAnalysis()
//{
//}
//Token* LexicalAnalysis::TokenLex(string lex, buildAutomat* root_b, int currentLine) 
////פונקצייה המקבלת לקסמה ומצביע למבנה האוטומט וממספר שורה, 
////הפונקציה עוברת על האוטומט למציאת טוקן מקבל
////הפונקציה מחזירה מצביע חטוקן המכיל: מספר טוקן, לקסמה, מספר שורה,ומצביע לטוקן הבא
////צריך לזרוק פה שגיאות!!!!!!
//{
//	State* current = &root_b->arrState[0];
//
//	for (int i = 0; i < lex.length(); i++)
//	{
//		unsigned char c = lex[i];
//		if (current->hashState[c] == nullptr)
//			return nullptr;
//		current = current->hashState[c];
//	}
//
//	if (current->numToken == -1)
//		return nullptr;
//
//	Token* node = new Token();
//	node->lex = lex;
//	node->typeToken = (TokenType)current->numToken;
//	node->lineNumber = currentLine;
//	node->nextlex = nullptr;
//	return node;
//}
//
////פונקצייה שמקבלת קוד, ומצביע לאוטומט
////TokenLex  הפונקציה עוברת על הקוד, מחלקת ללקסמות, בודקת תווים מיוחדים ומזמנת את הפונקציה 
////הפונקציה בונה רשימה של טוקנים ומחזירה אותה.
//Token* LexicalAnalysis::getListTokens(string code,buildAutomat* root_b)
//{
//	Token* listHead = nullptr;
//	Token* listTail = nullptr;
//	int currentLine = 1;
//	//רשימת מפרידים לבדוק אם לעשות בenum ומה עוד ברשימה
//	string List_of_separators =	" ()|&:,/+-*^={}\"'<>!=;\n";
//	int x = 0;
//	string lexema_from_code = "";
//	auto addNode = [&](Token* node) //פונקציית למדה הוספת טוקן לרשימת הטוקנים
//		{
//			if (node == nullptr) return;//צריך לבצע הוספה לרשימת השגיאות
//			node->nextlex = nullptr;
//			if (listHead == nullptr) //אם הרשימה ריקה
//				listHead = listTail = node;
//			else
//			{
//				listTail->nextlex = node;
//				listTail = node;
//			}
//		};
//	
//	while (x < code.length())
//	{
//			
//			//אם זה לא מתווי המפרידים
//			if (List_of_separators.find(code[x]) == string::npos)
//			{
//				lexema_from_code += code[x];
//				x++;
//			}
//			else
//			{	//אם זה מתווי המפרידים-שולחים את הלקסמה האחרונה לאוטומט-רק אם היא לא ריקה
//				if (!lexema_from_code.empty())
//				{
//					Token* l1 = TokenLex(lexema_from_code, root_b, currentLine);
//					addNode(l1);
//					lexema_from_code = "";
//				}
//
//
//				//טיפול במפריד
//				switch (code[x])
//				{
//				case '\n':
//				case ';':
//				{
//					
//						//  ניצור צומת ידנית
//					/*if (x - 1 > 0 && (code[x - 1] != ';' && code[x - 1] != '\n'))
//					{
//						Token* sep = TokenLex(";", root_b, currentLine);
//						currentLine++;
//						addNode(sep);
//					}*/
//				if(x-1>0 && (code[x-1] != ';'&& code[x - 1] != '\n'))
//				{
//					Token* sep = new Token();
//					sep->lex = ";"; //האם להפריד ל ; וירידת שורה או שאין ענין
//					//מספר טוקן זה רק למקבל?? או גם לירידת שורה שלא נמצא באוטומט
//					sep->nextlex = nullptr;
//					sep->lineNumber = currentLine;
//					sep->typeToken =Tok_newline;
//					currentLine++;
//					addNode(sep);
//				}
//						
//					break;
//				}
//				case ' ':
//					lexema_from_code = "";
//					cout << "One\n";
//					break;
//				case '\'':
//					lexema_from_code += code[x];
//					x++;
//					if (x < code.length() && code[x] != '\'') // התו הפנימי
//					{
//						lexema_from_code += code[x];
//						x++;
//					}
//					if (x < code.length() && code[x] == '\'') // ' סוגר
//						lexema_from_code += code[x];
//					//הוספה שגיאה מתאימה לרשימת השגיאות
//					break;
//
//
//				case '{':
//					lexema_from_code += code[x];
//					x++;
//					while (x < code.length() && code[x] != '}')
//					{
//						lexema_from_code += code[x];
//						x++;
//					}
//					if (x < code.length() && code[x] == '}') // אם מצאנו את הסוגר
//						lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
//					else
//						cout << "שגיאה לקסיקאלית: הערה לא סגורה\n";
//					break;
//					//הוספה שגיאה מתאימה לרשימת השגיאות
//
//
//				case '"':
//					lexema_from_code += code[x];
//					x++;
//					while (x < code.length() && code[x] != '"')
//					{
//						lexema_from_code += code[x];
//						x++;
//					}
//					if (x < code.length() && code[x] == '"') // אם מצאנו את הסוגר
//						lexema_from_code += code[x]; // הוספת הסוגר ללקסמה
//					else
//						cout << "שגיאה לקסיקאלית: מחרוזת לא סגורה\n";
//					//הוספה שגיאה מתאימה לרשימת השגיאות
//
//
//					break;
//				case '|':
//					if (x + 1 < code.length() && code[x + 1] == '|')
//					{
//						lexema_from_code += code[x];
//						x++;
//					}
//					lexema_from_code += code[x];
//
//					cout << "One\n";
//					break;
//				case '<':
//				case '>':
//				case '!':
//					if (x + 1 < code.length() && code[x + 1] == '=')
//					{
//						lexema_from_code += code[x];
//						x++;
//
//					}
//					else {
//						if (code[x] == '!')
//						{
//							//שגיאה אם זה לא !=
//							cout << "שגיאה לקסיקאלית: ! לבד אינו חוקי\n";
//							lexema_from_code = "";
//
//
//							//להוסיף לרשימת השגיאות לקסיקאליות
//							break;
//
//						}
//						if (code[x] == '<')
//						{
//							if (x + 1 < code.length() && code[x + 1] == '-')
//							{
//								lexema_from_code += code[x];  
//								x++;                           
//							 
//							}
//							
//						}
//
//						
//					
//					}
//
//					lexema_from_code += code[x];
//
//
//					cout << "One\n";
//					break;
//				case '=':
//
//					if (x + 1 < code.length() && (code[x + 1] == '<' || code[x + 1] == '>' || code[x + 1] == '!'))
//					{
//						lexema_from_code += code[x];
//						x++;
//					}
//					lexema_from_code += code[x];
//
//
//
//					cout << "One\n";
//					break;
//				
//				
//				default://כלומר אם  code[x] הוא מפריד אבל לא אחד מהמיוחדים שצריך לבדוק אם יש להם תו נוסף אחרי או לא- כלומר מפריד רגיל כמו ()&+-*^:,
//					//מה זה עושה ? בכל מקרה? או כמו else?
//					lexema_from_code = code[x];
//
//					//cout << "Unknown\n";
//					break;
//				}
//
//				if (x < code.length() && (code[x] == ' ' || code[x]=='!'))
//				{
//					x++;
//					continue;
//				}
//				if (!lexema_from_code.empty())
//				{
//					Token* l = TokenLex(lexema_from_code, root_b,currentLine);
//					addNode(l);
//				}
//				
//				
//				x++;
//				lexema_from_code = "";
//
//
//
//				//שורות מיותרות לכאורה
//				//שליחת הלקסמה לאוטומט
//				//Lexema* l = TokenLex(lexema_from_code);
//				//lexema_from_code = "";
//				//יצירת מצביע לרשימה חדשה המייצגת שורה חדשה
//				//הוספת מצביע זה לרשימה של מצביעים ללקס
//				//Lexema* l = this.tokenLex(lexema_from_code);//הוספת טוקן לרשימה
//
//
//
//			}
//		
//								
//	}
//	if (!lexema_from_code.empty())
//	{
//		Token* lexema_end = TokenLex(lexema_from_code,root_b,currentLine);
//
//		addNode(lexema_end);
//	}
//	return listHead;
//}
//
//void LexicalAnalysis::printListToken(Token* head)
//{
//	Token* current = head;
//
//	while (current != nullptr)
//	{
//		cout << "lexema: "
//			<< current->lex
//			<< " | token: "
//			<< TokenNames[ current->typeToken]
//			<< " | lineNumber: "
//			<< current->lineNumber
//			<< endl;
//
//		current = current->nextlex;
//	}
//}
//
//void LexicalAnalysis::deleteTokens(Token* head)
//{
//		Token* current = head;
//
//		while (current != nullptr)
//		{
//			Token* next = current->nextlex;
//			delete current;
//			current = next;
//		}
//	
//}
