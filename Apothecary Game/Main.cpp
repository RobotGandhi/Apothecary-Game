#include <time.h>
#include <SFML\Graphics.hpp>
#include <SFML\Main.hpp>
#include <SFML\Window.hpp>
#include "Adventurers.h"
#include "Areas.h"
#include "Ingredients.h"
#include "InteractableButton.h"
#include "Items.h"
#include "Player.h"
#include "Shop.h"

using namespace std;
using namespace sf;

void printError(const char errorMessage[1000]);

string generateIngredientList(Player player);
string generateItemList(Player player);
string generateSaleList(Shop shop);
string generateAdventurerList(Player player, Areas area);
string generateAreaList(Player player);

int main(){
	//Create the seed for the RNG.
	srand(time(NULL));

	//Create settings for the game.
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Get the size of the screen and creates a window. Also limits the application to 60 fps.
	VideoMode desktopSize = VideoMode::getDesktopMode();
	RenderWindow mainWindow(VideoMode(1280, 720, desktopSize.bitsPerPixel), "Apothecary Game", Style::Titlebar | Style::Close, settings);
	mainWindow.setFramerateLimit(60);
	
	//Loads the font used for text.
	Font fontAntiqua;
	if (!fontAntiqua.loadFromFile("BKANT.ttf")){
		printError("Book Antiqua (BKANT.ttf) could not be loaded.");
	}

	//Defines the color used when hovering over buttons.
	Color buttonShade(101, 94, 0);
	Color buttonDefault(178, 164, 0);
	Color textBrighten(216, 120, 31);
	Color textDefault(63, 31, 1);

	Texture cursorTexture;
	if (!cursorTexture.loadFromFile("Blue_Arrow_001.png"))
		printError("File Blue_Arrow_001.png could not be loaded.");
	Vector2u textureSize = cursorTexture.getSize();
	RectangleShape cursor(Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y)));
	cursor.setTexture(&cursorTexture);

	Texture regularButtonTexture;
	if (!regularButtonTexture.loadFromFile("Wooden_Button(200x100).png"))
		printError("File Wooden_Button(200x100).png could not be loaded.");
	Texture slimButtonTexture;
	if (!slimButtonTexture.loadFromFile("Wooden_Button(400x50).png"))
		printError("File Wooden_Button(400x50).png could not be loaded.");

	//Create shapes that are currently used for buttons.
	InteractableButton enterShop(200, 100, 1035, 575, "Enter your Shop", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton enterTavern(200, 100, 540, 575, "Enter the Tavern", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton startCrafting(200, 100, 45, 575, "Start Crafting", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton confirmCrafting(200, 100, 1035, 575, "Craft!", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton resetCrafting(200, 100, 540, 575, "Return your items", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton returnToMenu(200, 100, 45, 575, "Back to Menu", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton experiment(400, 50, 440, 225, "Start experimenting", fontAntiqua, textDefault, buttonDefault, slimButtonTexture);
	InteractableButton craftFromRecipe(400, 50, 440, 375, "Craft from a recipe", fontAntiqua, textDefault, buttonDefault, slimButtonTexture);
	InteractableButton startSale(200, 100, 1035, 575, "Sell an item", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton collectMoney(200, 100, 540, 575, "Collect", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton sendAdventurers(400, 50, 440, 150, "Send out an adventurer", fontAntiqua, textDefault, buttonDefault, slimButtonTexture);
	InteractableButton spendTime(400, 50, 440, 300, "Spend time in the tavern", fontAntiqua, textDefault, buttonDefault, slimButtonTexture);
	InteractableButton confirm(200, 100, 1035, 575, "Confirm", fontAntiqua, textDefault, buttonDefault, regularButtonTexture);
	InteractableButton collectItems(400, 50, 440, 450, "Collect items", fontAntiqua, textDefault, buttonDefault, slimButtonTexture);

	//Creates text instances. If the text is used in multiple levels, the position is set when the level changes.
	Text ingredientList;
	ingredientList.setFont(fontAntiqua);
	ingredientList.setCharacterSize(25);
	ingredientList.setFillColor(textBrighten);

	Text itemList;
	itemList.setFont(fontAntiqua);
	itemList.setCharacterSize(25);
	itemList.setFillColor(textBrighten);

	Text adventurerList;
	adventurerList.setFont(fontAntiqua);
	adventurerList.setCharacterSize(25);
	adventurerList.setFillColor(textBrighten);
	adventurerList.setPosition(45, 45);

	Text shopItems;
	shopItems.setFont(fontAntiqua);
	shopItems.setCharacterSize(25);
	shopItems.setFillColor(textBrighten);
	shopItems.setPosition(45, 45);
	
	Text areaList;
	areaList.setFont(fontAntiqua);
	areaList.setCharacterSize(25);
	areaList.setFillColor(textBrighten);
	areaList.setPosition(45, 45);

	Text displayMoney;
	displayMoney.setFont(fontAntiqua);
	displayMoney.setCharacterSize(25);
	displayMoney.setFillColor(textBrighten);
	displayMoney.setPosition(1000, 10);

	Text displayShopMoney;
	displayShopMoney.setFont(fontAntiqua);
	displayShopMoney.setCharacterSize(25);
	displayShopMoney.setFillColor(textBrighten);
	displayShopMoney.setPosition(45, 10);

	Text levelIndicator;
	levelIndicator.setFont(fontAntiqua);
	levelIndicator.setCharacterSize(25);
	levelIndicator.setFillColor(textBrighten);
	levelIndicator.setPosition(640, 25);

	//A variable used for checking which screen the player is on.
	int level = 0;
	//A variable for checking what the player is highlighting.
	int newCursorPosition = 0;
	int oldCursorPosition = 0;

	//A variable of ingredients used in the current crafting attempt.
	vector<Ingredients> currentCraftingAttempt;

	//A bool checking whetever the "Level not loaded" error has already been printed, in order to keep the error log from flooding with the same error.
	bool levelErrorPrinted = false;
	//A bool checking if an object needs to be updated.
	bool recheckTavern = false;
	bool updateArea = false;
	bool updateIngredient = false;
	bool updateItem = false;

	//Initialises items and ingredients and gives the player some starting ingredients.
	Player player(100);

	Ingredients tempIngredient("DEFAULT", 0);
	Ingredients rejuvHerb("Rejuvenation Herbs", 5);
	Ingredients energyHerb("Energy-Filled Herbs", 5);
	Ingredients pureWater("Purified Water", 5);

	Items tempItem("DEFAULT", 0, 0, 0, false);
	Items lessHealPotion("Lesser Healing Potion", 0, 20, 10, false);
	Items lessManaPotion("Lesser Mana Potion", 0, 20, 10, false);

	Adventurers temp(0, 0, 0, 0, 0, "DEFAULT", "TEMP");
	Adventurers mage(1, 10, 10, 50, 50, "Place Holder", "Mage");
	Adventurers knight(1, 50, 50, 10, 10, "White Knight", "Knight");

	Areas tempArea("DEFAULT", 0);
	Areas energySprings("Energy Springs", 1);

	Shop playerShop;

	Tavern tavern;

	energySprings.addPossibleIngredient(rejuvHerb);
	energySprings.addPossibleIngredient(energyHerb);
	energySprings.addPossibleIngredient(pureWater);

	energySprings.addPossibleItem(lessManaPotion);
	energySprings.addPossibleItem(lessHealPotion);

	player.learnIngredient(rejuvHerb);
	player.learnIngredient(energyHerb);
	player.learnIngredient(pureWater);

	player.learnItem(lessHealPotion);

	player.recruitAdventurer(mage);
	player.recruitAdventurer(knight);

	player.unlockArea(energySprings);

	//Keeps the program open while the window is open.
	while (mainWindow.isOpen()){
		Event event;
		while (mainWindow.pollEvent(event)){
			//Checks if the window has recieved an event, and it the event wants to close the window, the window closes.
			if (event.type == Event::Closed)
				mainWindow.close();
			//Checks if the mouse is hovering over any buttons and highlights that button.
			if (event.type == Event::MouseMoved){
				if (level == 0){
					if (startCrafting.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						startCrafting.changeButtonColor(buttonShade);
						startCrafting.changeTextColor(textBrighten);
					}
					else if (enterTavern.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						enterTavern.changeButtonColor(buttonShade);
						enterTavern.changeTextColor(textBrighten);
					}
					else if (enterShop.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						enterShop.changeButtonColor(buttonShade);
						enterShop.changeTextColor(textBrighten);
					}
					else{
						enterShop.changeButtonColor(buttonDefault);
						enterShop.changeTextColor(textDefault);
						enterTavern.changeButtonColor(buttonDefault);
						enterTavern.changeTextColor(textDefault);
						startCrafting.changeButtonColor(buttonDefault);
						startCrafting.changeTextColor(textDefault);
					}
				}
				else if (level == 1){
					if (experiment.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						experiment.changeButtonColor(buttonShade);
						experiment.changeTextColor(textBrighten);
					}
					else if (craftFromRecipe.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						craftFromRecipe.changeButtonColor(buttonShade);
						craftFromRecipe.changeTextColor(textBrighten);
					}
					else if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else {
						experiment.changeButtonColor(buttonDefault);
						experiment.changeTextColor(textDefault);
						craftFromRecipe.changeButtonColor(buttonDefault);
						craftFromRecipe.changeTextColor(textDefault);
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
					}
				}
				else if (level == 2){
					if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else if (resetCrafting.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						resetCrafting.changeButtonColor(buttonShade);
						resetCrafting.changeTextColor(textBrighten);
					}
					else if ((confirmCrafting.isOverlapping(event.mouseMove.x, event.mouseMove.y))) {
						confirmCrafting.changeButtonColor(buttonShade);
						confirmCrafting.changeTextColor(textBrighten);
					}
					else {
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
						resetCrafting.changeButtonColor(buttonDefault);
						resetCrafting.changeTextColor(textDefault);
						confirmCrafting.changeButtonColor(buttonDefault);
						confirmCrafting.changeTextColor(textDefault);
					}
				}
				else if (level == 3) {
					if (startSale.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						startSale.changeButtonColor(buttonShade);
						startSale.changeTextColor(textBrighten);
					}
					else if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else if (collectMoney.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						collectMoney.changeButtonColor(buttonShade);
						collectMoney.changeTextColor(textBrighten);
					}
					else {
						startSale.changeButtonColor(buttonDefault);
						startSale.changeTextColor(textDefault);
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
						collectMoney.changeButtonColor(buttonDefault);
						collectMoney.changeTextColor(textDefault);
					}
				}
				else if (level == 4) {
					if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else {
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
					}
				}
				else if (level == 5) {
					if (sendAdventurers.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						sendAdventurers.changeButtonColor(buttonShade);
						sendAdventurers.changeTextColor(textBrighten);
					}
					else if (spendTime.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						spendTime.changeButtonColor(buttonShade);
						spendTime.changeTextColor(textBrighten);
					}
					else if (collectItems.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						collectItems.changeButtonColor(buttonShade);
						collectItems.changeTextColor(textBrighten);
					}
					else if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else {
						sendAdventurers.changeButtonColor(buttonDefault);
						sendAdventurers.changeTextColor(textDefault);
						spendTime.changeButtonColor(buttonDefault);
						spendTime.changeTextColor(textDefault);
						collectItems.changeButtonColor(buttonDefault);
						collectItems.changeTextColor(textDefault);
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
					}
				}
				else if (level == 6) {
					if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else if (confirm.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						confirm.changeButtonColor(buttonShade);
						confirm.changeTextColor(textBrighten);
					}
					else {
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
						confirm.changeButtonColor(buttonDefault);
						confirm.changeTextColor(textDefault);
					}
				}
				else if (level == 7) {
					if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else if (confirm.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						confirm.changeButtonColor(buttonShade);
						confirm.changeTextColor(textDefault);
					}
					else {
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
						confirm.changeButtonColor(buttonDefault);
						confirm.changeTextColor(textDefault);
					}
				}
				else if (level == 8) {
					if (returnToMenu.isOverlapping(event.mouseMove.x, event.mouseMove.y)) {
						returnToMenu.changeButtonColor(buttonShade);
						returnToMenu.changeTextColor(textBrighten);
					}
					else {
						returnToMenu.changeButtonColor(buttonDefault);
						returnToMenu.changeTextColor(textDefault);
					}
				}
			}
			//Checks if the mouse is over a clickable object if the left mouse button is pressed.
			if ((event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) || recheckTavern == true) {
				if (level == 0) {
					//Takes the player to the Crafting Method Selector.
					if (startCrafting.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 1;
						levelIndicator.setString("Crafting Method Selector");
					}
					//Takes the player to the tavern.
					else if (enterTavern.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 5;
						levelIndicator.setString("The Tavern");
						if (tavern.getIngredientsHeld().size() + tavern.getItemsHeld().size() > 0)
							collectItems.changeString("Collect items");
						else
							collectItems.changeString("No items to collect");
					}
					//Takes the player to their shop.
					else if (enterShop.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 3;
						levelIndicator.setString("Your Shop");
						displayShopMoney.setString("The shop currently carries " + to_string(playerShop.getMoney()) + " currency.");
					}
				}
				else if (level == 1) {
					//Takes the player to the experimentation area and sets the lists up.
					if (experiment.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 2;
						levelIndicator.setString("Experimentation");
						ingredientList.setPosition(45, 15);
						itemList.setPosition(500, 75);
						cursor.setPosition(15, 77.5);
					}
					//Takes the player to the recipe crafting area and sets the lists up.
					else if (craftFromRecipe.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 8;
						levelIndicator.setString("Crafting from recipe");
						itemList.setPosition(45, 75);
						cursor.setPosition(15, 77.5);
					}
					//Takes the player to the main menu.
					else if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
				}
				else if (level == 2) {
					//Takes the player to the main menu and returns all the ingredients they have selected.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						for each (Ingredients ingredient in currentCraftingAttempt) {
							player.addIngredientToInventory(ingredient.getName());
						}
						currentCraftingAttempt.clear();
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					//Returns all ingredients the player has already selected to their inventory.
					else if (resetCrafting.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						for each (Ingredients ingredient in currentCraftingAttempt) {
							player.addIngredientToInventory(ingredient.getName());
						}
						currentCraftingAttempt.clear();
					}
					//Tries to craft an item with the ingredients that the player has selected. Also makes time pass in the store and for adventurers.
					else if (confirmCrafting.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						player.attemptCrafting(currentCraftingAttempt);
						if (currentCraftingAttempt.size() != 0) {
							playerShop.addProgress();
							//Currently using finding by name. Might change to something more efficient later.
							for each (Adventurers adventurer in player.getRecruitedAdventurers()) {
								if (adventurer.getAssignedArea().getName() != "DEFAULT") {
									tavern = player.addAdventurerProgress(player.locate(adventurer.getName()), tavern);
								}
							}
						}
						currentCraftingAttempt.clear();
					}
				}
				else if (level == 3) {
					//Takes the player to an area where they can add items to the store.
					if (startSale.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 4;
						levelIndicator.setString("Item Listing");
						itemList.setPosition(45, 75);
						cursor.setPosition(15, 77.5);
					}
					//Takes the player to the main menu.
					else if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					//Adds the money the store is holding to the player's inventory and clears the store.
					else if (collectMoney.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						player.gainMoney(playerShop.getMoney());
						playerShop.resetMoney();
						displayShopMoney.setString("The shop currently carries " + to_string(playerShop.getMoney()) + " currency.");
					}
				}
				else if (level == 4) {

					//Takes the player to the main menu.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
				}
				else if (level == 5) {
					//Takes the player to the main menu.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					//Lets the player choose where to send an adventurer.
					else if (sendAdventurers.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 6;
						levelIndicator.setString("Area selection");
						cursor.setPosition(15, 107.5);
					}
					else if (spendTime.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						playerShop.addProgress();
						for each (Adventurers adventurer in player.getRecruitedAdventurers()) {
							if (adventurer.getAssignedArea().getName() != "DEFAULT") {
								tavern = player.addAdventurerProgress(player.locate(adventurer.getName()), tavern);
							}
						}
						if (tavern.getIngredientsHeld().size() + tavern.getItemsHeld().size() > 0)
							collectItems.changeString("Collect items");
					}
					//Adds the adventurer's items to the player's inventory.
					else if (collectItems.isOverlapping(event.mouseButton.x, event.mouseButton.y) || recheckTavern == true) {
						int position = 0;

						int i_ingredients = 0;
						int i_rejuvHerb = 0;
						int i_energyHerb = 0;
						int i_pureWater = 0;

						int i_items = 0;
						int i_lessHealPotion = 0;
						int i_lessManaPotion = 0;
						//Fetching ingredients.
						for each (Ingredients ingredient in tavern.getIngredientsHeld()) {
							if (ingredient.getName() == "Rejuvenation Herbs")
								i_rejuvHerb++;
							else if (ingredient.getName() == "Energy-Filled Herbs")
								i_energyHerb++;
							else if (ingredient.getName() == "Purified Water")
								i_pureWater++;
							else {
								string errorMessage = "An unknown ingredient was found: " + ingredient.getName();
								printError(errorMessage.c_str());
							}
							i_ingredients++;
						}
						//Fetching items.
						for each (Items item in tavern.getItemsHeld()) {
							if (item.getName() == "Lesser Healing Potion")
								i_lessHealPotion++;
							else if (item.getName() == "Lesser Mana Potion")
								i_lessManaPotion++;
							else {
								string errorMessage = "An unknown item was found: " + item.getName();
								printError(errorMessage.c_str());
							}
							i_items++;
						}

						cout << "Ingredients: " + to_string(i_rejuvHerb) + ", " + to_string(i_energyHerb) + ", " + to_string(i_pureWater) << endl;
						cout << "Items: " + to_string(i_lessHealPotion) + ", " + to_string(i_lessManaPotion) << endl;

						if (recheckTavern == false)
							collectItems.changeString(to_string(i_ingredients) + " ingredients and " + to_string(i_items) + " items obtained!");

						if (i_rejuvHerb != 0) {
							position = player.locate("Rejuvenation Herbs");
							if (position != 5000)
								tempIngredient = Ingredients("Rejuvenation Herbs", player.getIngredientInventory().at(position).getAmount() + i_rejuvHerb);
							else {
								tempIngredient = rejuvHerb;
								tempIngredient.setAmount(i_rejuvHerb);
							}
							tavern.deleteIngredients("Rejuvenation Herbs");
							updateIngredient = true;
							recheckTavern = true;
							break;
						}
						if (i_energyHerb != 0) {
							position = player.locate("Energy-Filled Herbs");
							if (position != 5000)
								tempIngredient = Ingredients("Energy-Filled Herbs", player.getIngredientInventory().at(position).getAmount() + i_energyHerb);
							else {
								tempIngredient = energyHerb;
								tempIngredient.setAmount(i_energyHerb);
							}
							tavern.deleteIngredients("Energy-Filled Herbs");
							updateIngredient = true;
							recheckTavern = true;
							break;
						}
						if (i_pureWater != 0){
							position = player.locate("Purified Water");
							if (position != 5000)
								tempIngredient = Ingredients("Purified Water", player.getIngredientInventory().at(position).getAmount() + i_pureWater);
							else {
								tempIngredient = pureWater;
								tempIngredient.setAmount(i_pureWater);
							}
							tavern.deleteIngredients("Purified Water");
							updateIngredient = true;
							recheckTavern = true;
							break;
						}
						if (i_lessHealPotion != 0) {
							position = player.locate("Lesser Healing Potion");
							if (position != 5000)
								tempItem = Items("Lesser Healing Potion", player.getItemInventory().at(position).getAmount() + i_lessHealPotion, player.getItemInventory().at(position).getPrice(), player.getItemInventory().at(position).getThereshold(), player.getItemInventory().at(position).isRecipeKnown());
							else {
								tempItem = lessHealPotion;
								tempItem.setAmount(i_lessHealPotion);
							}
							tavern.deleteItems("Lesser Healing Potion");
							updateItem = true;
							recheckTavern = true;
							break;
						}
						if (i_lessManaPotion != 0) {
							position = player.locate("Lesser Mana Potion");
							if (position != 5000)
								tempItem = Items("Lesser Mana Potion", player.getItemInventory().at(position).getAmount() + i_lessManaPotion, player.getItemInventory().at(position).getPrice(), player.getItemInventory().at(position).getThereshold(), player.getItemInventory().at(position).isRecipeKnown());
							else {
								tempItem = lessManaPotion;
								tempItem.setAmount(i_lessManaPotion);
							}
							tavern.deleteItems("Lesser Mana Potion");
							updateItem = true;
							recheckTavern = true;
							break;
						}
						recheckTavern = false;
					}
				}
				else if (level == 6) {
					//Takes the player to the main menu.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					else if (confirm.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						tempArea = player.getUnlockedAreas().at(newCursorPosition);
						level = 7;
						levelIndicator.setString("Adventurer selection");
						cursor.setPosition(15, 107.5);
					}
				}
				else if (level == 7) {
					//Takes the player to the main menu.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					else if (confirm.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						if (player.getRecruitedAdventurers().at(newCursorPosition).getAssignedArea().getName() == "DEFAULT") {
							//Set the adventurer's assigned area to the chosen area.
							player.updateAdventurerLocation(player.locate(player.getRecruitedAdventurers().at(newCursorPosition).getName()), tempArea);
							level = 0;
							levelIndicator.setString("Main Menu");
							updateArea = true;
						}
					}
				}
				else if (level == 8) {
					//Takes the player to the main menu.
					if (returnToMenu.isOverlapping(event.mouseButton.x, event.mouseButton.y)) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
				}
			}
			//Checks if the player is pressing a button and starts the proper events.
			if (event.type == Event::KeyPressed){;
				if (level == 2){
					//Allows the player to choose between their ingredients.
					if (event.key.code == Keyboard::Up){
						newCursorPosition--;
						if (newCursorPosition <= 0)
							newCursorPosition = 0;
					}
					else if (event.key.code == Keyboard::Down){
						newCursorPosition++;
						if (newCursorPosition >= player.getIngredientInventory().size() - 1)
							newCursorPosition = player.getIngredientInventory().size() - 1;
					}
					else if (event.key.code == Keyboard::Return){
						//Adds the selected ingredient to a temporary vector that will be used for crafting.
						tempIngredient = player.getIngredientInventory().at(newCursorPosition);
						if (tempIngredient.getAmount() != 0){
							currentCraftingAttempt.push_back(tempIngredient);
							tempIngredient.loseAmount(1);
							updateIngredient = true;
						}
					}
				}
				else if (level == 4) {
					//Allows the player to choose between their items.
					if (event.key.code == Keyboard::Up) {
						newCursorPosition--;
						if (newCursorPosition <= 0)
							newCursorPosition = 0;
					}
					else if (event.key.code == Keyboard::Down) {
						newCursorPosition++;
						if (newCursorPosition >= player.getItemInventory().size() - 1)
							newCursorPosition = player.getItemInventory().size() - 1;
					}
					//Adds the selected item to the player's shop.
					else if (event.key.code == Keyboard::Return) {
						tempItem = player.getItemInventory().at(newCursorPosition);
						if (tempItem.getAmount() != 0) {
							playerShop.stockItem(Items(tempItem.getName(), 1, tempItem.getPrice(), tempItem.getThereshold(), tempItem.isRecipeKnown()));
							tempItem.loseAmount(1);
							updateItem = true;
						}
					}
				}
				else if (level == 6) {
					if (event.key.code == Keyboard::Up) {
						newCursorPosition--;
						if (newCursorPosition <= 0)
							newCursorPosition = 0;
					}
					else if (event.key.code == Keyboard::Down) {
						newCursorPosition++;
						if (newCursorPosition >= player.getUnlockedAreas().size() - 1) {
							newCursorPosition = player.getUnlockedAreas().size() - 1;
						}
					}
				}
				else if (level == 7) {
					if (event.key.code == Keyboard::Up) {
						newCursorPosition--;
						if (newCursorPosition <= 0)
							newCursorPosition = 0;
					}
					else if (event.key.code == Keyboard::Down) {
						newCursorPosition++;
						if (newCursorPosition >= player.getRecruitedAdventurers().size() - 1) {
							newCursorPosition = player.getRecruitedAdventurers().size() - 1;
						}
					}
				}
			}
		}
		if (updateArea == true) {
			tempArea.setName("DEFAULT");
			updateArea = false;
		}
		//Updates ingredients and items if needed.
		if (updateIngredient == true){
			player.updateIngredientInventory(tempIngredient);
			updateIngredient = false;
			//Resets the temporary ingredient.
			tempIngredient.setName("DEFAULT");
			tempIngredient.setAmount(0);
		}
		if (updateItem == true) {
			player.updateItemInventory(tempItem);
			updateItem = false;
			//Resets the temporary item.
			tempItem.setName("DEFAULT");
			tempItem.setAmount(0);
			tempItem.setPrice(0);
			tempItem.forgetRecipe();
		}
		//Updates selection cursor position.
		if (level == 2 || level == 4 || level == 6 || level == 7){
			if (oldCursorPosition < newCursorPosition){
				oldCursorPosition++;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y + 30);
			}
			else if (oldCursorPosition > newCursorPosition){
				oldCursorPosition--;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y - 30);
			}
		}
		//Updates the strings for the HUD/list texts.
		ingredientList.setString(generateIngredientList(player));
		itemList.setString(generateItemList(player));
		adventurerList.setString(generateAdventurerList(player, tempArea));
		shopItems.setString(generateSaleList(playerShop));
		areaList.setString(generateAreaList(player));
		displayMoney.setString("Currency: " + to_string(player.getMoney()));
		levelIndicator.setOrigin(levelIndicator.getLocalBounds().left + levelIndicator.getLocalBounds().width / 2,
			levelIndicator.getLocalBounds().top + levelIndicator.getLocalBounds().height / 2);
		//Clear the window of what it's currently showing, then redraw the objects and display the new frame.
		mainWindow.clear(Color::Black);
		mainWindow.draw(displayMoney);
		mainWindow.draw(levelIndicator);
		switch (level){
		case 0:
			//Main screen
			mainWindow.draw(enterShop.getButton());
			mainWindow.draw(enterShop.getLabel());
			mainWindow.draw(enterTavern.getButton());
			mainWindow.draw(enterTavern.getLabel());
			mainWindow.draw(startCrafting.getButton());
			mainWindow.draw(startCrafting.getLabel());
			break;
		case 1:
			//Item creation method selector
			mainWindow.draw(experiment.getButton());
			mainWindow.draw(experiment.getLabel());
			mainWindow.draw(craftFromRecipe.getButton());
			mainWindow.draw(craftFromRecipe.getLabel());
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			break;
		case 2:
			//Item creation (experimentation) screen
			mainWindow.draw(confirmCrafting.getButton());
			mainWindow.draw(confirmCrafting.getLabel());
			mainWindow.draw(resetCrafting.getButton());
			mainWindow.draw(resetCrafting.getLabel());
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(cursor);
			mainWindow.draw(ingredientList);
			mainWindow.draw(itemList);
			break;
		case 3:
			//The shop
			mainWindow.draw(startSale.getButton());
			mainWindow.draw(startSale.getLabel());
			mainWindow.draw(collectMoney.getButton());
			mainWindow.draw(collectMoney.getLabel());	
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(shopItems);
			mainWindow.draw(displayShopMoney);
			break;
		case 4:
			//Placing items in the shop
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(itemList);
			mainWindow.draw(cursor);
			break;
		case 5:
			//The tavern
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(sendAdventurers.getButton());
			mainWindow.draw(sendAdventurers.getLabel());
			mainWindow.draw(spendTime.getButton());
			mainWindow.draw(spendTime.getLabel());
			mainWindow.draw(collectItems.getButton());
			mainWindow.draw(collectItems.getLabel());
			break;
		case 6:
			//Selecting area to send adventurers.
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(confirm.getButton());
			mainWindow.draw(confirm.getLabel());
			mainWindow.draw(cursor);
			mainWindow.draw(areaList);
			break;
		case 7:
			//Selecting adventurer to send.
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(confirm.getButton());
			mainWindow.draw(confirm.getLabel());
			mainWindow.draw(adventurerList);
			mainWindow.draw(cursor);
			break;
		case 8:
			//Crafting from recipe
			mainWindow.draw(returnToMenu.getButton());
			mainWindow.draw(returnToMenu.getLabel());
			mainWindow.draw(itemList);
			mainWindow.draw(cursor);
			break;
		default:
			//Level does not exist
			if (levelErrorPrinted == false){
				string errorMessage = "The application attempted loading level id " + to_string(level) + ", but such a level does not exist.";
				printError(errorMessage.c_str());
				levelErrorPrinted = true;
			}
			break;
		}
		mainWindow.display();
	}
	return 0;
}

string generateIngredientList(Player player){
	//Generates a readable string for the player's ingredients.
	string newList;

	newList = "Ingredients:\n\n";

	for each (Ingredients ingredient in player.getIngredientInventory()){
		newList += ingredient.getName() + ": " + to_string(ingredient.getAmount()) + " owned.\n";
	}
	return newList;
}

string generateItemList(Player player){
	//Generates a readable string for the player's items.
	string newList;

	for each (Items item in player.getItemInventory()){
		newList += item.getName() + ": " + to_string(item.getAmount()) + " owned.\n";
	}
	return newList;
}

string generateSaleList(Shop shop) {
	//Generates a readable string for the shop's items.
	string newList;

	for each(Items item in shop.getShopItems()) {
		newList += item.getName() + ". " + to_string(item.getAmount()) + " carried for " + to_string(item.getPrice()) + " currency each.\n";
	}
	return newList;
}

string generateAdventurerList(Player player, Areas area) {
	string newList;

	newList = "What adventurer would you like to send to " + area.getName() + "?\n\n";

	for each(Adventurers adventurer in player.getRecruitedAdventurers()) {
		newList += adventurer.getName() + ". ATK: " + to_string(adventurer.getAttack()) + ". DEF: " + to_string(adventurer.getDefense()) + ". M.ATK: " + to_string(adventurer.getMagicAttack()) + ". M.DEF: " + to_string(adventurer.getMagicDefense()) + ". ";

		if (adventurer.getAssignedArea().getName() == "DEFAULT") {
			newList += "(FREE)";
		}
		else
			newList += "(BUSY)";
		newList += "\n";
	}
	return newList;
}

string generateAreaList(Player player) {
	string newList;

	newList = "Where would you like to send your adventurer?\n\n";

	for each (Areas area in player.getUnlockedAreas()) {
		newList += area.getName() + "\n";
	}
	return newList;
}