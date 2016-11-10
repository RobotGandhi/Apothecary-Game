#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

string currentDateTime();

void printError(const char errorMessage[1000]){
	//Attempts to open the error log.
	ofstream errorFile;
	errorFile.open("errorLog.txt", ios::app | ios::out);

	//If the error log couldn't open, print an error into the console instead.
	if (!errorFile.is_open())
		cout << "The error file could not be opened." << endl;
	else{
		errorFile << currentDateTime() << ": " << errorMessage << endl;
		errorFile.close();
	}
}

string currentDateTime() {
	//Used to display the current date and time.
	time_t t = time(0);

	struct tm * now = NULL;
	localtime_s(now, &t);
	string dateTime = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);
	return dateTime;
}