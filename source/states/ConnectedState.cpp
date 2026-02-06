#include <cmath>
#include <fstream>
#include <string>
#include "../gui/gui.hpp"
#include "ConnectedState.hpp"

#define DEBUG_VERSION false

#if DEBUG_VERSION
#include <iostream>
#endif

void ConnectedState::changeCurrentDirectory()
{
	if(this->selectedElementIterator == this->ftpElementsIteratorEnd)
	{
		this->createErrorMessageBox("Not selected any directory to enter");
		return;
	}

	if(this->isSelectedElementAnFtpElement)
	{
		if(!this->checkIfElementIsFtpFolder(this->selectedElementIterator->getString()))
		{
			this->createErrorMessageBox("You can only enter folders");
			return;
		}

		sf::Ftp::Response response = this->ftpConenction.changeDirectory
		(this->selectedElementIterator->getString());
		if(!response.isOk())
		{
#if DEBUG_VERSION
			std::cout << response.getMessage() << '\n';
#endif
			this->createErrorMessageBox("Can't change directory");
			return;
		}
		this->getFtpElements();
		this->setFtpElementsIconsAndPositions();
	}
	else
	{
		std::string newDirectoryWithSeparator(DIRECTORY_SEPARATOR 
		+ this->selectedElementIterator->getString());
		std::filesystem::path newPath(std::filesystem::current_path().string() 
		+ newDirectoryWithSeparator);
		if(!std::filesystem::is_directory(newPath))
		{
			this->createErrorMessageBox("You can only enter folders");
			return;
		}
		else if(!std::filesystem::exists(newPath))
		{
			this->createErrorMessageBox("This directory doesn't exists");
			return;
		}

		std::filesystem::current_path(newPath);
		this->getLocalElements();
		this->setLocalElementsIconsAndPositions();
	}
	this->resetSelectedElementIterator();
}

void ConnectedState::changeCurrentDirectoryToInputedOne()
{
	std::filesystem::path newPath(this->inputBox.getInputString());
	if(newPath.empty())
	{
		this->createErrorMessageBox("Directory input is empty");
		return;
	}
	else if(!std::filesystem::exists(newPath))
	{
		this->createErrorMessageBox("This path doesn't exists: " + newPath.string());
		return;
	}

	std::filesystem::current_path(newPath);
	this->getLocalElements();
	this->setLocalElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::changeFtpPageToLeft()
{
	if(this->ftpElementsPageNumber == 1)
	{
		return;
	}

	--this->ftpElementsPageNumber;
	this->setCurrentFtpPageText();

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE; ++i)
	{
		--this->ftpElementsIterator;
	}
	this->setFtpElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::changeFtpPageToRight()
{
	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->ftpElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(this->ftpElementsPageNumber == numberOfPages)
	{
		return;
	}

	++this->ftpElementsPageNumber;
	this->setCurrentFtpPageText();

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE; ++i)
	{
		++this->ftpElementsIterator;
	}
	this->setFtpElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::changeLocalPageToLeft()
{
	if(this->localElementsPageNumber == 1)
	{
		return;
	}

	--this->localElementsPageNumber;
	this->setCurrentLocalPageText();

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE; ++i)
	{
		--this->localElementsIterator;
	}
	this->setLocalElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::changeLocalPageToRight()
{
	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil(static_cast <double>
	(this->localElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(this->localElementsPageNumber == numberOfPages)
	{
		return;
	}

	++this->localElementsPageNumber;
	this->setCurrentLocalPageText();

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE; ++i)
	{
		++this->localElementsIterator;
	}
	this->setLocalElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::checkIfButtonsWerePressed()
{
	if(this->buttons[ButtonsNames::changeDirectory].isPressed())
	{
		this->changeCurrentDirectory();
	}
	else if(this->buttons[ButtonsNames::changeDirectoryToInputed].isPressed())
	{
		this->changeCurrentDirectoryToInputedOne();
	}
	else if(this->buttons[ButtonsNames::comeBackToPreviousFtpDirectory].isPressed())
	{
		this->comeBackToFtpParentDirectory();
	}
	else if(this->buttons[ButtonsNames::comeBackToPreviousLocalDirectory].isPressed())
	{
		this->comeBackToLocalParentDirectory();
	}
	else if(this->buttons[ButtonsNames::createElement].isPressed())
	{
		this->createNewDirectory();
	}
	else if(this->buttons[ButtonsNames::deleteElement].isPressed())
	{
		this->deleteSelectedElement();
	}
	else if(this->buttons[ButtonsNames::disconnect].isPressed())
	{
		void(this->ftpConenction.disconnect());
		this->states.pop();
	}
	else if(this->buttons[ButtonsNames::downloadElement].isPressed())
	{
		this->downloadSelectedElement();
	}
	else if(this->buttons[ButtonsNames::ftpPageChangeLeft].isPressed())
	{
		this->changeFtpPageToLeft();
	}
	else if(this->buttons[ButtonsNames::ftpPageChangeRight].isPressed())
	{
		this->changeFtpPageToRight();
	}
	else if(this->buttons[ButtonsNames::localPageChangeLeft].isPressed())
	{
		this->changeLocalPageToLeft();
	}
	else if(this->buttons[ButtonsNames::localPageChangeRight].isPressed())
	{
		this->changeLocalPageToRight();
	}
	else if(this->buttons[ButtonsNames::renameElement].isPressed())
	{
		this->renameSelectedElement();
	}
	else if(this->buttons[ButtonsNames::uploadElement].isPressed())
	{
		this->uploadSelectedElementToFtpServer();
	}
}

const bool ConnectedState::checkIfElementIsFtpFolder(const std::string & element)
{
	if(this->ftpConenction.changeDirectory(element).isOk())
	{
		void(this->ftpConenction.parentDirectory());
		return true;
	}
	return false;
}

void ConnectedState::checkIfElementIsSelected(sf::Vector2f & mousePosition)
{
	std::list <sf::Text>::iterator it;
	sf::Vector2f previousPosition = this->selectedElementBackground.getPosition();

	for(it = this->localElementsIterator; it != this->localElementsIteratorEnd; ++it)
	{
		this->selectedElementBackground.setPosition(it->getPosition());
		if(this->selectedElementBackground.getGlobalBounds().contains(mousePosition))
		{
			this->isSelectedElementAnFtpElement = false;
			this->selectedElementIterator = it;
			return;
		}
	}

	for(it = this->ftpElementsIterator; it != this->ftpElementsIteratorEnd; ++it)
	{
		this->selectedElementBackground.setPosition(it->getPosition());
		if(this->selectedElementBackground.getGlobalBounds().contains(mousePosition))
		{
			this->isSelectedElementAnFtpElement = true;
			this->selectedElementIterator = it;
			return;
		}
	}
	this->selectedElementBackground.setPosition(previousPosition);
}

ConnectedState::ConnectedState(sf::RenderWindow & window, sf::Font & font,
std::stack <std::unique_ptr <State>> & states, char32_t & keyPressedRef, sf::Ftp & ftpConnectionRef)
: State(window, font, states), ftpConenction(ftpConnectionRef), keyPressed(keyPressedRef),
texts(font, font, font, font),
inputBox(gui::calculatePercentageVector(window.getSize(), {38.f, 10.f}),
gui::calculatePercentageVector(window.getSize(), {3.f, 85.f}),
gui::calculateOutlineThickness(window.getSize(), 1.f), sf::Color(48, 48, 48),
sf::Color(128, 128, 128), sf::Color(255, 255, 255), font, 
gui::calculateFontSize(window.getSize(), 16), 40)
{
	//Remove compiler warnings.
	void(this->fileIcon.loadFromFile("resources/icons/file.png"));
	void(this->folderIcon.loadFromFile("resources/icons/folder.png"));
	this->selectedElementBackground.setFillColor(sf::Color(128, 128, 128, 128));
	this->selectedElementBackground.setSize(gui::calculatePercentageVector
	(this->window.getSize(), {44.f, 5.f}));

	this->createButtons();
	this->createLines();
	this->createTexts();

	this->getFtpElements();
	this->setFtpElementsIconsAndPositions();

	this->getLocalElements();
	this->setLocalElementsIconsAndPositions();
	this->setIconsPositionsAndSizes();
}

void ConnectedState::comeBackToFtpParentDirectory()
{
	sf::Ftp::Response response = this->ftpConenction.parentDirectory();
	if(!response.isOk())
	{
#if DEBUG_VERSION
		std::cout << response.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't change directory");
		return;
	}
	this->getFtpElements();
	this->setFtpElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::comeBackToLocalParentDirectory()
{
	std::filesystem::path newPath(std::filesystem::current_path().parent_path());
	if(!std::filesystem::exists(newPath))
	{
		this->createErrorMessageBox("Can't change directory to the parent, it doesn't exists");
		return;
	}

	std::filesystem::current_path(newPath);
	this->getLocalElements();
	this->setLocalElementsIconsAndPositions();
	this->resetSelectedElementIterator();
}

void ConnectedState::createButtons()
{
	this->buttons.reserve(14);
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 16);
	const float outlineThickness = gui::calculateOutlineThickness(this->window.getSize(), 1.5f);
	const sf::Vector2f pageChangeButtonsSize = gui::calculatePercentageVector
	(this->window.getSize(), {4.f, 4.f});
	const sf::Color idleOutlineColor(48, 48, 48);
	const sf::Color idleShapeColor(128, 128, 128);
	const sf::Color idleTextColor(255, 255, 255);
	const sf::Color idleComeBackButtonsShapeColor(240, 240, 240);

	//Buttons are added in order of their enum names (ButtonsNames)
	//(example, first button added is disconnect button because it's the first entry in enum).
	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {10.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {87.f, 91.25f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "DISCONNECT", fontSize);

	this->buttons.emplace_back(pageChangeButtonsSize,
	gui::calculatePercentageVector(this->window.getSize(), {85.f, 1.f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "<", fontSize);

	this->buttons.emplace_back(pageChangeButtonsSize,
	gui::calculatePercentageVector(this->window.getSize(), {95.f, 1.f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, ">", fontSize);

	this->buttons.emplace_back(pageChangeButtonsSize,
	gui::calculatePercentageVector(this->window.getSize(), {35.f, 1.f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "<", fontSize);

	this->buttons.emplace_back(pageChangeButtonsSize,
	gui::calculatePercentageVector(this->window.getSize(), {45.f, 1.f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, ">", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {17.5f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {68.f, 83.75f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "CREATE FOLDER", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {7.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {68.f, 91.25f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "UPLOAD", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {9.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {76.5f, 91.25f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "DOWNLOAD", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {10.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {87.f, 83.75f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "DELETE", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {7.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {59.5f, 83.75f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "RENAME", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {7.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {59.5f, 91.25f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor, this->font, "ENTER", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {40.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {51.f, 5.f}), 0.f,
	idleOutlineColor, idleComeBackButtonsShapeColor, idleOutlineColor, this->font, "...", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {40.f, 5.f}),
	gui::calculatePercentageVector(this->window.getSize(), {0.f, 5.f}), 0.f,
	idleOutlineColor, idleComeBackButtonsShapeColor, idleOutlineColor, this->font, "...", fontSize);

	this->buttons.emplace_back(gui::calculatePercentageVector(this->window.getSize(), {15.f, 12.5f}),
	gui::calculatePercentageVector(this->window.getSize(), {43.f, 83.75f}), outlineThickness,
	idleOutlineColor, idleShapeColor, idleTextColor,
	this->font, "CHANGE CURRENT\nLOCAL DIRECTORY\nTO INPUTED ONE", fontSize);
}

void ConnectedState::createErrorMessageBox(const std::string & message)
{
	std::string errorMessage("Error: " + message);

	//Putting new line and '-' once every 40 characters.
	for(std::size_t i = 40; i < errorMessage.length(); i += 40)
	{
		errorMessage.insert(i + (i / 40), "-\n");
	}

	this->errorMessageBox = std::make_unique <MessageBox>
	(gui::calculatePercentageVector(this->window.getSize(), {40.f, 30.f}),
	gui::calculatePercentageVector(this->window.getSize(), {30.f, 35.f}),
	gui::calculatePercentageVector(this->window.getSize(), {32.5f, 45.f}),
	gui::calculateOutlineThickness(this->window.getSize(), 2.f),
	sf::Color(48, 48, 48), sf::Color(128, 128, 128), sf::Color(255, 255, 255),
	this->font, errorMessage.c_str(), gui::calculateFontSize(this->window.getSize(), 16));
}

void ConnectedState::createLines()
{
	this->bottomLine.setFillColor(sf::Color(0, 0, 0));
	this->bottomLine.setSize(gui::calculatePercentageVector(this->window.getSize(), {100.f, 2.f}));
	this->bottomLine.setPosition(gui::calculatePercentageVector(this->window.getSize(), {0.f, 80.f}));
	
	this->middleLine.setFillColor(sf::Color(0, 0, 0));
	this->middleLine.setSize({this->bottomLine.getSize().y, this->bottomLine.getPosition().y});
	this->middleLine.setPosition({this->bottomLine.getSize().x / 2.f 
	- this->middleLine.getSize().x / 2.f, 0.f});
}

void ConnectedState::createNewDirectory()
{
	std::string directoryName = this->inputBox.getInputString();
	if(directoryName.empty())
	{
		this->createErrorMessageBox("New name input is empty");
		return;
	}

	sf::Ftp::Response response = this->ftpConenction.createDirectory(directoryName);
	if(!response.isOk())
	{
#if DEBUG_VERSION
		std::cout << response.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't create directory");
		return;
	}
	
	this->ftpElements.emplace_back(this->font, directoryName,
	gui::calculateFontSize(this->window.getSize(), 16));
	this->ftpElements.back().setFillColor(sf::Color(0, 0, 0));
	this->setFtpElementsIconsAndPositions();
	this->setCurrentFtpPageText();
}

void ConnectedState::createTexts()
{
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 16);
	this->texts[textsNames::currentFtpDirectory].setCharacterSize(fontSize);
	this->texts[textsNames::currentFtpDirectory].setFillColor(sf::Color(0, 0, 0));
	this->texts[textsNames::currentFtpDirectory].setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {55.f, 0.f}));
	this->texts[textsNames::currentFtpPageText].setCharacterSize(fontSize);
	this->texts[textsNames::currentFtpPageText].setFillColor(sf::Color(0, 0, 0));
	this->texts[textsNames::currentFtpPageText].setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {90.f, 0.f}));

	this->texts[textsNames::currentLocalDirectory].setCharacterSize(fontSize);
	this->texts[textsNames::currentLocalDirectory].setFillColor(sf::Color(0, 0, 0));
	this->texts[textsNames::currentLocalDirectory].setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {5.f, 0.f}));
	this->texts[textsNames::currentLocalPageText].setCharacterSize(fontSize);
	this->texts[textsNames::currentLocalPageText].setFillColor(sf::Color(0, 0, 0));
	this->texts[textsNames::currentLocalPageText].setPosition(gui::calculatePercentageVector
	(this->window.getSize(), {40.f, 0.f}));
}

void ConnectedState::deleteSelectedElement()
{
	if(this->selectedElementIterator == this->ftpElementsIteratorEnd
	|| !this->isSelectedElementAnFtpElement)
	{
		this->createErrorMessageBox("Not selected any ftp file/directory to delete");
		return;
	}

	if(this->checkIfElementIsFtpFolder(this->selectedElementIterator->getString()))
	{
		sf::Ftp::Response response = this->ftpConenction.deleteDirectory
		(this->selectedElementIterator->getString());
		if(!response.isOk())
		{
#if DEBUG_VERSION
			std::cout << response.getMessage() << '\n';
#endif
			this->createErrorMessageBox("Can't delete directory named: " +
			this->selectedElementIterator->getString());
			return;
		}
	}
	else
	{
		sf::Ftp::Response response = this->ftpConenction.deleteFile
		(this->selectedElementIterator->getString().toAnsiString());
		if(!response.isOk())
		{
#if DEBUG_VERSION
			std::cout << response.getMessage() << '\n';
#endif
			this->createErrorMessageBox("Can't delete file named: " +
			this->selectedElementIterator->getString());
			return;
		}
	}

	std::size_t numberOfPagesAfterDeleting = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->ftpElements.size() - 1) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(numberOfPagesAfterDeleting == 0)
	{
		numberOfPagesAfterDeleting = 1;
	}

	if(this->ftpElementsPageNumber > numberOfPagesAfterDeleting)
	{
		this->changeFtpPageToLeft();
	}

	this->ftpElements.erase(selectedElementIterator);
	this->setFtpElementsIconsAndPositions();
	this->setCurrentFtpPageText();
	this->resetSelectedElementIterator();
}

void ConnectedState::downloadSelectedElement()
{
	if(this->selectedElementIterator == this->ftpElementsIteratorEnd
	|| !this->isSelectedElementAnFtpElement)
	{
		this->createErrorMessageBox("Not selected any ftp file to download");
		return;
	}
	else if(this->checkIfElementIsFtpFolder(selectedElementIterator->getString()))
	{
		this->createErrorMessageBox("Can't download directory, only a single file at once");
		return;
	}

	sf::Ftp::Response response = this->ftpConenction.download
	(this->selectedElementIterator->getString().toAnsiString(), std::filesystem::current_path());
	if(!response.isOk())
	{
#if DEBUG_VERSION
		std::cout << response.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't download selected file");
		return;
	}

	this->localElements.emplace_back(this->font, this->selectedElementIterator->getString(),
	gui::calculateFontSize(this->window.getSize(), 16));
	this->localElements.back().setFillColor(sf::Color(0, 0, 0));
	if(this->localElementsPageNumber == static_cast <std::uint32_t>(std::ceil
	(static_cast <double>(this->ftpElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE)))
	{
		this->setLocalElementsIconsAndPositions();
	}
	this->resetSelectedElementIterator();
}

void ConnectedState::getFtpElements()
{
	sf::Ftp::DirectoryResponse currentDirectoryResponse = this->ftpConenction.getWorkingDirectory();
	if(!currentDirectoryResponse.isOk())
	{
#if DEBUG_VERSION
		std::cout << currentDirectoryResponse.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't get current directory");
		return;
	}

	std::string currentFtpDirectoryString = currentDirectoryResponse.getDirectory().string();
	if(currentFtpDirectoryString.length() > MAX_CHARACTERS_FOR_CURRENT_DIRECTORY)
	{
		currentFtpDirectoryString = currentFtpDirectoryString.substr
		(0, MAX_CHARACTERS_FOR_CURRENT_DIRECTORY - 3).append("...");
	}
	this->texts[textsNames::currentFtpDirectory].setString(currentFtpDirectoryString);

	sf::Ftp::ListingResponse elementsResponse = this->ftpConenction.getDirectoryListing();
	if(!elementsResponse.isOk())
	{
#if DEBUG_VERSION
		std::cout << elementsResponse.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't get files and folders list");
		return;
	}

	this->ftpElementsPageNumber = 1;
	this->ftpElements.clear();
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 16);
	for(const std::string & element : elementsResponse.getListing())
	{
		if(element.length() > MAX_CHARACTERS_FOR_ELEMENT)
		{
			this->ftpElements.emplace_back(this->font, 
			element.substr(0, MAX_CHARACTERS_FOR_ELEMENT - 3).append("..."), fontSize);
		}
		else
		{
			this->ftpElements.emplace_back(this->font, element, fontSize);
		}
		this->ftpElements.back().setFillColor(sf::Color(0, 0, 0));
	}
	
	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->ftpElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(numberOfPages == 0)
	{
		numberOfPages = 1;
	}
	this->setCurrentFtpPageText();
	this->ftpElementsIterator = this->ftpElements.begin();
}

void ConnectedState::getLocalElements()
{
	std::string currentLocalDirectoryString = std::filesystem::
	current_path().relative_path().string();
	if(currentLocalDirectoryString.length() > MAX_CHARACTERS_FOR_CURRENT_DIRECTORY)
	{
		currentLocalDirectoryString = currentLocalDirectoryString.substr
		(0, MAX_CHARACTERS_FOR_CURRENT_DIRECTORY - 3).append("...");
	}
	this->texts[textsNames::currentLocalDirectory].setString(currentLocalDirectoryString);

	this->localElementsPageNumber = 1;
	this->localElements.clear();
	const std::uint32_t fontSize = gui::calculateFontSize(this->window.getSize(), 16);
	std::string elementName;

	for(const std::filesystem::directory_entry & element 
	: std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		elementName = element.path().filename().string();
		if(elementName.length() > MAX_CHARACTERS_FOR_ELEMENT)
		{
			this->localElements.emplace_back(this->font, 
			elementName.substr(0, MAX_CHARACTERS_FOR_ELEMENT - 3).append("..."), fontSize);
		}
		else
		{
			this->localElements.emplace_back(this->font, elementName, fontSize);
		}
		this->localElements.back().setFillColor(sf::Color(0, 0, 0));
	}

	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->ftpElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(numberOfPages == 0)
	{
		numberOfPages = 1;
	}
	this->setCurrentLocalPageText();
	this->localElementsIterator = this->localElements.begin();
}

void ConnectedState::renameSelectedElement()
{
	if(this->selectedElementIterator == this->ftpElementsIteratorEnd
	|| !this->isSelectedElementAnFtpElement)
	{
		this->createErrorMessageBox("Not selected any ftp file/folder to rename");
		return;
	}

	std::string newName = this->inputBox.getInputString();
	if(newName.empty())
	{
		this->createErrorMessageBox("New name input is empty");
		return;
	}

	sf::Ftp::Response response = this->ftpConenction.renameFile
	(this->selectedElementIterator->getString().toAnsiString(), newName);
	if(!response.isOk())
	{
#if DEBUG_VERSION
		std::cout << response.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't rename file named: " 
		+ this->selectedElementIterator->getString());
		return;
	}
	this->selectedElementIterator->setString(newName);
	this->resetSelectedElementIterator();
}

void ConnectedState::render()
{
	this->renderLocalElements();
	this->renderFtpElements();
	this->window.draw(this->middleLine);
	this->window.draw(this->bottomLine);
	this->inputBox.render(this->window);
	this->renderButtons();
	this->renderTexts();

	if(this->selectedElementIterator != this->ftpElementsIteratorEnd)
	{
		this->window.draw(this->selectedElementBackground);
	}

	if(this->errorMessageBox != nullptr)
	{
		this->errorMessageBox->render(this->window);
	}
}

void ConnectedState::renderButtons()
{
	for(Button & button : this->buttons)
	{
		button.render(this->window);
	}
}

void ConnectedState::renderFtpElements()
{
	std::int32_t i = 0;
	for(std::list <sf::Text>::iterator it = this->ftpElementsIterator;
	it != this->ftpElementsIteratorEnd; ++it, ++i)
	{
		this->window.draw(* it);
		window.draw(this->ftpElementsIcons[i]);
	}
}

void ConnectedState::renderLocalElements()
{
	std::int32_t i = 0;
	for(std::list <sf::Text>::iterator it = this->localElementsIterator;
	it != this->localElementsIteratorEnd; ++it, ++i)
	{
		this->window.draw(* it);
		window.draw(this->localElementsIcons[i]);
	}
}

void ConnectedState::renderTexts()
{
	for(sf::Text & text : this->texts)
	{
		this->window.draw(text);
	}
}

void ConnectedState::resetSelectedElementIterator()
{
	this->selectedElementIterator = this->ftpElementsIteratorEnd;
}

void ConnectedState::setCurrentFtpPageText()
{
	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->ftpElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(numberOfPages == 0)
	{
		numberOfPages = 1;
	}

	this->texts[textsNames::currentFtpPageText].setString(std::to_string(ftpElementsPageNumber)
	+ '/' + std::to_string(numberOfPages));
}

void ConnectedState::setCurrentLocalPageText()
{
	std::size_t numberOfPages = static_cast <std::size_t>(std::ceil
	(static_cast <double>(this->localElements.size()) / MAX_NUMBER_OF_ELEMENTS_PER_PAGE));
	if(numberOfPages == 0)
	{
		numberOfPages = 1;
	}

	this->texts[textsNames::currentLocalPageText].setString(std::to_string(localElementsPageNumber)
	+ '/' + std::to_string(numberOfPages));
}

void ConnectedState::setFtpElementsIconsAndPositions()
{
	sf::Vector2f position = gui::calculatePercentageVector(window.getSize(), {55.f, 10.f});
	const float valueAddedToMoveIcon = static_cast <float>
	(gui::calculateFontSize(this->window.getSize(), 16)) / 4.f
	+ gui::calculatePercentageVector(this->window.getSize(), {3.f, 5.f}).y;

	std::list <sf::Text>::iterator copyOfIterator = this->ftpElementsIterator;

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE
	&& copyOfIterator != this->ftpElements.end(); ++i, ++copyOfIterator)
	{
		copyOfIterator->setPosition(position);
		position.y += valueAddedToMoveIcon;

		if(this->checkIfElementIsFtpFolder(copyOfIterator->getString()))
		{
			this->ftpElementsIcons[i].setTexture(& this->folderIcon);
		}
		else
		{
			this->ftpElementsIcons[i].setTexture(& this->fileIcon);
		}
	}

	this->ftpElementsIteratorEnd = copyOfIterator;
}

void ConnectedState::setIconsPositionsAndSizes()
{
	sf::Vector2f position = gui::calculatePercentageVector(window.getSize(), {42.f, 10.f});
	const sf::Vector2f size = gui::calculatePercentageVector(this->window.getSize(), {3.f, 5.f});
	const float valueAddedToMoveIcon = static_cast <float>
	(gui::calculateFontSize(this->window.getSize(), 16)) / 4.f;

	for(sf::RectangleShape & icon : this->localElementsIcons)
	{
		icon.setPosition(position);
		icon.setSize(size);
		position.y += size.y + valueAddedToMoveIcon;
	}

	position = gui::calculatePercentageVector(window.getSize(), {92.f, 10.f});
	for(sf::RectangleShape & icon : this->ftpElementsIcons)
	{
		icon.setPosition(position);
		icon.setSize(size);
		position.y += size.y + valueAddedToMoveIcon;
	}
}

void ConnectedState::setLocalElementsIconsAndPositions()
{
	sf::Vector2f position = gui::calculatePercentageVector(window.getSize(), {5.f, 10.f});
	const float valueAddedToMoveIcon = static_cast <float>
	(gui::calculateFontSize(this->window.getSize(), 16)) / 4.f
	+ gui::calculatePercentageVector(this->window.getSize(), {3.f, 5.f}).y;

	std::list <sf::Text>::iterator copyOfIterator = this->localElementsIterator;

	for(std::int32_t i = 0; i < MAX_NUMBER_OF_ELEMENTS_PER_PAGE
	&& copyOfIterator != this->localElements.end(); ++i, ++copyOfIterator)
	{
		copyOfIterator->setPosition(position);
		position.y += valueAddedToMoveIcon;

		if(std::filesystem::is_directory(std::filesystem::current_path().string()
		+ DIRECTORY_SEPARATOR + copyOfIterator->getString().toAnsiString()))
		{
			this->localElementsIcons[i].setTexture(&this->folderIcon);
		}
		else
		{
			this->localElementsIcons[i].setTexture(&this->fileIcon);
		}
	}

	this->localElementsIteratorEnd = copyOfIterator;
}

void ConnectedState::update(sf::Vector2f && mousePosition)
{
	void(this->ftpConenction.keepAlive());
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

	this->inputBox.update(mousePosition, this->keyPressed);
	for(Button & button : this->buttons)
	{
		button.update(mousePosition);
	}
	this->checkIfButtonsWerePressed();

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		this->checkIfElementIsSelected(mousePosition);
	}
}

void ConnectedState::uploadSelectedElementToFtpServer()
{
	if(this->selectedElementIterator == this->ftpElementsIteratorEnd
	|| this->isSelectedElementAnFtpElement)
	{
		this->createErrorMessageBox("Not selected any local file");
		return;
	}

	sf::Ftp::Response response = this->ftpConenction.upload
	(this->selectedElementIterator->getString().toAnsiString(), "");
	if(!response.isOk())
	{
#if DEBUG_VERSION
		std::cout << response.getMessage() << '\n';
#endif
		this->createErrorMessageBox("Can't upload file");
	}

	this->ftpElements.emplace_back(this->font, this->selectedElementIterator->getString(),
	gui::calculateFontSize(this->window.getSize(), 16));
	this->ftpElements.back().setFillColor(sf::Color(0, 0, 0));
	this->setFtpElementsIconsAndPositions();
	this->setCurrentFtpPageText();
	this->resetSelectedElementIterator();
}