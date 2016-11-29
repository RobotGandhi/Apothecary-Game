#include "Adventurers.h"

Adventurers::Adventurers(int a_level, int a_defense, int a_attack, int a_magicAttack, int a_magicDefense, string a_name, string a_job){
	level = a_level;
	attack = a_attack;
	defense = a_defense;
	magicAttack = a_magicAttack;
	magicDefense = a_magicDefense;
	name = a_name;
	job = a_job;
}

Tavern Adventurers::addProgress(Tavern tavern){
	cout << progress;
	gainProgress(rand() % 2 + 3);
	if (progress >= tolerance) {
		int avaliableItems = assignedArea.getPossibleIngredients().size() + getAssignedArea().getPossibleItems().size();
		for (int i = 0; i < maxCarry; i++) {
			int itemToRoll = rand() % avaliableItems + 1;
			if (itemToRoll > assignedArea.getPossibleIngredients().size()) {
				cout << "Rolled for an item.";
				itemToRoll -= assignedArea.getPossibleIngredients().size();
				if ((rand() % 5) > 3) {
					tavern.obtainItem(assignedArea.getPossibleItems().at(itemToRoll - 1));
					cout << "Item added.";
				}
			}
			else {
				cout << "Rolled for an ingredient.";
				if ((rand() % 5) > 2) {
					tavern.obtainIngredient(assignedArea.getPossibleIngredients().at(itemToRoll - 1));
					cout << "Ingredient added.";
				}
			}
		}
		assignArea(Areas("DEFAULT", 0));
	}
	return tavern;
}