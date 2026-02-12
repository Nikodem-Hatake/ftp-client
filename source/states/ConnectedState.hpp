#ifndef CONNECTED_STATE_HPP
#define CONNECTED_STATE_HPP

#define MAX_NUMBER_OF_ELEMENTS_PER_PAGE 11
#define MAX_CHARACTERS_FOR_ELEMENT 40
#define MAX_CHARACTERS_FOR_CURRENT_DIRECTORY 30
#define DIRECTORY_SEPARATOR '\\'

#include <filesystem>
#include <list>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Network/Ftp.hpp>
#include "../gui/CheckBox.hpp"
#include "../gui/InputBox.hpp"
#include "../gui/MessageBox.hpp"
#include "State.hpp"

class ConnectedState : public State
{
	private:
	enum ButtonsNames : std::int8_t
	{
		disconnect,
		ftpPageChangeLeft,
		ftpPageChangeRight,
		localPageChangeLeft,
		localPageChangeRight,
		createElement,
		uploadElement,
		downloadElement,
		deleteElement,
		renameElement,
		changeDirectory,
		comeBackToPreviousFtpDirectory,
		comeBackToPreviousLocalDirectory,
		changeDirectoryToInputed
	};

	enum textsNames : std::int8_t
	{
		currentFtpDirectory,
		currentLocalDirectory,
		currentFtpPageText,
		currentLocalPageText
	};

	struct VisibleElement
	{
		sf::Text text;
		std::list <std::string>::iterator iterator;
		VisibleElement(const sf::Text & text) : text(text), iterator()
		{
			
		}
	};

	std::array <sf::RectangleShape, MAX_NUMBER_OF_ELEMENTS_PER_PAGE> ftpElementsIcons;
	std::array <sf::RectangleShape, MAX_NUMBER_OF_ELEMENTS_PER_PAGE> localElementsIcons;
	std::array <sf::Text, 4> texts;
	InputBox inputBox;
	sf::RectangleShape bottomLine;
	sf::RectangleShape middleLine;
	sf::RectangleShape selectedElementBackground;
	sf::Texture fileIcon;
	sf::Texture folderIcon;
	std::vector <Button> buttons;
	std::list <std::string> ftpElements;
	std::list <std::string> localElements;
	std::vector <VisibleElement> ftpVisibleElements;
	std::vector <VisibleElement> localVisibleElements;
	std::list <std::string>::iterator ftpElementsIterator;
	std::list <std::string>::iterator ftpElementsIteratorEnd;
	std::list <std::string>::iterator localElementsIterator;
	std::list <std::string>::iterator localElementsIteratorEnd;
	std::vector <VisibleElement>::iterator selectedElementIterator;
	std::unique_ptr <MessageBox> errorMessageBox;
	sf::Ftp & ftpConenction;
	char32_t & keyPressed;
	std::uint32_t ftpElementsPageNumber;
	std::uint32_t localElementsPageNumber;
	bool isSelectedElementAnFtpElement;
	void changeCurrentDirectory();
	void changeCurrentDirectoryToInputedOne();
	void changeFtpPageToLeft();
	void changeFtpPageToRight();
	void changeLocalPageToLeft();
	void changeLocalPageToRight();
	void checkIfButtonsWerePressed();
	const bool checkIfElementIsFtpFolder(const std::string & element);
	void checkIfElementIsSelected(sf::Vector2f & mousePosition);
	void comeBackToFtpParentDirectory();
	void comeBackToLocalParentDirectory();
	void createButtons();
	void createErrorMessageBox(const std::string & message);
	void createLines();
	void createNewDirectory();
	void createTexts();
	void createVisibleTexts();
	void deleteSelectedElement();
	void downloadSelectedElement();
	void getFtpElements();
	void getLocalElements();
	void renameSelectedElement();
	void renderButtons();
	void renderFtpElements();
	void renderLocalElements();
	void renderTexts();
	void resetSelectedElementIterator();
	void setCurrentFtpPageText();
	void setCurrentLocalPageText();
	void setFtpVisibleElementsIconsAndPositions();
	void setIconsPositionsAndSizes();
	void setLocalVisibleElementsIconsAndPositions();
	void uploadSelectedElementToFtpServer();

	public:
	ConnectedState(sf::RenderWindow & window, sf::Font & font,
	std::stack <std::unique_ptr <State>> & states,
	char32_t & keyPressedRef, sf::Ftp & ftpConnectionRef);
	void render() override;
	void update(sf::Vector2f && mousePosition) override;
};

#endif