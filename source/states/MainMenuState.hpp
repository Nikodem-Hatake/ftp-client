#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "../gui/Button.hpp"
#include "State.hpp"

class MainMenuState : public State
{
	private:
	enum ButtonsNames : std::int8_t
	{
		connect,
		settings,
		exit
	};

	std::array <std::unique_ptr <Button>, 3> buttons;
	char32_t & keyPressed;
	void checkIfButtonsWerePressed();
	void createButtons();

	public:
	MainMenuState(sf::RenderWindow & window, sf::Font & font,
	std::stack <std::unique_ptr <State>> & states, char32_t & keyPressed);
	void render() override;
	void update(sf::Vector2f && mousePosition) override;
};

#endif