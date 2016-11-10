#pragma once
#include <vector>
#include "Items.h"
#include "Ingredients.h"

class Player {
public:
	Player(int a_money);

	void gainMoney(int moneyGained);
	void loseMoney(int moneyLost);
	void learnItem(Items newItem){
		knownItems.push_back(newItem);
	}
	void learnIngredient(Ingredients newIngredient){
		knownIngredients.push_back(newIngredient);
	}
	void updateItemInventory(Items toUpdate);
	void updateIngredientInventory(Ingredients toUpdate);
	void attemptCrafting(vector<Ingredients> ingredientsToUse);

	int getMoney(){
		return money;
	}
	int locate(string thingToLocate);

	vector<Items> getItemInventory(){
		return knownItems;
	}
	vector<Ingredients> getIngredientInventory(){
		return knownIngredients;
	}
private:
	int money;

	vector<Items> knownItems;
	vector<Ingredients> knownIngredients;
};