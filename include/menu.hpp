#pragma once
#ifndef MENU_HPP
#define MENU_HPP
#include "stdlibs.hpp"

/////////////////////////////////////////////////////////////////
//DECLARATIONS FOR MENU.CPP
//Makros

#define MAX_LENGTH_SAVENAME 12
#define MAX_LENGTH_IP_PUBLIC 15
#define MAX_LENGTH_IP_LOCAL 15
#define MAX_LENGTH_PORT 6
sf::Text initErrorMessage(sf::Font *font, float scaling);
void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage);
void inputErrorDisplay(menuPopUps *menuPopUp, sf::Event *ev);
void escapeKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, gameMode mode);
mainMenuLayerId getPreviousLayer(mainMenuLayerId currentLayer, gameMode mode);
void enterKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, menuPopUps *menuWarning, Application *appPointer);
bool createSave(std::string newSaveName, menuPopUps *menuWarning, Application *appPointer);
void deleteKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer);
void textEntered(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer, sf::Event ev, gameMode mode);
bool validCharacterInput(mainMenuLayerId activeLayer, sf::Uint32 c, int activeLength, gameMode mode, int n);
void mouseButtonPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, Application *appPointer, gameMode mode);
void setActiveSafe(std::string saveName, std::string *pathToBeLoaded, std::vector<gameSaveSummary> availableSaveFiles);
void getMenuPicks(std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, gameMode *mode);


void setMenuPopUpMessage(void);


#endif //MENU_HPP