#pragma once
#include <iostream>

using namespace std;

class Ingredients{
public:
	Ingredients(string name, int startingAmount);

	void setName(string ingredientName){
		name = ingredientName;
	}
	void setAmount(int newAmount){
		amountCarried = newAmount;
	}
	void gainAmount(int gained);
	void loseAmount(int lost);

	string getName(){
		return name;
	}
	int getAmount(){
		return amountCarried;
	}
private:
	string name;
	int amountCarried;
};