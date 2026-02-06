#ifndef DROPDOWN_LIST_HPP
#define DROPDOWN_LIST_HPP

#include <vector>
#include "../gui/Button.hpp"

class DropdownList
{
	private:
	Button displayButton;
	std::vector <Button> elements;
	std::uint32_t selectedElement;
	bool isDisplayed;

	public:
	DropdownList(const sf::Vector2f & size, sf::Vector2f position, 
	const float & outlineThickness, const sf::Color & idleOutlineColor, 
	const sf::Color & idleShapeColor, const sf::Color & idleTextColor, 
	sf::Font & font, const std::vector <const char *> & elementsTextStrings, 
	const char * textString, const std::uint32_t & fontSize);
	std::uint32_t getSelectedElement();
	void render(sf::RenderTarget & window);
	void update(sf::Vector2f & mousePosition);
};

#endif