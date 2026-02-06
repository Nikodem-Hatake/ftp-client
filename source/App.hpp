#ifndef APP_HPP
#define APP_HPP

#include <SFML/GpuPreference.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "states/State.hpp"

class App
{
	private:
	sf::RenderWindow window;
	sf::Font font;
	std::stack <std::unique_ptr <State>> states;
	char32_t keyPressed;
	void updateSFMLEvents();

	public:
	App();
	void run();
};

#endif