#pragma once
#include <iostream>
#include "Areas.h"
#include "Tavern.h"

using namespace std;

class Adventurers{
public:
	Adventurers(int a_level, int a_defense, int a_attack, int a_magicAttack, int a_magicDefense, string a_name, string a_job);

	void assignArea(Areas newArea) {
		assignedArea = newArea;
	}
	void gainProgress(int gained) {
		progress += gained;
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
	int getProgress() {
		return progress;
	}
	int getTolerance() {
		return tolerance;
	}

	string getName() {
		return name;
	}

	Areas getAssignedArea() {
		return assignedArea;
	}

	Tavern addProgress(Tavern tavern);

private:
	bool isQuesting = false;

	int level;
	int attack;
	int defense;
	int magicAttack;
	int magicDefense;
	int maxCarry = 5;
	int tolerance = 10; //Change later, milestone id #14
	int progress;

	string name;
	string job;

	Areas assignedArea = Areas("DEFAULT", 0);
};