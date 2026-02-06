#include "DropdownList.hpp"

DropdownList::DropdownList(const sf::Vector2f & size, sf::Vector2f position,
const float & outlineThickness, const sf::Color & idleOutlineColor,
const sf::Color & idleShapeColor, const sf::Color & idleTextColor,
sf::Font & font, const std::vector <const char *> & elementsTextStrings,
const char * textString, const std::uint32_t & fontSize)
: displayButton(size, position, outlineThickness, idleOutlineColor, 
idleShapeColor, idleTextColor, font, textString, fontSize)
{
	this->isDisplayed = false;
	this->selectedElement = 0;

	this->elements.reserve(elementsTextStrings.size());
	for(const char * elementTextString : elementsTextStrings)
	{
		position.y += size.y + outlineThickness;
		this->elements.emplace_back(size, position, outlineThickness, idleOutlineColor,
		idleShapeColor, idleTextColor, font, elementTextString, fontSize);
	}
}

void DropdownList::render(sf::RenderTarget & window)
{
	this->displayButton.render(window);
	if(this->isDisplayed)
	{
		for(Button & element : this->elements)
		{
			element.render(window);
		}
	}
}

std::uint32_t DropdownList::getSelectedElement()
{
	return this->selectedElement;
}

void DropdownList::update(sf::Vector2f & mousePosition)
{
	this->displayButton.update(mousePosition);
	if(this->displayButton.isPressed())
	{
		if(this->isDisplayed)
		{
			this->isDisplayed = false;
		}
		else
		{
			this->isDisplayed = true;
		}
	}

	if(this->isDisplayed)
	{
		std::uint32_t numberOfElements = static_cast <std::uint32_t>(this->elements.size());
		for(std::uint32_t i = 0; i < numberOfElements; ++i)
		{
			this->elements[i].update(mousePosition);
			if(this->elements[i].isPressed())
			{
				this->selectedElement = i + 1;
				return;
			}
		}
		this->selectedElement = 0;
	}
}