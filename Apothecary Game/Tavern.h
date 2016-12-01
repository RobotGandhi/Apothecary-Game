#pragma once
#include <vector>
#include "Ingredients.h"
#include "Items.h"

class Tavern
{
public:
	Tavern();

	void obtainIngredient(Ingredients newIngredient) {
		ingredientsHeld.push_back(newIngredient);
	}
	void obtainItem(Items newItem) {
		itemsHeld.push_back(newItem);
	}
	void deleteIngredients(string toDelete);
	void deleteItems(string toDelete);

	vector<Ingredients> getIngredientsHeld(){
		return ingredientsHeld;
	}
	vector<Items> getItemsHeld() {
		return itemsHeld;
	}
private:
	vector<Ingredients> ingredientsHeld;
	vector<Items> itemsHeld;
};