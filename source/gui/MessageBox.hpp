#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include "Button.hpp"

class MessageBox
{
	private:
	Button closeButton;
	sf::Text message;
	sf::RectangleShape backgroundWithBorder;

	public:
	const bool isClosed();
	MessageBox(const sf::Vector2f & size, const sf::Vector2f & position, 
	const sf::Vector2f & messageTextPosition, const float & outlineThickness, 
	const sf::Color & outlineColor, const sf::Color & backgroundColor,
	const sf::Color & messageColor, sf::Font & font, 
	const char * messageString, const std::uint32_t & fontSize);
	void render(sf::RenderTarget & window);
	void update(sf::Vector2f & mousePosition);
};

#endif