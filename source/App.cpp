#include "App.hpp"
#include "gui/gui.hpp"
#include "states/MainMenuState.hpp"

App::App() : window(sf::VideoMode({DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT}),
"FTP CLIENT", sf::Style::Close), keyPressed(U'\0'), font("resources/font.ttf")
{
	this->window.setVerticalSyncEnabled(true);
	this->states.emplace(std::make_unique <MainMenuState>
	(this->window, this->font, this->states, this->keyPressed));
}

void App::run()
{
	while(this->window.isOpen())
	{
		this->window.clear(sf::Color(240, 240, 240));

		this->updateSFMLEvents();
		this->states.top()->update(static_cast <sf::Vector2f> 
		(sf::Mouse::getPosition(this->window)));
		if(this->states.empty())
		{
			this->window.close();
			return;
		}

		this->states.top()->render();
		this->window.display();
	}
}

void App::updateSFMLEvents()
{
	this->keyPressed = U'\0';
	while(const std::optional <sf::Event> event = this->window.pollEvent())
	{
		if(event->is <sf::Event::Closed>())
		{
			this->window.close();
		}
		else if(const auto * keyPressedEvent = event->getIf <sf::Event::TextEntered>())
		{
			this->keyPressed = keyPressedEvent->unicode;
		}
	}
}