#include <time.h>
#include <SFML\Graphics.hpp>
#include <SFML\Main.hpp>
#include <SFML\Window.hpp>
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
	Color shadedYellow(101, 94, 0);
	Color textBlue(89, 25, 255);
	Color buttonYellow(178, 164, 0);

	//Create shapes that are currently used for buttons.
	InteractableButton enterShop(200, 100, 1035, 575, "Enter your Shop", fontAntiqua, textBlue, buttonYellow);
	InteractableButton enterTavern(200, 100, 540, 575, "Enter the Tavern", fontAntiqua, textBlue, buttonYellow);
	InteractableButton startCrafting(200, 100, 45, 575, "Start Crafting", fontAntiqua, textBlue, buttonYellow);
	InteractableButton confirmCrafting(200, 100, 1035, 575, "Craft!", fontAntiqua, textBlue, buttonYellow);
	InteractableButton returnToMenu(200, 100, 45, 575, "Back to Menu", fontAntiqua, textBlue, buttonYellow);
	InteractableButton experiment(400, 50, 440, 200, "Start experimenting", fontAntiqua, textBlue, buttonYellow);
	InteractableButton startSale(200, 100, 1035, 575, "Sell an item", fontAntiqua, textBlue, buttonYellow);
	InteractableButton collectMoney(200, 100, 540, 575, "Collect", fontAntiqua, textBlue, buttonYellow);

	Texture texture;
	if (!texture.loadFromFile("Blue_Arrow_001.png"))
		printError("File Blue_Arrow_001.png could not be loaded.");
	Vector2u textureSize = texture.getSize();
	RectangleShape cursor(Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y)));
	cursor.setTexture(&texture);

	//Creates text instances. If the text is used in multiple levels, the position is set when the level changes.
	Text ingredientList;
	ingredientList.setFont(fontAntiqua);
	ingredientList.setCharacterSize(25);
	ingredientList.setFillColor(textBlue);

	Text itemList;
	itemList.setFont(fontAntiqua);
	itemList.setCharacterSize(25);
	itemList.setFillColor(textBlue);

	Text shopItems;
	shopItems.setFont(fontAntiqua);
	shopItems.setCharacterSize(25);
	shopItems.setFillColor(textBlue);
	shopItems.setPosition(45, 45);

	Text displayMoney;
	displayMoney.setFont(fontAntiqua);
	displayMoney.setCharacterSize(25);
	displayMoney.setFillColor(textBlue);
	displayMoney.setPosition(1000, 10);

	Text displayShopMoney;
	displayShopMoney.setFont(fontAntiqua);
	displayShopMoney.setCharacterSize(25);
	displayShopMoney.setFillColor(textBlue);
	displayShopMoney.setPosition(45, 10);

	Text levelIndicator;
	levelIndicator.setFont(fontAntiqua);
	levelIndicator.setCharacterSize(25);
	levelIndicator.setFillColor(textBlue);
	levelIndicator.setPosition(540, 10);

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

	player.learnIngredient(rejuvHerb);
	player.learnIngredient(energyHerb);
	player.learnIngredient(pureWater);

	player.learnItem(lessHealPotion);

	Shop playerShop;

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
					if (event.mouseMove.x >= startCrafting.left() && event.mouseMove.x <= startCrafting.right()
						&& event.mouseMove.y >= startCrafting.top() && event.mouseMove.y <= startCrafting.bottom()) {
						startCrafting.changeButtonColor(shadedYellow);
					}
					else if (event.mouseMove.x >= enterTavern.left() && event.mouseMove.x <= enterTavern.right()
						&& event.mouseMove.y >= enterTavern.top() && event.mouseMove.y <= enterTavern.bottom()) {
						enterTavern.changeButtonColor(shadedYellow);
					}
					else if (event.mouseMove.x >= enterShop.left() && event.mouseMove.x <= enterShop.right()
						&& event.mouseMove.y >= enterShop.top() && event.mouseMove.y <= enterShop.bottom()) {
						enterShop.changeButtonColor(shadedYellow);
					}
					else{
						enterShop.changeButtonColor(buttonYellow);
						enterTavern.changeButtonColor(buttonYellow);
						startCrafting.changeButtonColor(buttonYellow);
					}
				}
				else if (level == 1){
					if (event.mouseMove.x >= experiment.left() && event.mouseMove.x <= experiment.right()
						&& event.mouseMove.y >= experiment.top() && event.mouseMove.y <= experiment.bottom()) {
						experiment.changeButtonColor(shadedYellow);
					}
					else {
						experiment.changeButtonColor(buttonYellow);
					}
				}
				else if (level == 2){
					if (event.mouseMove.x >= returnToMenu.left() && event.mouseMove.x <= returnToMenu.right()
						&& event.mouseMove.y >= returnToMenu.top() && event.mouseMove.y <= returnToMenu.bottom()) {
						returnToMenu.changeButtonColor(shadedYellow);
					}
					else if (event.mouseMove.x >= confirmCrafting.left() && event.mouseMove.x <= confirmCrafting.right()
						&& event.mouseMove.y >= confirmCrafting.top() && event.mouseMove.y <= confirmCrafting.bottom()) {
						confirmCrafting.changeButtonColor(shadedYellow);
					}
					else {
						returnToMenu.changeButtonColor(buttonYellow);
						confirmCrafting.changeButtonColor(buttonYellow);
					}
				}
				else if (level == 3) {
					if (event.mouseMove.x >= startSale.left() && event.mouseMove.x <= startSale.right()
						&& event.mouseMove.y >= startCrafting.top() && event.mouseMove.y <= startSale.bottom()) {
						startSale.changeButtonColor(shadedYellow);
					}
					else if (event.mouseMove.x >= returnToMenu.left() && event.mouseMove.x <= returnToMenu.right()
						&& event.mouseMove.y >= returnToMenu.top() && event.mouseMove.y <= returnToMenu.bottom()) {
						returnToMenu.changeButtonColor(shadedYellow);
					}
					else if (event.mouseMove.x >= collectMoney.left() && event.mouseMove.x <= collectMoney.right()
						&& event.mouseMove.y >= collectMoney.top() && event.mouseMove.y <= collectMoney.bottom()) {
						collectMoney.changeButtonColor(shadedYellow);
					}
					else {
						startSale.changeButtonColor(buttonYellow);
						returnToMenu.changeButtonColor(buttonYellow);
						collectMoney.changeButtonColor(buttonYellow);
					}
				}
				else if (level == 4) {
					if (event.mouseMove.x >= returnToMenu.left() && event.mouseMove.x <= returnToMenu.right()
						&& event.mouseMove.y >= returnToMenu.top() && event.mouseMove.y <= returnToMenu.bottom())
						returnToMenu.changeButtonColor(shadedYellow);
					else
						returnToMenu.changeButtonColor(buttonYellow);
				}
			}
			//Checks if the mouse is over a clickable object if the left mouse button is pressed.
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (level == 0) {
					//Takes the player to the Crafting Method Selector.
					if (event.mouseButton.x >= startCrafting.left() && event.mouseButton.x <= startCrafting.right()
						&& event.mouseButton.y >= startCrafting.top() && event.mouseButton.y <= startCrafting.bottom()) {
						level = 1;
						levelIndicator.setString("Crafting Method Selector");
					}
					//Takes the player to the tavern.
					else if (event.mouseButton.x >= enterTavern.left() && event.mouseButton.x <= enterTavern.right()
						&& event.mouseButton.y >= enterTavern.top() && event.mouseButton.y <= enterTavern.bottom()) {
						//TODO: Implement tavern.
					}
					//Takes the player to their shop.
					else if (event.mouseButton.x >= enterShop.left() && event.mouseButton.x <= enterShop.right()
						&& event.mouseButton.y >= enterShop.top() && event.mouseButton.y <= enterShop.bottom()) {
						level = 3;
						levelIndicator.setString("Your Shop");
						displayShopMoney.setString("The shop currently carries " + to_string(playerShop.getMoney()) + " currency.");
					}
				}
				else if (level == 1) {
					//Takes the player to the experimentation area and sets the lists up.
					if (event.mouseButton.x >= experiment.left() && event.mouseButton.x <= experiment.right()
						&& event.mouseButton.y >= experiment.top() && event.mouseButton.y <= experiment.bottom()) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 2;
						levelIndicator.setString("Experimentation");
						ingredientList.setPosition(45, 15);
						itemList.setPosition(500, 75);
						cursor.setPosition(15, 77.5);
					}
				}
				else if (level == 2) {
					//Takes the player to the main menu.
					if (event.mouseButton.x >= returnToMenu.left() && event.mouseButton.x <= returnToMenu.right()
						&& event.mouseButton.y >= returnToMenu.top() && event.mouseButton.y <= returnToMenu.bottom()) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					//Tries to craft an item with the ingredients that the player has selected. Also makes time pass in the store.
					else if (event.mouseButton.x >= confirmCrafting.left() && event.mouseButton.x <= confirmCrafting.right()
						&& event.mouseButton.y >= confirmCrafting.top() && event.mouseButton.y <= confirmCrafting.bottom()) {
						player.attemptCrafting(currentCraftingAttempt);
						currentCraftingAttempt.clear();
						playerShop.addProgress();
					}
				}
				else if (level == 3) {
					//Takes the player to an area where they can add items to the store.
					if (event.mouseButton.x >= startSale.left() && event.mouseButton.x <= startSale.right()
						&& event.mouseButton.y >= startSale.top() && event.mouseButton.y <= startSale.bottom()) {
						oldCursorPosition = 0;
						newCursorPosition = 0;
						level = 4;
						levelIndicator.setString("Item Listing");
						itemList.setPosition(45, 75);
						cursor.setPosition(15, 77.5);
					}
					//Takes the player to the main menu.
					else if (event.mouseButton.x >= returnToMenu.left() && event.mouseButton.x <= returnToMenu.right()
						&& event.mouseButton.y >= returnToMenu.top() && event.mouseButton.y <= returnToMenu.bottom()) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
					//Adds the money the store is holding to the player's inventory and clears the store.
					else if (event.mouseButton.x >= collectMoney.left() && event.mouseButton.x <= collectMoney.top()
						&& event.mouseButton.y >= collectMoney.top() && event.mouseButton.y <= collectMoney.bottom()) {
						player.gainMoney(playerShop.getMoney());
						playerShop.resetMoney();
						displayShopMoney.setString("The shop currently carries " + to_string(playerShop.getMoney()) + " currency.");
					}
				}
				else if (level == 4) {
					//Takes the player to the main menu.
					if (event.mouseButton.x >= returnToMenu.left() && event.mouseButton.x <= returnToMenu.right()
						&& event.mouseButton.y >= returnToMenu.top() && event.mouseButton.y <= returnToMenu.bottom()) {
						level = 0;
						levelIndicator.setString("Main Menu");
					}
				}
			}
			//Checks if the player is pressing a button and starts the proper events.
			if (event.type == Event::KeyPressed){
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
							playerShop.stockItem(Items(tempItem.getName(), 1, tempItem.getPrice(), tempItem.getProgress(), tempItem.isRecipeKnown()));
							tempItem.loseAmount(1);
							updateItem = true;
						}
					}
				}
			}
		}
		//Updates ingredients and items if needed.
		if (updateIngredient == true){
			player.updateIngredientInventory(tempIngredient);
			updateIngredient = false;
		}
		if (updateItem == true) {
			player.updateItemInventory(tempItem);
			updateItem = false;
		}
		//Updates selection cursor position.
		if (level == 2){
			if (oldCursorPosition < newCursorPosition){
				oldCursorPosition++;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y + 30);
			}
			else if (oldCursorPosition > newCursorPosition){
				oldCursorPosition--;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y - 30);
			}
		}
		else if (level == 4) {
			if (oldCursorPosition < newCursorPosition) {
				oldCursorPosition++;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y + 30);
			}
			else if (oldCursorPosition > newCursorPosition) {
				oldCursorPosition--;
				cursor.setPosition(cursor.getPosition().x, cursor.getPosition().y - 30);
			}
		}
		//Resets the temporary ingredient.
		tempIngredient.setName("DEFAULT");
		tempIngredient.setAmount(0);
		//Resets the temporary item.
		tempItem.setName("DEFAULT");
		tempItem.setAmount(0);
		tempItem.setPrice(0);
		tempItem.forgetRecipe();
		//Updates the strings for the texts.
		ingredientList.setString(generateIngredientList(player));
		itemList.setString(generateItemList(player));
		shopItems.setString(generateSaleList(playerShop));
		displayMoney.setString("Currency: " + to_string(player.getMoney()));
		//Clear the window of what it's currently showing, then redraw the objects and display the new frame.
		mainWindow.clear(Color::Black);
		mainWindow.draw(displayMoney);
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
			break;
		case 2:
			//Item creation (experimentation) screen
			mainWindow.draw(confirmCrafting.getButton());
			mainWindow.draw(confirmCrafting.getLabel());
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
			//The tavern (Hiring adventurers)
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
		mainWindow.draw(levelIndicator);
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