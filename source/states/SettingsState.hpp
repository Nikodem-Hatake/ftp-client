#ifndef SETTINGS_STATE_HPP
#define SETTINGS_STATE_HPP

#include "../gui/Button.hpp"
#include "../gui/DropdownList.hpp"
#include "State.hpp"

class SettingsState : public State
{
	private:
	std::unique_ptr <DropdownList> resolutionOptions;
	std::unique_ptr <Button> exitButton;
	void changeResolution();
	void createGui();

	public:
	void render() override;
	SettingsState(sf::RenderWindow & window, sf::Font & font,
	std::stack <std::unique_ptr <State>> & states);
	void update(sf::Vector2f && mousePosition) override;
};

#endif