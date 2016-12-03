#include "InteractableButton.h"

InteractableButton::InteractableButton(int a_xSize, int a_ySize, int a_xPos, int a_yPos, string a_label, Font & a_font, Color textDefault, Color buttonDefault, Texture & a_texture){
	button.setSize(Vector2f(a_xSize, a_ySize));
	button.setPosition(a_xPos, a_yPos);
	button.setFillColor(buttonDefault);
	button.setTexture(&a_texture);

	buttonLabel.setString(a_label);
	buttonLabel.setFont(a_font);
	buttonLabel.setCharacterSize(20);
	buttonLabel.setFillColor(textDefault);
	buttonLabel.setOrigin(buttonLabel.getLocalBounds().left + buttonLabel.getLocalBounds().width / 2,
		buttonLabel.getLocalBounds().top + buttonLabel.getLocalBounds().height / 2);
	buttonLabel.setPosition(a_xPos + (a_xSize / 2), a_yPos + (a_ySize / 2));
}

void InteractableButton::changeString(string newString) {
	buttonLabel.setString(newString);
	buttonLabel.setOrigin(buttonLabel.getLocalBounds().left + buttonLabel.getLocalBounds().width / 2,
		buttonLabel.getLocalBounds().top + buttonLabel.getLocalBounds().height / 2);
}

bool InteractableButton::isOverlapping(int x_pos, int y_pos){
	if (x_pos >= left() && x_pos <= right() && y_pos >= top() && y_pos <= bottom())
		return true;
	else
		return false;
}
