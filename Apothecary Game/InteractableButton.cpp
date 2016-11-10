#include "InteractableButton.h"

InteractableButton::InteractableButton(int a_xSize, int a_ySize, int a_xPos, int a_yPos, string a_label, Font & a_font, Color textBlue, Color buttonYellow){
	button.setSize(Vector2f(a_xSize, a_ySize));
	button.setPosition(a_xPos, a_yPos);
	button.setFillColor(buttonYellow);

	buttonLabel.setString(a_label);
	buttonLabel.setFont(a_font);
	buttonLabel.setCharacterSize(20);
	buttonLabel.setFillColor(textBlue);
	buttonLabel.setOrigin(buttonLabel.getLocalBounds().left + buttonLabel.getLocalBounds().width / 2,
		buttonLabel.getLocalBounds().top + buttonLabel.getLocalBounds().height / 2);
	buttonLabel.setPosition(a_xPos + (a_xSize / 2), a_yPos + (a_ySize / 2));
}