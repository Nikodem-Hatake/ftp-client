#include <SFML/Network/IpAddress.hpp>
#include "../gui/gui.hpp"
#include "ConnectedState.hpp"
#include "ConnectState.hpp"

void ConnectState::checkIfButtonsWerePressed()
{
	if(this->buttons[ButtonsNames::connect].isPressed())
	{
		std::optional <sf::IpAddress> ipAddress = sf::IpAddress::resolve
		(this->inputBoxes[InputBoxesNames::ip].getInputString().toAnsiString());
		if(ipAddress == std::nullopt)
		{
			this->createErrorMessageBox(std::string("Invalid IP address/Domain name"));
			return;
		}

		this->connectToFtpServer(* ipAddress);
	}
	else if(this->buttons[ButtonsNames::exit].isPressed())
	{
		this->states.pop();
	}
}

void ConnectState::connectToFtpServer(sf::IpAddress & ipAddress)
{
	sf::Ftp::Response response = this->ftpConnection.connect
	(ipAddress, 21, sf::milliseconds(100));
	if(!response.isOk())
	{
		this->createErrorMessageBox("Can't connect");
		return;
	}

	if(this->anonymousLoginCheckBox.isChecked())
	{
		response = this->ftpConnection.login();
	}
	else
	{
		response = this->ftpConnection.login
		(this->inputBoxes[InputBoxesNames::username].getInputString(),
		this->inputBoxes[InputBoxesNames::password].getInputString());
	}

	if(!response.isOk())
	{
		this->createErrorMessageBox("Can't login");
		return;
	}

	this->states.emplace(std::make_unique <ConnectedState>(this->window, this->font,
	this->states, this->keyPressed, this->ftpConnection));
}

ConnectState::ConnectState(sf::RenderWindow & window, sf::Font & font,
std::stack <std::unique_ptr <State>> & states, char32_t & keyPressed)
: State(window, font, states), keyPressed(keyPressed),
anonymousLoginCheckBox(gui::calculatePercentageVector(window.getSize(), {7.f, 10.f}),
gui::calculatePercentageVector(window.getSize(), {10.f, 75.f}),
gui::calculateOutlineThickness(window.getSize(), 1.5f), this->anonymousLoginCheckBoxTexture),
anonymousLoginText(font, "ANONYMOUS LOGIN", gui::calculateFontSize(window.getSize(), 16)),
ipInputBoxText(font, "IP/DOMAIN NAME", gui::calculateFontSize(window.getSize(), 16)),
passwordInputBoxText(font, "PASSWORD", gui::calculateFontSize(window.getSize(), 16)),
usernameInputBoxText(font, "USERNAME", gui::calculateFontSize(window.getSize(), 16))
{
	this->currentlyFocusedInputBoxIndex = -1;
	this->errorMessageBox = nullptr;
	this->backgroundWithBorder.setFillColor(sf::Color(240, 240, 240));
	this->backgroundWithBorder.setOutlineColor(sf::Color(0, 0, 0));
	this->backgroundWithBorder.setOutlineThickness
	(gui::calculateOutlineThickness(this->window.getSize(), 2.f));
	this->backgroundWithBorder.setPosition
	(gui::calculatePercentageVector(this->window.getSize(), {5.f, 5.f}));
	this->backgroundWithBorder.setSize
	(gui::calculatePercentageVector(this->window.getSize(), {90.f, 90.f}));
	
	//Remove compiler warnings.
	void(this->anonymousLoginCheckBoxTexture.loadFromFile("resources/icons/checkbox.png"));
	this->anonymousLoginText.setFillColor(sf::Color(0, 0, 0));
	this->anonymousLoginText.setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {19.f, 76.5f}));

	this->createButtons();
	this->createInputBoxesWithTexts();
}

void ConnectState::createButtons()
{
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 32);
	const float outlineThickness = gui::calculateOutlineThickness(this->window.getSize(), 1.5f);

	this->buttons.reserve(2);
	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {17.5f, 10.f}),
	gui::calculatePercentageVector(this->window.getSize(), {57.5f, 75.f}), outlineThickness,
	sf::Color(48, 48, 48), sf::Color(128, 128, 128), sf::Color(255, 255, 255), this->font,
	"CONNECT", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {10.f, 10.f}),
	gui::calculatePercentageVector(this->window.getSize(), {80.f, 75.f}), outlineThickness,
	sf::Color(48, 48, 48), sf::Color(128, 128, 128), sf::Color(255, 255, 255), this->font,
	"EXIT", fontSize);
}

void ConnectState::createErrorMessageBox(const std::string & message)
{
	this->errorMessageBox = std::make_unique <MessageBox>
	(gui::calculatePercentageVector(this->window.getSize(), {40.f, 30.f}),
	gui::calculatePercentageVector(this->window.getSize(), {30.f, 35.f}),
	gui::calculatePercentageVector(this->window.getSize(), {32.5f, 45.f}),
	gui::calculateOutlineThickness(this->window.getSize(), 2.f),
	sf::Color(48, 48, 48), sf::Color(128, 128, 128), sf::Color(255, 255, 255),
	this->font, std::string("Error: " + message).c_str(),
	gui::calculateFontSize(this->window.getSize(), 16));
}

void ConnectState::createInputBoxesWithTexts()
{
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 16);
	const float outlineThickness = gui::calculateOutlineThickness(this->window.getSize(), 2.f);
	const sf::Vector2f size = gui::calculatePercentageVector(this->window.getSize(), {65.f, 10.f});
	this->inputBoxes.reserve(NUMBER_OF_INPUT_BOXES_IN_CONNECT_STATE);

	this->inputBoxes.emplace_back(size, gui::calculatePercentageVector
	(this->window.getSize(), {10.f, 15.f}), outlineThickness, sf::Color(48, 48, 48),
	sf::Color(128, 128, 128), sf::Color(255, 255, 255), this->font, fontSize, 70);
	this->ipInputBoxText.setFillColor(sf::Color(0, 0, 0));
	this->ipInputBoxText.setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {77.f, 16.f}));

	this->inputBoxes.emplace_back(size, gui::calculatePercentageVector
	(this->window.getSize(), {10.f, 35.f}), outlineThickness, sf::Color(48, 48, 48),
	sf::Color(128, 128, 128), sf::Color(255, 255, 255), this->font, fontSize, 70);
	this->usernameInputBoxText.setFillColor(sf::Color(0, 0, 0));
	this->usernameInputBoxText.setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {77.f, 36.f}));

	this->inputBoxes.emplace_back(size, gui::calculatePercentageVector
	(this->window.getSize(), {10.f, 55.f}), outlineThickness, sf::Color(48, 48, 48),
	sf::Color(128, 128, 128), sf::Color(255, 255, 255), this->font, fontSize, 70);
	this->passwordInputBoxText.setFillColor(sf::Color(0, 0, 0));
	this->passwordInputBoxText.setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {77.f, 56.f}));
}

void ConnectState::render()
{
	this->window.draw(this->backgroundWithBorder);

	this->anonymousLoginCheckBox.render(this->window);
	this->window.draw(this->anonymousLoginText);
	this->renderInputBoxesWithTexts();

	for(Button & button : buttons)
	{
		button.render(this->window);
	}

	if(this->errorMessageBox != nullptr)
	{
		this->errorMessageBox->render(this->window);
	}
}

void ConnectState::renderInputBoxesWithTexts()
{
	this->window.draw(this->ipInputBoxText);
	this->window.draw(this->usernameInputBoxText);
	this->window.draw(this->passwordInputBoxText);
	for(InputBox & inputBox : this->inputBoxes)
	{
		inputBox.render(this->window);
	}
}

void ConnectState::update(sf::Vector2f && mousePosition)
{
	if(this->errorMessageBox != nullptr)
	{
		this->errorMessageBox->update(mousePosition);
		if(this->errorMessageBox->isClosed())
		{
			this->errorMessageBox = nullptr;
		}
		else
		{
			return;
		}
	}

	this->anonymousLoginCheckBox.update(mousePosition);
	this->updateInputBoxes(mousePosition);
	for(Button & button : buttons)
	{
		button.update(mousePosition);
	}
	this->checkIfButtonsWerePressed();
}

void ConnectState::updateInputBoxes(sf::Vector2f & mousePosition)
{
	for(std::int32_t i = 0; i < NUMBER_OF_INPUT_BOXES_IN_CONNECT_STATE; ++i)
	{
		this->inputBoxes[i].update(mousePosition, this->keyPressed);
		if(this->inputBoxes[i].isFocused() && this->currentlyFocusedInputBoxIndex != i)
		{
			if(this->currentlyFocusedInputBoxIndex != -1)
			{
				this->inputBoxes[this->currentlyFocusedInputBoxIndex].removeFocus();
			}
			this->currentlyFocusedInputBoxIndex = i;
		}
	}

	if(this->currentlyFocusedInputBoxIndex != -1 
	&& !this->inputBoxes[this->currentlyFocusedInputBoxIndex].isFocused())
	{
		this->currentlyFocusedInputBoxIndex = -1;
	}
}