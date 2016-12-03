#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

void printError(const char errorMessage[1000]){
	//Attempts to open the error log.
	ofstream errorFile;
	errorFile.open("errorLog.txt", ios::app | ios::out);

	//If the error log couldn't open, print an error into the console instead.
	if (!errorFile.is_open())
		cout << "The error file could not be opened." << endl;
	else{
		errorFile << errorMessage << endl;
		errorFile.close();
	}
}