#include "Tavern.h"

Tavern::Tavern()
{
}

void Tavern::deleteIngredients(string toDelete) {
	vector<int> ingredientsToDelete;
	bool sorting = true;
	for (int position = 0; position < ingredientsHeld.size(); position++) {
		if (ingredientsHeld.at(position).getName() == toDelete)
			ingredientsToDelete.push_back(position);
	}

	//Sorts the vector in decenfing order to avoid errors.
	if (ingredientsToDelete.size() < 2)
		sorting = false;
	if (sorting == true) {
		bool swapped = true;
		for (int i = 1; (i <= ingredientsToDelete.size()) && swapped; i++) {
			swapped = false;
			for (int j = 0; j < (ingredientsToDelete.size() - 1); j++) {
				if (ingredientsToDelete[j + 1] > ingredientsToDelete[j]) {
					int temp = ingredientsToDelete[j];
					ingredientsToDelete[j] = ingredientsToDelete[j + 1];
					ingredientsToDelete[j + 1] = temp;
					swapped = true;
				}
			}
		}
	}

	//Removes the obtained items from the tavern's inventory.
	for each (int ingredientPosition in ingredientsToDelete) {
		ingredientsHeld.erase(ingredientsHeld.begin() + ingredientPosition);
	}
}

void Tavern::deleteItems(string toDelete) {
	vector<int> itemsToDelete;
	bool sorting = true;
	for (int position = 0; position < itemsHeld.size(); position++) {
		itemsToDelete.push_back(position);
	}

	if (itemsToDelete.size() < 2)
		sorting = false;
	if (sorting == true) {
		bool swapped = true;
		for (int i = 1; (i <= itemsToDelete.size()) && swapped; i++) {
			swapped = false;
			for (int j = 0; j < (itemsToDelete.size() - 1); j++) {
				if (itemsToDelete[j + 1] > itemsToDelete[j]) {
					int temp = itemsToDelete[j];
					itemsToDelete[j] = itemsToDelete[j + 1];
					itemsToDelete[j + 1] = temp;
					swapped = true;
				}
			}
		}
	}
	//Removes the sold items from the shop's inventory.
	for each (int itemPosition in itemsToDelete) {
		itemsHeld.erase(itemsHeld.begin() + itemPosition);
	}
}