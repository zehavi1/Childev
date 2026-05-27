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
		//קריאה מקובץ
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
	//הודעת שגיאה	
	string len1;
	while (getline(file1, len1, '\r'))// /n בקובץ tu word שיעבור עד /r ולא עד /n

	{
		
		//string line = "1 A 3"
		//השורה שנקראה מהקובץ;
		stringstream ss(len1);
		string word; int arrTxtState[3]; int i = 0;
		
		
		
			while (i < 3 && ss >> word)
			{
			std::cout << "WORD = [" << word << "]\n";
			if (i == 0 || i == 2)
				arrTxtState[i] = stoi(word);
			else

				arrTxtState[i] = word[0];
			i++;
			
			}
			// non-owning pointer: מצביע אל מצב קיים ב-arrState
			this->arrState[arrTxtState[0]].hashState[arrTxtState[1]] = &this->arrState[arrTxtState[2]];
			//*(this->arrState[arrTxtState[0]].hashState)[arrTxtState[1]] = (this->arrState[arrTxtState[2]]);
			std::cout << &this->arrState[arrTxtState[2]] << '\n';
			std::cout << this->arrState[arrTxtState[0]].hashState[arrTxtState[1]] << '\n';
			std::cout << "h333 file\n";
		
		
		

		
		//צריך לבדוק איך מגדירים על מקומות 0 ו 2 במערך איך עושים שייצגו את הערך המספרי של המספר ולא אסקי אף שמדובר במספר דו ספרתי
	}
	file1.close();

	std::cout << "file 1 end file\n";

	ifstream file2("accept_states_token.txt");
	if (!file2.is_open())
	{
		cerr << "Cannot open file\n"; return;
	}//הודעת שגיאה


	//הודעת שגיאה	
	string len2;
	std::cout << "start read file2\n";

	while (getline(file2, len2,'\n'))

	{
		stringstream ss(len2);
		string word; int arrTxtaccept_states_token[2]; int i = 0;
		while (i<2 && ss >> word)
		{

			arrTxtaccept_states_token[i] = stoi(word);
			i++;

		}
		

		this->arrState[arrTxtaccept_states_token[0]].numToken = arrTxtaccept_states_token[1];
		std::cout << arrTxtaccept_states_token[0] << '\n';
		std::cout << arrTxtaccept_states_token[1] << '\n';

	}

	file2.close();
	std::cout << "file 2 end file\n";

	int arrToken[NumOfToken + 1] = { 0 };

	ifstream file3("compilerTxtData.txt");
	if (!file3.is_open())
	{
		cerr << "Cannot open file\n"; return;
	} //הודעת שגיאה

	string len3;
	std::cout << "start read file2\n";

	while (getline(file3, len3, '\n'))
	{
		arrToken[stoi(len3)] = 1;
		std::cout<< arrToken[stoi(len3)] << '\n';

	}

	file3.close();

	std::cout << "file 3 end file\n";

	for (int i = 0; i < NumOfState; i++)
	{
		if (arrToken[this->arrState[i].numToken] == 1)
			for (int j = 0; j < Ascii; j++)
			{
				if (this->arrState[i].hashState[j] == NULL)
					this->arrState[i].hashState[j] = &this->arrState[1];

			}
	}
	std::cout << "file 457 end file\n";


	std::cout << "automat success!!!!!!!!!!!!!!!!!!!!!!!\n";

	//אתחול מצב 0



	//קריאה מקובץ מצבים וכו



	// בכל תא מגדירים מערך מסוג מצב בגודל כל התווים באסקי טבלת אש וכן מגדירים בתא מס טוקן במקרה של -1 הוא לא מקבל

	// ממצב 0 כל התאים באש שהם אותיות מצביעים למצב 1

	// מתחילים לקרו מהקובץ. אם קראתי תו ממצב 0 שהיה מצביע למצב 1 יש לדרוס אותו ולהפנות לפי הקובץ ובמצב החדש אליו מצביע להגדיר שמקבל טוקן של מצב 1

	// כל מצב שייפתח חדש יש לאתחל את כל האש להצביע למצב 1 רק מי שמתאים לשם משתנה!


}
