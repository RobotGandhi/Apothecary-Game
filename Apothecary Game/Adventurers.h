#pragma once
#include <iostream>
#include "Areas.h"

using namespace std;

class Adventurers{
public:
	Adventurers(int a_level, int a_defense, int a_attack, int a_magicAttack, int a_magicDefense, string a_name, string a_job);

	void assignArea(Areas newArea) {
		assignedArea = newArea;
	}

	int getLevel() {
		return level;
	}
	int getAttack() {
		return attack;
	}
	int getDefense() {
		return defense;
	}
	int getMagicAttack() {
		return magicAttack;
	}
	int getMagicDefense() {
		return magicDefense;
	}

	string getName() {
		return name;
	}

private:
	bool isQuesting = false;

	int level;
	int attack;
	int defense;
	int magicAttack;
	int magicDefense;

	float tolerance;

	string name;
	string job;

	Areas assignedArea = Areas("DEFAULT", 0);
};