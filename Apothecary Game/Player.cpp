#include "Player.h"

void printError(const char errormessage[1000]);

Player::Player(int a_money){
	money = a_money;
}

void Player::gainMoney(int moneyGained){
	money += moneyGained;
}

void Player::loseMoney(int moneyLost){
	money -= moneyLost;
}

void Player::updateItemInventory(Items toUpdate){
	int position = locate(toUpdate.getName());
	if (position != 5000)
		knownItems.at(position) = toUpdate;
	else
		learnItem(toUpdate);
}

void Player::updateIngredientInventory(Ingredients toUpdate){
	int position = locate(toUpdate.getName());
	if (position != 5000)
		knownIngredients.at(position) = toUpdate;
	else
		learnIngredient(toUpdate);
}

void Player::addIngredientToInventory(string ingredientToAdd){
	int position = locate(ingredientToAdd);

	knownIngredients.at(position).gainAmount(1);
}

void Player::attemptCrafting(vector<Ingredients> ingredientsToUse){
	int position = 0;

	int rejuvHerb = 0;
	int energyHerb = 0;
	int pureWater = 0;

	for each (Ingredients ingredient in ingredientsToUse){
		if (ingredient.getName() == "Rejuvenation Herbs")
			rejuvHerb++;
		else if (ingredient.getName() == "Energy-Filled Herbs")
			energyHerb++;
		else if (ingredient.getName() == "Purified Water")
			pureWater++;
		else{
			string errorMessage = "An unknown ingredient was found: " + ingredient.getName();
			printError(errorMessage.c_str());
		}
	}
	if (rejuvHerb == 1 && energyHerb == 0 && pureWater == 2){
		position = locate("Lesser Healing Potion");
		if (position != 5000)
			updateItemInventory(Items("Lesser Healing Potion", knownItems.at(position).getAmount() + 1, 20, 10, true));
		else
			updateItemInventory(Items("Lesser Healing Potion", 1, 20, 10, true));
	}
	else if (rejuvHerb == 0 && energyHerb == 1 && pureWater == 2) {
		position = locate("Lesser Mana Potion");
		if (position != 5000)
			updateItemInventory(Items("Lesser Mana Potion", knownItems.at(position).getAmount() + 1, 20, 10, true));
		else
			updateItemInventory(Items("Lesser Mana Potion", 1, 20, 10, true));
	}
}

int Player::locate(string thingToLocate){
	int position = 0;
	for each (Ingredients ingredient in knownIngredients){
		if (ingredient.getName() == thingToLocate)
			return position;
		position++;
	}
	position = 0;
	for each (Items item in knownItems){
		if (item.getName() == thingToLocate)
			return position;
		position++;
	}
	//If nothing is found, position returns a set number, serving as an indicator to add the item/ingredient or throw an error message.
	position = 5000;
	return position;
}