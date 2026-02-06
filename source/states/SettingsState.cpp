#include "../gui/gui.hpp"
#include "SettingsState.hpp"

void SettingsState::changeResolution()
{
	switch(this->resolutionOptions->getSelectedElement())
	{
		case 1:
		{
			this->window.create(sf::VideoMode({DEFAULT_WINDOW_WIDTH,
			DEFAULT_WINDOW_HEIGHT}), "FTP CLIENT", sf::Style::Close);
			break;
		}
		case 2:
		{
			this->window.create(sf::VideoMode({1280, 720}), "FTP CLIENT", sf::Style::Close);
			break;
		}
		case 3:
		{
			this->window.create(sf::VideoMode({DEFAULT_WINDOW_WIDTH,
			DEFAULT_WINDOW_HEIGHT}), "FTP CLIENT", sf::State::Fullscreen);
			break;
		}
	}

	this->exitButton = nullptr;
	this->resolutionOptions = nullptr;
	this->createGui();
}

void SettingsState::createGui()
{
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 32);
	const float outlineThickness = gui::calculateOutlineThickness(this->window.getSize(), 2.f);

	this->exitButton = std::make_unique <Button>
	(gui::calculatePercentageVector(this->window.getSize(), {20.f, 20.f}),
	gui::calculatePercentageVector(this->window.getSize(), {75.f, 10.f}),
	outlineThickness, sf::Color(48, 48, 48), sf::Color(128, 128, 128),
	sf::Color(255, 255, 255), this->font, "EXIT", fontSize);

	this->resolutionOptions = std::make_unique <DropdownList>
	(gui::calculatePercentageVector(this->window.getSize(), {40.f, 20.f}),
	gui::calculatePercentageVector(this->window.getSize(), {30.f, 10.f}),
	outlineThickness, sf::Color(48, 48, 48), sf::Color(128, 128, 128),
	sf::Color(255, 255, 255),this->font, std::vector <const char *>
	{"640x360", "1280x720", "FULL SCREEN"}, "CHANGE RESOLUTION", fontSize);
}

void SettingsState::render()
{
	this->exitButton->render(this->window);
	this->resolutionOptions->render(this->window);
}

SettingsState::SettingsState(sf::RenderWindow & window, sf::Font & font,
std::stack <std::unique_ptr <State>> & states) : State(window, font, states)
{
	this->createGui();
}

void SettingsState::update(sf::Vector2f && mousePosition)
{
	this->exitButton->update(mousePosition);
	if(this->exitButton->isPressed())
	{
		this->states.pop();
		return;
	}
	
	this->resolutionOptions->update(mousePosition);
	if(this->resolutionOptions->getSelectedElement())
	{
		this->changeResolution();
	}
}