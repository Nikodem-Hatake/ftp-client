#include "../gui/gui.hpp"
#include "ConnectState.hpp"
#include "MainMenuState.hpp"
#include "SettingsState.hpp"

void MainMenuState::checkIfButtonsWerePressed()
{
	if(this->buttons[ButtonsNames::connect]->isPressed())
	{
		this->states.emplace(std::make_unique <ConnectState>
		(this->window, this->font, this->states, this->keyPressed));
	}
	else if(this->buttons[ButtonsNames::settings]->isPressed())
	{
		for(std::size_t i = 0; i < this->buttons.size(); ++i)
		{
			this->buttons[i] = nullptr;
		}
		this->states.emplace(std::make_unique <SettingsState>
		(this->window, this->font, this->states));
	}
	else if(this->buttons[ButtonsNames::exit]->isPressed())
	{
		this->states.pop();
	}
}

void MainMenuState::createButtons()
{
	const sf::Vector2f buttonSize = gui::calculatePercentageVector(this->window.getSize(), {40.f, 20.f});
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 32);
	const float outlineThickness = gui::calculateOutlineThickness(this->window.getSize(), 2.f);

	this->buttons[ButtonsNames::connect] = std::make_unique <Button>
	(buttonSize, gui::calculatePercentageVector(this->window.getSize(), {30.f, 10.f}),
	outlineThickness, sf::Color(48, 48, 48), sf::Color(128, 128, 128),
	sf::Color(255, 255, 255), this->font, "CONNECT", fontSize);

	this->buttons[ButtonsNames::settings] = std::make_unique <Button>
	(buttonSize, gui::calculatePercentageVector(this->window.getSize(), {30.f, 40.f}),
	outlineThickness, sf::Color(48, 48, 48), sf::Color(128, 128, 128),
	sf::Color(255, 255, 255), this->font, "SETTINGS", fontSize);

	this->buttons[ButtonsNames::exit] = std::make_unique <Button>
	(buttonSize, gui::calculatePercentageVector(this->window.getSize(), {30.f, 70.f}),
	outlineThickness, sf::Color(48, 48, 48), sf::Color(128, 128, 128),
	sf::Color(255, 255, 255), this->font, "EXIT", fontSize);
}

MainMenuState::MainMenuState(sf::RenderWindow & window, sf::Font & font,
std::stack <std::unique_ptr <State>> & states, char32_t & keyPressed) 
: State(window, font, states), keyPressed(keyPressed)
{
	this->createButtons();
}

void MainMenuState::render()
{
	if(this->buttons[0] == nullptr)	//When returning from SettingsState.
	{
		this->createButtons();
	}

	for(std::unique_ptr <Button> & button : this->buttons)
	{
		button->render(this->window);
	}
}

void MainMenuState::update(sf::Vector2f && mousePosition)
{
	for(std::unique_ptr <Button> & button : this->buttons)
	{
		button->update(mousePosition);
	}
	this->checkIfButtonsWerePressed();
}