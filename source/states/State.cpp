#include "State.hpp"

void State::render()
{

}

State::State(sf::RenderWindow & window, sf::Font & font,
std::stack <std::unique_ptr <State>> & states) 
: window(window), font(font), states(states)
{

}

void State::update(sf::Vector2f && mousePosition)
{
	
}