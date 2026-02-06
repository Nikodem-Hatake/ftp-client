#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

class InputBox
{
	private:
	sf::Text inputText;
	sf::RectangleShape shape;
	std::uint32_t charactersLimit;
	bool hasFocus;
	bool mouseClickedState;
	void updateFocus();
	void updateInputText(char32_t & key);

	public:
	sf::String getInputString();
	InputBox(const sf::Vector2f & size, const sf::Vector2f & position, 
	const float & outlineThickness, const sf::Color & idleOutlineColor, 
	const sf::Color & idleShapeColor, const sf::Color & idleTextColor,
	sf::Font & font, const std::uint32_t & fontSize, const std::uint32_t & charactersLimit);
	const bool isFocused();
	void removeFocus();
	void render(sf::RenderTarget & window);
	void update(sf::Vector2f & mousePosition, char32_t & keyPressed);
};

#endif