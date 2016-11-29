#include "Shop.h"
#include <stdlib.h>

void Shop::stockItem(Items newItem){
	int position = locate(newItem.getName());
	if (position == 5000)
		itemsOnSale.push_back(newItem);
	else
		itemsOnSale.at(position).setAmount(itemsOnSale.at(position).getAmount() + newItem.getAmount());
}

//When time passes, the items you have stocked in your store gets closer and closer to being sold.
void Shop::addProgress() {
	bool sorting = true;
	int position;
	vector<int> itemsToDelete;
	//Add progress to whatever's being sold, then compare it to the given item's thereshold.
	for each(Items item in itemsOnSale) {
		int gained = rand() % 4 + 8;
		item.gainProgress(gained);
		if (item.getProgress() >= item.getThereshold()) {
			itemsOnSale.at(locate(item.getName())).loseAmount(1);
			moneyToCollect += item.getPrice();
			if (itemsOnSale.at(locate(item.getName())).getAmount() <= 0) {
				position = locate(item.getName());
				itemsToDelete.push_back(position);
			}
		}
	}
	//Sorts the itemsToDelete vector in decending order to make sure the wrong items doesn't get sold.
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
		itemsOnSale.erase(itemsOnSale.begin() + itemPosition);
	}
}

int Shop::locate(string thingToLocate){
	int position = 0;
	for each (Items item in itemsOnSale) {
		if (item.getName() == thingToLocate)
			return position;
		position++;
	}
	//If nothing is found, position returns a set number, serving as an indicator to add the item/ingredient or throw an error message.
	position = 5000;
	return position;
}