#include "gui.hpp"
#include "MessageBox.hpp"

const bool MessageBox::isClosed()
{
	return this->closeButton.isPressed();
}

MessageBox::MessageBox(const sf::Vector2f & size, const sf::Vector2f & position,
const sf::Vector2f & messageTextPosition, const float & outlineThickness,
const sf::Color & outlineColor, const sf::Color & backgroundColor,
const sf::Color & messageColor, sf::Font & font,
const char * messageString, const std::uint32_t & fontSize)
: closeButton(size * 0.10f, {position.x + size.x - size.x * 0.10f,
position.y}, outlineThickness, outlineColor, backgroundColor,
messageColor, font, "X", fontSize / 2U),
message(font, messageString, fontSize)
{
	this->backgroundWithBorder.setFillColor(backgroundColor);
	this->backgroundWithBorder.setOutlineColor(outlineColor);
	this->backgroundWithBorder.setOutlineThickness(outlineThickness);
	this->backgroundWithBorder.setPosition(position);
	this->backgroundWithBorder.setSize(size);

	this->message.setFillColor(messageColor);
	this->message.setPosition(messageTextPosition);
}

void MessageBox::render(sf::RenderTarget & window)
{
	window.draw(this->backgroundWithBorder);
	this->closeButton.render(window);
	window.draw(this->message);
}

void MessageBox::update(sf::Vector2f & mousePosition)
{
	this->closeButton.update(mousePosition);
}