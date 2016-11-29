#pragma once
#include <vector>
#include "Ingredients.h"
#include "Items.h"

using namespace std;

class Areas
{
public:
	Areas(string a_name, float a_baseTolerance);

	void setName(string newName) {
		name = newName;
	}
	void addPossibleIngredient(Ingredients newIngredient) {
		ingredientsToFind.push_back(newIngredient);
	}
	void addPossibleItem(Items newItem) {
		itemsToFind.push_back(newItem);
	}

	string getName() {
		return name;
	}

	vector<Ingredients> getPossibleIngredients() {
		return ingredientsToFind;
	}
	vector<Items> getPossibleItems() {
		return itemsToFind;
	}
private:
	float baseTolerance;
	string name;

	vector<Ingredients> ingredientsToFind;
	vector<Items> itemsToFind;
};