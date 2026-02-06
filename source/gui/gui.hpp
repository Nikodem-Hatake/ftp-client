#ifndef GUI_HPP
#define GUI_HPP

#define DEFAULT_WINDOW_HEIGHT 360
#define DEFAULT_WINDOW_WIDTH 640

#include <cstdint>
#include <SFML/System/Vector2.hpp>

namespace gui
{
	std::uint32_t calculateFontSize(const sf::Vector2u & windowSize, const std::uint32_t & fontSize);
	float calculateOutlineThickness(const sf::Vector2u & windowSize, const float & outlineThickness);
	sf::Vector2f calculatePercentageVector(const sf::Vector2u & windowSize, const sf::Vector2f & percents);
}

#endif