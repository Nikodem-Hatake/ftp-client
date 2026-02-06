#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <stack>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class State
{
	protected:
	sf::RenderWindow & window;
	sf::Font & font;
	std::stack <std::unique_ptr <State>> & states;

	public:
	virtual void render();
	State(sf::RenderWindow & window, sf::Font & font,
	std::stack <std::unique_ptr <State>> & states);
	virtual void update(sf::Vector2f && mousePosition);
};

#endif