#pragma once
#include <SFML\Graphics.hpp>

using namespace std;
using namespace sf;

class InteractableButton {
public:
	InteractableButton(int a_xSize, int a_ySize, int a_xPos, int a_yPos, string a_label, Font & a_font, Color textDefault, Color buttonDefault, Texture & a_texture);

	void changeButtonColor(Color newColor) {
		button.setFillColor(newColor);
	}
	void changeTextColor(Color newColor) {
		buttonLabel.setFillColor(newColor);
	}

	float left() {
		return button.getPosition().x;
	}
	float right() {
		return button.getPosition().x + button.getSize().x;
	}
	float top() {
		return button.getPosition().y;
	}
	float bottom() {
		return button.getPosition().y + button.getSize().y;
	}

	bool isOverlapping(int x_pos, int y_pos);

	RectangleShape getButton() {
		return button;
	}

	Text getLabel() {
		return buttonLabel;
	}
private:
	RectangleShape button;
	Text buttonLabel;
};