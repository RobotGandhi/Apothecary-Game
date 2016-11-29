#include "Items.h"

Items::Items(string a_name, int a_startingAmount, int a_salePrice, int a_thereshold, bool a_recipeKnown){
	itemName = a_name;
	amountCarried = a_startingAmount;
	initialSalePrice = a_salePrice;
	chosenSalePrice = a_salePrice;
	saleThereshold = a_thereshold;
	saleProgress = 0;
	recipeKnown = a_recipeKnown;
}

void Items::gainAmount(int gained){
	int temp = getAmount() + gained;
	if (temp > 99)
		temp = 99;
	setAmount(temp);
}

void Items::loseAmount(int lost){
	int temp = getAmount() - lost;
	if (temp < 0)
		temp = 0;
	setAmount(temp);
}