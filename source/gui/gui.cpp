#include "gui.hpp"

std::uint32_t gui::calculateFontSize(const sf::Vector2u & windowSize, const std::uint32_t & fontSize)
{
	return windowSize.x / DEFAULT_WINDOW_WIDTH * fontSize;
}

float gui::calculateOutlineThickness(const sf::Vector2u & windowSize, const float & outlineThickness)
{
	return windowSize.x / DEFAULT_WINDOW_WIDTH * outlineThickness;
}

sf::Vector2f gui::calculatePercentageVector(const sf::Vector2u & windowSize, const sf::Vector2f & percents)
{
	return sf::Vector2f(windowSize.x / 100.f * percents.x,
	windowSize.y / 100.f * percents.y);
}