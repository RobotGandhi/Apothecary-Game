#pragma once
#include <vector>
#include "Areas.h"
#include "Adventurers.h"
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
	void recruitAdventurer(Adventurers newAdventurer) {
		recruitedAdventurers.push_back(newAdventurer);
	}
	void unlockArea(Areas newArea) {
		unlockedAreas.push_back(newArea);
	}
	void updateItemInventory(Items toUpdate);
	void updateIngredientInventory(Ingredients toUpdate);
	void addIngredientToInventory(string ingredientToAdd);
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
	vector<Adventurers> getRecruitedAdventurers() {
		return recruitedAdventurers;
	}
	vector<Areas> getUnlockedAreas() {
		return unlockedAreas;
	}
private:
	int money;

	vector<Items> knownItems;
	vector<Ingredients> knownIngredients;
	vector<Adventurers> recruitedAdventurers;
	vector<Areas> unlockedAreas;
};