#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <array>
#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

class Button
{
	private:
	struct ButtonColors
	{
		sf::Color outlineColor;
		sf::Color shapeColor;
		sf::Color textColor;
	};
	
	enum ButtonStates : std::int8_t
	{
		idle,
		hover,
		pressed
	};

	sf::Text text;
	sf::RectangleShape shape;
	std::array <ButtonColors, 3> buttonColors;
	ButtonStates buttonState;
	bool mouseClickedState;
	void centerText();
	void updateColors();

	public:
	Button(const sf::Vector2f & size, const sf::Vector2f & position,
	const float & outlineThickness, const sf::Color & idleOutlineColor,
	const sf::Color & idleShapeColor, const sf::Color & idleTextColor,
	sf::Font & font, const char * textString, const std::uint32_t & fontSize);
	const bool isPressed();
	void render(sf::RenderTarget & window);
	void update(sf::Vector2f & mousePosition);
};

#endif