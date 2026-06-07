#include "buildAutomat.h"




buildAutomat::buildAutomat()
{

	std::cout << "h333 file\n";

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
			this->arrState[i].hashState[j] = NULL;
		}
	}
	std::cout << "hhhh file\n";
	ifstream file1("states.txt");
	if (!file1.is_open())
	{
		cerr << "Cannot open file\n"; return;
	}


	
	int fromState, toState;
	char inputChar;
	while (file1 >> fromState >> inputChar >> toState)
	{
		if (fromState >= 0 && fromState < NumOfState &&
			toState >= 0 && toState < NumOfState &&
			(unsigned char)inputChar < Ascii)
		{
			this->arrState[fromState].hashState[(unsigned char)inputChar] =
				&this->arrState[toState];
		}
		else
			cerr << "שגיאה: ערך מחוץ לגבולות בקובץ states.txt\n";
	
	std::cout << fromState << '\n';
    std::cout << inputChar << '\n';
    std::cout << toState << '\n';
	std::cout << "h333 file\n";
	}	
		
	
	file1.close();

	std::cout << "file 1 end file\n";

	ifstream file2("accept_states_token.txt");
	if (!file2.is_open())
	{
		cerr << "Cannot open file\n"; return;
	}//הודעת שגיאה

	std::cout << "start read file2\n";

	
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

		std::cout << state << '\n';
		std::cout << token << '\n';
	}
	file2.close();
	std::cout << "file 2 end file\n";

	int arrToken[NumOfToken + 1] = { 0 };

	ifstream file3("compilerTxtData.txt");
	if (!file3.is_open())
	{
		cerr << "Cannot open file\n"; return;
	} //הודעת שגיאה

	std::cout << "start read file2\n";

	
	int tok;
	while (file3 >> tok)
	{
		if (tok >= 0 && tok <= NumOfToken)
		{
			arrToken[tok] = 1;
		}
		else
			cerr << "שגיאה: ערך מחוץ לגבולות בקובץ compilerTxtData.txt\n";

		std::cout << arrToken[tok] << '\n';
	}
	file3.close();

	std::cout << "file 3 end file\n";

	for (int i = 0; i < NumOfState; i++)
	{
		if (this->arrState[i].numToken >= 0 &&
			this->arrState[i].numToken <= NumOfToken &&
			arrToken[this->arrState[i].numToken] == 1)
			for (int j = 0; j < Ascii; j++)
			{
				if (this->arrState[i].hashState[j] == NULL)
					this->arrState[i].hashState[j] = &this->arrState[1];

			}
	}
	std::cout << "file 457 end file\n";


	std::cout << "automat success!!!!!!!!!!!!!!!!!!!!!!!\n";

	

}
