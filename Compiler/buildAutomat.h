#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define Ascii 128
#define NumOfState 57
#define NumOfToken 23
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

//קריאה מקובץ

//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//
//void buildAutomat::readTransitionsFromFile(std::string fileName) {
//    std::ifstream file(fileName); // פתיחת הקובץ
//    std::string line;
//
//    if (!file.is_open()) {
//        std::cerr << "Error: Could not open file " << fileName << std::endl;
//        return;
//    }
//
//    // קריאת הקובץ שורה אחר שורה
//    while (std::getline(file, line)) {
//        if (line.empty()) continue; // דילוג על שורות ריקות
//
//        std::stringstream ss(line);//שומר מחרוזת
//        std::string parts[3]; // המערך בגודל 3 
//        int i = 0;
//
//        // פירוק השורה למילים לפי רווחים
//        while (ss >> parts[i] && i < 3) {
//            i++;
//        }
// 
//       
//      string line= "1 t 3"
//      int[] arr= new int[3];
//      arr[0]=int .parse(line[0]);
//      arr[2]=int .parse(line[4]);
//      arr[1]=line[2];
//        // כאן יש לך את הנתונים במערך parts
//        // parts[0] - המצב הנוכחי
//        // parts[1] - התו (הקלט)
//        // parts[2] - המצב הבא
//
//        // דוגמה לשימוש בנתונים (המרת מחרוזת למספר במידת הצורך):
//        // int fromState = std::stoi(parts[0]);
//        // char inputChar = parts[1][0];
//        // int toState = std::stoi(parts[2]);






