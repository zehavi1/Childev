#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define Ascii 128
#define NumOfState 56//
#define NumOfToken 29//
using namespace std;

struct State //מבנה מצב
{
	int numToken; //מספר טוקן- במקרה של מצב שלא מקבל שדה זה יכיל -1
	State* hashState[Ascii]; //מצביע למערך של מצביעים
};

class buildAutomat
{
public:
	

	// מערך המצבים
	State arrState[NumOfState];

	//מערך עזר המכיל את הערכים באסקי האפשריים ל id
	int arrIdentifier[Ascii];

	buildAutomat();
	

	
};




