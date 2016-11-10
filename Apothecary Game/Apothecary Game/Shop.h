#pragma once
#include <vector>
#include "Items.h"

class Shop{
public:
	void stockItem(Items newItem);
	void addProgress();
	void resetMoney() {
		moneyToCollect = 0;
	}

	int locate(string thingToLocate);
	int getMoney() {
		return moneyToCollect;
	}

	vector<Items> getShopItems() {
		return itemsOnSale;
	}
private:
	vector<Items> itemsOnSale;
	int moneyToCollect;
};