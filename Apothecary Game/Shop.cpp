#include "Shop.h"
#include <stdlib.h>

void Shop::stockItem(Items newItem){
	int position = locate(newItem.getName());
	if (position == 5000)
		itemsOnSale.push_back(newItem);
	else
		itemsOnSale.at(position).setAmount(newItem.getAmount());
}

//When time passes, the items you have stocked in your store gets closer and closer to being sold.
void Shop::addProgress() {
	int position;
	vector<int> itemsToDelete;
	for each(Items item in itemsOnSale) {
		item.gainProgress(rand()%3 + 9);
		if (item.getProgress() >= item.getThereshold()) {
			position = locate(item.getName());
			itemsToDelete.push_back(position);
			moneyToCollect += item.getPrice();
		}
	}
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