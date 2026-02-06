#ifndef CONNECT_STATE_HPP
#define CONNECT_STATE_HPP

#include <vector>
#include <SFML/Network/Ftp.hpp>
#include "../gui/CheckBox.hpp"
#include "../gui/InputBox.hpp"
#include "../gui/MessageBox.hpp"
#include "State.hpp"

#define NUMBER_OF_INPUT_BOXES_IN_CONNECT_STATE 3

class ConnectState : public State
{
	private:
	enum ButtonsNames : std::int8_t
	{
		connect,
		exit
	};
	enum InputBoxesNames : std::int8_t
	{
		ip,
		username,
		password
	};

	sf::Text anonymousLoginText;
	sf::Text ipInputBoxText;
	sf::Text passwordInputBoxText;
	sf::Text usernameInputBoxText;
	CheckBox anonymousLoginCheckBox;
	sf::RectangleShape backgroundWithBorder;
	sf::Ftp ftpConnection;
	sf::Texture anonymousLoginCheckBoxTexture;
	std::vector <InputBox> inputBoxes;
	std::vector <Button> buttons;
	std::unique_ptr <MessageBox> errorMessageBox;
	char32_t & keyPressed;
	std::int32_t currentlyFocusedInputBoxIndex;
	void checkIfButtonsWerePressed();
	void connectToFtpServer(sf::IpAddress & ipAddress);
	void createButtons();
	void createErrorMessageBox(const std::string & message);
	void createInputBoxesWithTexts();
	void renderInputBoxesWithTexts();
	void updateInputBoxes(sf::Vector2f & mousePosition);

	public:
	ConnectState(sf::RenderWindow & window, sf::Font & font,
	std::stack <std::unique_ptr <State>> & states, char32_t & keyPressed);
	void render() override;
	void update(sf::Vector2f && mousePosition) override;
};

#endif