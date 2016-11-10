#pragma once
#include <iostream>

using namespace std;

class Items {
public:
	Items(string a_name, int a_startingAmount, int a_salePrice, int a_thereshold, bool a_recipeKnown);

	void setName(string newItemName){
		itemName = newItemName;
	}
	void setAmount(int newAmount){
		amountCarried = newAmount;
	}
	void setPrice(int newPrice) {
		chosenSalePrice = newPrice;
	}
	void learnRecipe() {
		recipeKnown = true;
	}
	void forgetRecipe(){
		recipeKnown = false;
	}
	void gainAmount(int gained);
	void loseAmount(int lost);
	void gainProgress(int gained) {
		saleProgress += gained;
	}

	bool isRecipeKnown() {
		return recipeKnown;
	}

	string getName(){
		return itemName;
	}

	int getAmount(){
		return amountCarried;
	}
	int getPrice() {
		return chosenSalePrice;
	}
	int getThereshold() {
		return saleThereshold;
	}
	int getProgress() {
		return saleProgress;
	}
private:
	bool recipeKnown;

	string itemName;

	int amountCarried;
	int initialSalePrice;
	int chosenSalePrice;
	int saleThereshold;
	int saleProgress;
};