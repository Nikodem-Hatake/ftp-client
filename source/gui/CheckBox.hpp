#ifndef CHECK_BOX_HPP
#define CHECK_BOX_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

class CheckBox
{
	private:
	sf::RectangleShape shape;
	sf::Texture & texture;
	bool isBeingChecked;
	bool mouseClickedState;

	public:
	CheckBox(const sf::Vector2f & size, const sf::Vector2f & position, 
	const float & outlineThickness, sf::Texture & texture);
	const bool isChecked();
	void render(sf::RenderTarget & window);
	void update(sf::Vector2f & mousePosition);
};

#endif