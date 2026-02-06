#include <SFML/Window/Mouse.hpp>
#include "InputBox.hpp"

#define BACKSPACE_VALUE 8
#define FIRST_VISIBLE_CHARACTER_VALUE 32
#define LAST_VISIBLE_CHARACTER_VALUE 126

sf::String InputBox::getInputString()
{
	return this->inputText.getString();
}

InputBox::InputBox(const sf::Vector2f & size, const sf::Vector2f & position,
const float & outlineThickness, const sf::Color & idleOutlineColor,
const sf::Color & idleShapeColor, const sf::Color & idleTextColor,
sf::Font & font, const std::uint32_t & fontSize, const std::uint32_t & charactersLimit)
: inputText(font, "", fontSize)
{
	this->charactersLimit = charactersLimit;
	this->hasFocus = false;
	this->mouseClickedState = false;
	
	this->inputText.setFillColor(idleTextColor);
	//Moving text 1% right and centering it in horizontaly.
	this->inputText.setPosition({position.x + size.x / 100.f, position.y + size.y / 5.f});

	this->shape.setFillColor(idleShapeColor);
	this->shape.setOutlineColor(idleOutlineColor);
	this->shape.setOutlineThickness(outlineThickness);
	this->shape.setPosition(position);
	this->shape.setSize(size);
}

const bool InputBox::isFocused()
{
	return this->hasFocus;
}

void InputBox::removeFocus()
{
	this->updateFocus();
}

void InputBox::render(sf::RenderTarget & window)
{
	window.draw(this->shape);
	window.draw(this->inputText);
}

void InputBox::update(sf::Vector2f & mousePosition, char32_t & keyPressed)
{
	if(this->shape.getGlobalBounds().contains(mousePosition))
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			this->mouseClickedState = true;
		}
		else if(this->mouseClickedState)	//Mouse button released.
		{
			this->mouseClickedState = false;
			this->updateFocus();
		}
	}

	if(this->hasFocus)
	{
		this->updateInputText(keyPressed);
	}
}

void InputBox::updateFocus()
{
	sf::Color outlineColor = this->shape.getOutlineColor();
	sf::Color shapeColor = this->shape.getFillColor();
	sf::Color textColor = this->inputText.getFillColor();

	if(this->hasFocus)
	{
		this->hasFocus = false;
		outlineColor = sf::Color(outlineColor.r + 48,
		outlineColor.g + 48, outlineColor.b + 48);
		shapeColor = sf::Color(shapeColor.r + 48,
		shapeColor.g + 48, shapeColor.b + 48);
		textColor = sf::Color(textColor.r + 48,
		textColor.g + 48, textColor.b + 48);
	}
	else
	{
		this->hasFocus = true;
		outlineColor = sf::Color(outlineColor.r - 48,
		outlineColor.g - 48, outlineColor.b - 48);
		shapeColor = sf::Color(shapeColor.r - 48,
		shapeColor.g - 48, shapeColor.b - 48);
		textColor = sf::Color(textColor.r - 48,
		textColor.g - 48, textColor.b - 48);
	}

	this->inputText.setFillColor(textColor);
	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(shapeColor);
}
#include <SFML/Network/Ftp.hpp>
void InputBox::updateInputText(char32_t & key)
{
	std::string text = this->inputText.getString();
	if((text.length() == this->charactersLimit && key != BACKSPACE_VALUE)
	|| (text.empty() && key == BACKSPACE_VALUE) || 
	(key < FIRST_VISIBLE_CHARACTER_VALUE && key != BACKSPACE_VALUE) 
	|| (key > LAST_VISIBLE_CHARACTER_VALUE))
	{
		return;
	}
	else if(key == BACKSPACE_VALUE)
	{
		text.pop_back();
	}
	else
	{
		text.push_back(static_cast <char> (key));
	}

	this->inputText.setString(text);
}