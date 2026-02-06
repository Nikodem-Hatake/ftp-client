#include <SFML/Window/Mouse.hpp>
#include "CheckBox.hpp"

CheckBox::CheckBox(const sf::Vector2f & size, const sf::Vector2f & position,
const float & outlineThickness, sf::Texture & texture) : texture(texture)
{
	this->isBeingChecked = false;
	this->mouseClickedState = false;
	this->shape.setOutlineColor(sf::Color(0, 0, 0));
	this->shape.setOutlineThickness(outlineThickness);
	this->shape.setPosition(position);
	this->shape.setSize(size);
}

const bool CheckBox::isChecked()
{
	return this->isBeingChecked;
}

void CheckBox::render(sf::RenderTarget & window)
{
	window.draw(this->shape);
}

void CheckBox::update(sf::Vector2f & mousePosition)
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
			if(this->isBeingChecked)
			{
				this->isBeingChecked = false;
				this->shape.setTexture(nullptr);
			}
			else
			{
				this->isBeingChecked = true;
				this->shape.setTexture(& this->texture);
			}
		}
	}
}