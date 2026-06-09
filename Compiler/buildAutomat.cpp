#include "buildAutomat.h"




buildAutomat::buildAutomat()
{

	//אתחול מערך עזר
	// אתחול מזהה תווים: אותיות A-Z ו-a-z מקבלות 1, שאר ברירת מחדל 0
		// מערך עזר 
	for (int i = 0; i < Ascii; i++)
	{
		if ((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z'))
			arrIdentifier[i] = 1;
		else
			arrIdentifier[i] = 0;
	}

	// תוספת: תווים נוספים שמותרים להתחלה/לזיהוי
	for (int i = '0'; i <= '9'; i++) arrIdentifier[i] = 1;
	arrIdentifier['_'] = 1;
	arrIdentifier['#'] = 1;
	arrIdentifier['$'] = 1;
	arrIdentifier['@'] = 1;
	//0 - 9  @ $ # _ 



	//אתחול מערך מצבים

	for (int i = 0; i < NumOfState; i++)
	{
		this->arrState[i].numToken = -1;
		for (int j = 0; j < Ascii; j++)
		{
			this->arrState[i].hashState[j] = nullptr;
		}
	}
	ifstream file1("states.txt");
	if (!file1.is_open())
	{
		cerr << "Cannot open file states.txt\n"; return;//שגיאה!
	}

	


		
			
		

		
	int fromState, toState;
	string symbol;
	while (file1 >> fromState >> symbol >> toState)
	{
	
		if (fromState >= 0 && fromState < NumOfState &&
			toState >= 0 && toState < NumOfState)
		{
			if (symbol.empty())
			{
				cerr << "שגיאה: תו ריק בקובץ states.txt\n";
				continue;
			}

			char inputChar;

			if (symbol == "SPACE")
				inputChar = ' ';
			else
				inputChar = symbol[0];
				
			if((unsigned char)inputChar < Ascii)
				this->arrState[fromState].hashState[(unsigned char)inputChar] =
					&this->arrState[toState];
		}
		else
			cerr << "שגיאה: ערך מחוץ לגבולות בקובץ states.txt\n";
	/*
	std::cout << fromState ;
    std::cout << inputChar ;
    std::cout << toState << '\n';*/
	}	
		
	
	file1.close();

	ifstream file2("accept_states_token.txt");
	if (!file2.is_open())
	{
		cerr << "Cannot open file accept_states_token.txt\n"; return;
	}//הודעת שגיאה


	int state, token;
	while (file2 >> state >> token)
	{
		if (state >= 0 && state < NumOfState &&
			token >= 0 && token <= NumOfToken)
		{
			this->arrState[state].numToken = token;
		}
		else
			cerr << "שגיאה: ערך מחוץ לגבולות בקובץ accept_states_token.txt\n";

		/*std::cout << state ;
		std::cout << token << '\n';*/
	}
	file2.close();

	int arrToken[NumOfToken + 1] = { 0 };

	ifstream file3("compilerTxtData.txt");
	if (!file3.is_open())
	{
		cerr << "Cannot open file compilerTxtData.txt\n"; return;
	} //הודעת שגיאה


	
	int tok;

	while (file3 >> tok)
	{
		
		if (tok >= 0 && tok <= NumOfToken)
		{
			arrToken[tok] = 1;
		}
		else
			cerr << "שגיאה: ערך מחוץ לגבולות בקובץ compilerTxtData.txt\n";

	}
	file3.close();


	for (int i = 0; i < NumOfState; i++)
	{
		if (this->arrState[i].numToken >= 0 &&
			this->arrState[i].numToken <= NumOfToken &&
			arrToken[this->arrState[i].numToken] == 1)
			for (int j = 0; j < Ascii; j++)
			{
				if (this->arrState[i].hashState[j] == nullptr)
					this->arrState[i].hashState[j] = &this->arrState[1];

			}
	}


	//std::cout << "automat success!!!!!!!!!!!!!!!!!!!!!!!\n";

	

}
