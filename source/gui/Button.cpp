#include <SFML/Window/Mouse.hpp>
#include "Button.hpp"

Button::Button(const sf::Vector2f & size, const sf::Vector2f & position,
const float & outlineThickness, const sf::Color & idleOutlineColor,
const sf::Color & idleShapeColor, const sf::Color & idleTextColor,
sf::Font & font, const char * textString, const std::uint32_t & fontSize)
: text(font, textString, fontSize)
{
	this->buttonState = ButtonStates::idle;
	this->mouseClickedState = false;

	this->buttonColors[ButtonStates::hover].outlineColor = sf::Color
	(idleOutlineColor.r - 32,idleOutlineColor.g - 32, idleOutlineColor.b - 32);
	this->buttonColors[ButtonStates::hover].shapeColor = sf::Color
	(idleShapeColor.r - 32, idleShapeColor.g - 32, idleShapeColor.b - 32);
	this->buttonColors[ButtonStates::hover].textColor = sf::Color
	(idleTextColor.r - 32, idleTextColor.g - 32, idleTextColor.b - 32);

	this->buttonColors[ButtonStates::idle].outlineColor = idleOutlineColor;
	this->buttonColors[ButtonStates::idle].shapeColor = idleShapeColor;
	this->buttonColors[ButtonStates::idle].textColor = idleTextColor;

	this->buttonColors[ButtonStates::pressed].outlineColor = sf::Color
	(idleOutlineColor.r - 48, idleOutlineColor.g - 48, idleOutlineColor.b - 48);
	this->buttonColors[ButtonStates::pressed].shapeColor = sf::Color
	(idleShapeColor.r - 48, idleShapeColor.g - 48, idleShapeColor.b - 48);
	this->buttonColors[ButtonStates::pressed].textColor = sf::Color
	(idleTextColor.r - 48, idleTextColor.g - 48, idleTextColor.b - 48);
	
	this->shape.setOutlineThickness(outlineThickness);
	this->shape.setPosition(position);
	this->shape.setSize(size);
	this->centerText();
	this->updateColors();
}

void Button::centerText()
{
	this->text.setOrigin(this->text.getGlobalBounds().size / 2.f
	+ this->text.getLocalBounds().position);
	this->text.setPosition(this->shape.getPosition() + (this->shape.getSize() / 2.f));
}

const bool Button::isPressed()
{
	return this->buttonState == ButtonStates::pressed && !this->mouseClickedState;
}

void Button::render(sf::RenderTarget & window)
{
	window.draw(this->shape);
	window.draw(this->text);
}

void Button::update(sf::Vector2f & mousePosition)
{
	ButtonStates previousButtonState = this->buttonState;
	this->buttonState = ButtonStates::idle;
	if(this->shape.getGlobalBounds().contains(mousePosition))
	{
		this->buttonState = ButtonStates::hover;
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			this->buttonState = ButtonStates::pressed;
			this->mouseClickedState = true;
		}
		else if(this->mouseClickedState)	//Mouse button released.
		{
			this->buttonState = ButtonStates::pressed;
			this->mouseClickedState = false;
		}
	}

	if(this->buttonState != previousButtonState)
	{
		this->updateColors();
	}
}

void Button::updateColors()
{
	this->shape.setFillColor(this->buttonColors[this->buttonState].shapeColor);
	this->shape.setOutlineColor(this->buttonColors[this->buttonState].outlineColor);
	this->text.setFillColor(this->buttonColors[this->buttonState].textColor);
}