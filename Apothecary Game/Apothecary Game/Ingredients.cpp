#include <iostream>
#include "Ingredients.h"

using namespace std;

Ingredients::Ingredients(string name, int startingAmount){
	setName(name);
	setAmount(startingAmount);
}

void Ingredients::gainAmount(int gained){
	int temp = getAmount() + gained;
	if (temp > 99)
		temp = 99;
	setAmount(temp);
}

void Ingredients::loseAmount(int lost){
	int temp = getAmount() - lost;
	if (temp < 0)
		temp = 0;
	setAmount(temp);
}